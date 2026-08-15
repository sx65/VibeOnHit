#include "../framework.h"

using namespace ImGui;

bool framework::sub_tab( const char* name, bool selected )
{
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    constexpr float sub_tab_bar_height = 18.f;
    constexpr float horizontal_padding = 4.f;
    constexpr float vertical_offset = -1.f;

    PushFont( fonts::main_10px );

    const ImVec2 text_size = CalcTextSize( name );
    const float button_width = text_size.x + ( horizontal_padding * 2.0f );

    const ImVec2 base_pos = window->DC.CursorPos;
    const ImVec2 pos = base_pos + ImVec2( -6.f, vertical_offset - sub_tab_bar_height * 0.25f );

    const ImVec2 size( button_width, sub_tab_bar_height );
    const ImRect bb( pos, pos + size );

    ItemSize( size, 0.f );
    ImGuiID id = window->GetID( name );
    if ( !ItemAdd( bb, id ) )
    {
        PopFont( );
        return false;
    }

    bool hovered, held, pressed = ButtonBehavior( bb, id, &hovered, &held, 0 );

    PushStyleColor( ImGuiCol_Text, selected ? colors::accent : ( hovered ? colors::text_hovered : colors::text ) );
    RenderTextClipped( bb.Min, bb.Max, name, NULL, NULL, ImVec2( 0.5f, 0.5f ) );
    PopStyleColor( );

    PopFont( );

    return pressed;
}
