#include "../framework.h"

using namespace ImGui;

bool framework::tab( const char* name, bool selected )
{
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    constexpr float title_bar_height = 27.f;
    constexpr float horizontal_padding = 13.f;
    constexpr float vertical_offset = -0.5f;

    const ImVec2 text_size = CalcTextSize( name );
    const float button_width = text_size.x + ( horizontal_padding * 2.0f );

    const ImVec2 base_pos = window->DC.CursorPos;
    const ImVec2 pos = base_pos + ImVec2( -7.f, vertical_offset - title_bar_height * 0.3f );

    const ImVec2 size( button_width, title_bar_height );
    const ImRect bb( pos, pos + size );

    ItemSize( size, 0.f );
    ImGuiID id = window->GetID( name );
    if ( !ItemAdd( bb, id ) )
        return false;

    bool hovered, held, pressed = ButtonBehavior( bb, id, &hovered, &held, 0 );

    if ( selected )
    {
        float center_x = ( bb.Min.x + bb.Max.x ) * 0.5f;
        float center_y = ( bb.Min.y + bb.Max.y ) * 0.5f;

        float line_y = center_y + ( text_size.y * 0.5f ) + 4.5f;

        float half_width = button_width * 0.4f;
        ImVec2 line_start( center_x - half_width, line_y );
        ImVec2 line_end( center_x + half_width - 0.5f, line_y );

        window->DrawList->AddLine( line_start, line_end, colors::accent );
    }

    PushStyleColor( ImGuiCol_Text, selected ? colors::accent : ( hovered ? colors::text_hovered : colors::text ) );
    RenderTextClipped( bb.Min + ImVec2( 1.f, 0.f ), bb.Max, name, NULL, NULL, ImVec2( 0.5f, 0.5f ) );
    PopStyleColor( );

    return pressed;
}
