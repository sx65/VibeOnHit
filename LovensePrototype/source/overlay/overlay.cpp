#include "overlay.h"

#include <string>
#include <memory/memory.h>
#include <thread>
#include <dwmapi.h>
#pragma comment( lib, "dwmapi.lib" )

#include "fonts/fonts.h"
#include "settings/settings.h"
#include "../game/game.h"
#include "../features/features.h"
#include "framework/framework.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

ID3D11Device* COverlay::D3DDevice = nullptr;
ID3D11DeviceContext* COverlay::D3DDeviceContext = nullptr;
IDXGISwapChain* COverlay::SwapChain = nullptr;
UINT COverlay::ResizeWidth = 0;
UINT COverlay::ResizeHeight = 0;
ID3D11RenderTargetView* COverlay::RenderTargetView = nullptr;

void COverlay::start( void ) {
    std::wstring class_name( L"Chrome_RenderWidgetHostHWND" );
    std::wstring window_name( L"Chrome Legacy Window" );

    WNDCLASSEXW wc{ sizeof( wc ) };
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle( nullptr );
    wc.lpszClassName = class_name.c_str();

    ATOM atom = ::RegisterClassExW( &wc );
    if ( !atom ) return;

    RECT window_rect{};
    GetWindowRect( memory.get_window_handle(), &window_rect );
    window_rect.left -= 75;

    int width = ( window_rect.right - window_rect.left ) + 10;
    int height = ( window_rect.bottom - window_rect.top ) + 10;

    HWND& hwnd = overlay_hwnd;
    hwnd = CreateWindowExW(
        WS_EX_NOACTIVATE,
        wc.lpszClassName,
        window_name.c_str(),
        WS_POPUP,
        window_rect.left - 5, window_rect.top - 5,
        width, height,
        memory.get_window_handle(),
        nullptr, wc.hInstance, nullptr
    );

    MARGINS margin{ 0, 0, width, height };
    DwmExtendFrameIntoClientArea( hwnd, &margin );
    if ( !hwnd ) return;

    DWORD exStyle = WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW;
    SetWindowLong( hwnd, GWL_EXSTYLE, exStyle );
    SetLayeredWindowAttributes( hwnd, 0, 255, LWA_ALPHA );

    ShowWindow( hwnd, SW_SHOWDEFAULT );
    UpdateWindow( hwnd );

    if ( !CreateDeviceD3D( hwnd ) ) {
        CleanupDeviceD3D();
        ::UnregisterClassW( wc.lpszClassName, wc.hInstance );
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    io.IniFilename = nullptr;
    io.Fonts->Flags |= ImFontAtlasFlags_NoBakedLines;
    io.Fonts->TexGlyphPadding = 1;

    ImGui::StyleColorsDark();

    framework::setup();

    ImGui_ImplWin32_Init( hwnd );
    ImGui_ImplDX11_Init( D3DDevice, D3DDeviceContext );

    ImFontConfig cfg;
    cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_Monochrome | ImGuiFreeTypeBuilderFlags_MonoHinting;
    cfg.FontDataOwnedByAtlas = false;

    variables->font.tahoma = io.Fonts->AddFontFromMemoryTTF( tahoma_hex, sizeof tahoma_hex, 12.f, &cfg, io.Fonts->GetGlyphRangesCyrillic() );
    variables->font.anta = io.Fonts->AddFontFromMemoryTTF( anta_hex, sizeof anta_hex, 12.f, &cfg, io.Fonts->GetGlyphRangesCyrillic() );
    variables->font.verdana = io.Fonts->AddFontFromMemoryTTF( verdana_hex, sizeof verdana_hex, 12.f, &cfg, io.Fonts->GetGlyphRangesCyrillic() ); 
    variables->font.pixel = io.Fonts->AddFontFromMemoryTTF( pixel_hex, sizeof pixel_hex, 12.f, &cfg, io.Fonts->GetGlyphRangesCyrillic() );

    ImGuiFreeType::GetBuilderForFreeType()->FontBuilder_Build( io.Fonts );

    bool insert_pressed = false;
    DWORD last_ex_style = exStyle;
    variables->gui.menu_opened = true;

    while ( true ) {
        game->view_matrix = game->visual_engine.get_view_matrix();
        game->dimensions = game->visual_engine.get_dimensions();

        MSG msg;
        while ( ::PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) ) {
            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
            if ( msg.message == WM_QUIT ) goto cleanup;
        }

        if ( ResizeWidth && ResizeHeight ) {
            CleanupRenderTarget();
            SwapChain->ResizeBuffers( 0, ResizeWidth, ResizeHeight, DXGI_FORMAT_UNKNOWN, 0 );
            ResizeWidth = ResizeHeight = 0;
            CreateRenderTarget();
        }

        top_hwnd = GetForegroundWindow();

        const SHORT key_state = GetAsyncKeyState( variables->gui.menu_key );
        const bool key_down = ( key_state & 0x8000 ) != 0;

        if ( top_hwnd == memory.get_window_handle() || top_hwnd == overlay_hwnd ) {
            if ( !variables->gui.show_loader && ( key_state & 0x1 ) && !insert_pressed ) {
                variables->gui.menu_opened ^= 1;
                if ( !variables->gui.menu_opened )
                    BringWindowToTop( memory.get_window_handle() );
            }
            insert_pressed = key_down;

            DWORD new_ex_style = variables->gui.show_loader
                ? ( WS_EX_LAYERED | WS_EX_TOOLWINDOW )
                : ( variables->gui.menu_opened
                    ? ( WS_EX_LAYERED | WS_EX_TOOLWINDOW )
                    : ( WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW ) );

            if ( new_ex_style != last_ex_style ) {
                SetWindowLong( hwnd, GWL_EXSTYLE, new_ex_style );
                last_ex_style = new_ex_style;
            }
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        move_window( hwnd );

        //features::Visuals::esp();

        ImGui::PushFont( variables->font.verdana );
        render_list.draw( ImGui::GetBackgroundDrawList() );
        ImGui::PopFont();

        if ( variables->gui.menu_opened ) {
            framework::render( "ext" );
        }

        ImGui::Render();
        const float clear_color_with_alpha[ 4 ]{ 0.f, 0.f, 0.f, 0.f };
        D3DDeviceContext->OMSetRenderTargets( 1, &RenderTargetView, nullptr );
        D3DDeviceContext->ClearRenderTargetView( RenderTargetView, clear_color_with_alpha );
        ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
        SwapChain->Present( 1, 0 );
    }

cleanup:
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    ::DestroyWindow( hwnd );
    ::UnregisterClassW( wc.lpszClassName, wc.hInstance );
}

bool COverlay::fullscreen( HWND window_handle ) {
    MONITORINFO monitorInfo = { sizeof( MONITORINFO ) };
    if ( !GetMonitorInfo( MonitorFromWindow( window_handle, MONITOR_DEFAULTTOPRIMARY ), &monitorInfo ) )
        return false;

    RECT windowRect{};
    if ( !GetWindowRect( window_handle, &windowRect ) )
        return false;

    return windowRect.left == monitorInfo.rcMonitor.left
        && windowRect.right == monitorInfo.rcMonitor.right
        && windowRect.top == monitorInfo.rcMonitor.top
        && windowRect.bottom == monitorInfo.rcMonitor.bottom;
}

static RECT last_target_rc{};
static HWND last_fore_hwnd = nullptr;
static DWORD last_style = 0;

void COverlay::move_window( HWND window_handle ) {
    RECT rc_now{};
    if ( !GetWindowRect( memory.get_window_handle(), &rc_now) )
        return;

    if ( top_hwnd == memory.get_window_handle() || memory.get_window_handle() == overlay_hwnd ) {
        if ( !GetCursorPos( &cursor ) )
            return;
        ScreenToClient( memory.get_window_handle(), &cursor );
    }

    const bool is_fs = fullscreen( memory.get_window_handle() );

    int w = rc_now.right - rc_now.left - ( is_fs ? 1 : 17 );
    int h = rc_now.bottom - rc_now.top;

    if ( !is_fs ) {
        w += 1;
        rc_now.left += 9;
        rc_now.top += 31;
        h -= 39;
    }

    if ( EqualRect( &last_target_rc, &rc_now ) &&
        last_fore_hwnd == memory.get_window_handle() &&
        last_style == ( is_fs ? 1u : 0u ) )
        return;

    last_target_rc = rc_now;
    last_fore_hwnd = memory.get_window_handle();
    last_style = is_fs ? 1u : 0u;

    ::MoveWindow( overlay_hwnd, rc_now.left, rc_now.top, w, h, FALSE );
}

bool COverlay::CreateDeviceD3D( HWND hwnd ) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 240;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT create_device_flags = 0;
    D3D_FEATURE_LEVEL feature_level;
    const D3D_FEATURE_LEVEL feature_level_array[ 2 ] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    HRESULT res = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, create_device_flags,
        feature_level_array, 2, D3D11_SDK_VERSION, &sd, &SwapChain,
        &D3DDevice, &feature_level, &D3DDeviceContext );
    if ( res == DXGI_ERROR_UNSUPPORTED ) {
        res = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_WARP, nullptr, create_device_flags,
            feature_level_array, 2, D3D11_SDK_VERSION, &sd, &SwapChain,
            &D3DDevice, &feature_level, &D3DDeviceContext );
    }
    if ( res != S_OK ) {
        return false;
    }

    CreateRenderTarget();
    return true;
}

void COverlay::CleanupDeviceD3D( void ) {
    CleanupRenderTarget();
    if ( SwapChain ) {
        SwapChain->Release();
        SwapChain = nullptr;
    }
    if ( D3DDeviceContext ) {
        D3DDeviceContext->Release();
        D3DDeviceContext = nullptr;
    }
    if ( D3DDevice ) {
        D3DDevice->Release();
        D3DDevice = nullptr;
    }
}

void COverlay::CreateRenderTarget( void ) {

    ID3D11Texture2D* p_back_buffer = nullptr;
    SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast< LPVOID* >( &p_back_buffer ) );
    D3DDevice->CreateRenderTargetView( p_back_buffer, nullptr, &RenderTargetView );
    if ( p_back_buffer ) {
        p_back_buffer->Release();
    }
}

void COverlay::CleanupRenderTarget( void ) {
    if ( RenderTargetView ) {
        RenderTargetView->Release();
        RenderTargetView = nullptr;
    }
}

LRESULT __stdcall COverlay::WndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) {
    if ( ImGui_ImplWin32_WndProcHandler( hwnd, msg, wparam, lparam ) )
        return true;

    switch ( msg ) {
    case WM_SIZE:
        if ( wparam == SIZE_MINIMIZED ) {
            return 0;
        }
        ResizeWidth = static_cast< UINT >( LOWORD( lparam ) );
        ResizeHeight = static_cast< UINT >( HIWORD( lparam ) );
        return 0;
    case WM_SYSCOMMAND:
        if ( ( wparam & 0xfff0 ) == SC_KEYMENU )
            return 0;

        break;
    case WM_DESTROY:
        ::PostQuitMessage( 0 );
        return 0;
    }

    return ::DefWindowProcW( hwnd, msg, wparam, lparam );
}
