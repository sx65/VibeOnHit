#pragma once

#include "imgui_includes.h"
#include "directx_includes.h"
#include "render_list/render_list.h"

class COverlay final {
public:
	void start( void );

	bool fullscreen( HWND window_handle );
	void move_window( HWND window_handle );

	POINT cursor;
    HWND top_hwnd;
    HWND overlay_hwnd;

    render::render_list render_list;

private:
    static ID3D11Device* D3DDevice;
    static ID3D11DeviceContext* D3DDeviceContext;
    static IDXGISwapChain* SwapChain;
    static UINT ResizeWidth;
    static UINT ResizeHeight;
    static ID3D11RenderTargetView* RenderTargetView;

    static bool CreateDeviceD3D( HWND hwnd );
    static void CleanupDeviceD3D( void );
    static void CreateRenderTarget( void );
    static void CleanupRenderTarget( void );
    static LRESULT WINAPI WndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );
};

inline COverlay overlay;