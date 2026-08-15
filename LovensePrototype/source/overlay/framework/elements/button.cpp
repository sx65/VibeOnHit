#include "../framework.h"

using namespace ImGui;

bool framework::button( const char* label, const ImVec2& size_arg )
{
    return button_ex( label, size_arg, ImGuiButtonFlags_None );
}

bool framework::button_ex( const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags )
{
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    PushFont( fonts::main_12px );

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID( label );
    const ImVec2 label_size = CalcTextSize( label, NULL, true );

    ImVec2 pos = window->DC.CursorPos;
    if ( ( flags & ImGuiButtonFlags_AlignTextBaseLine ) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset ) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize( size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f );

    const ImRect bb( pos, pos + size );
    ItemSize( size, style.FramePadding.y );
    if ( !ItemAdd( bb, id ) )
    {
        PopFont( );
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held, flags );

    // Render
    const ImU32 col = GetColorU32( ( held && hovered ) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button );
    RenderNavCursor( bb, id );
    window->DrawList->AddRectFilled( bb.Min, bb.Max, colors::title_bar );
    window->DrawList->AddRect( bb.Min, bb.Max, colors::border );

    if ( g.LogEnabled )
        LogSetNextTextDecoration( "[", "]" );
    PushStyleColor( ImGuiCol_Text, hovered ? colors::text_hovered : colors::text );
    RenderTextClipped( bb.Min, bb.Max, label, NULL, &label_size, style.ButtonTextAlign, &bb );
    PopStyleColor( );

    // Automatically close popups
    //if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
    //    CloseCurrentPopup();

    PopFont( );

    IMGUI_TEST_ENGINE_ITEM_INFO( id, label, g.LastItemData.StatusFlags );
    return pressed;
}
