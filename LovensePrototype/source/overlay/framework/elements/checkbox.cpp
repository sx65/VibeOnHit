#include "../framework.h"

using namespace ImGui;

bool framework::checkbox( const char* label, bool* v )
{
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    PushFont( fonts::main_12px );

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID( label );
    const ImVec2 label_size = CalcTextSize( label, NULL, true );

    constexpr float text_padding_x = 6.f;
    constexpr float text_padding_y = -2.f;
    constexpr float square_size = 12.f;
    const ImVec2 pos = window->DC.CursorPos + ImVec2( 1.f, 1.f );

    const ImRect check_bb( pos, ImVec2( pos.x + square_size, pos.y + square_size ) );
    const ImRect total_bb(
        ImVec2( pos.x, pos.y ),
        ImVec2( check_bb.Max.x + label_size.x + text_padding_x + 1.f, pos.y + square_size + text_padding_y + 2.f )
    );

    PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0.f, 0.f ) );
    ItemSize( total_bb, 0.f );
    PopStyleVar( );

    if ( !ItemAdd( total_bb, id ) )
    {
        PopFont( );
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior( total_bb, id, &hovered, &held );
    if ( pressed )
    {
        *v = !( *v );
        MarkItemEdited( id );
    }

    window->DrawList->AddRectFilled( check_bb.Min, check_bb.Max, *v ? colors::accent : colors::title_bar );
    window->DrawList->AddRect( check_bb.Min, check_bb.Max, colors::border );

    PushStyleColor( ImGuiCol_Text, *v ? colors::text_active : ( hovered ? colors::text_hovered : colors::text ) );
    RenderText( ImVec2( check_bb.Max.x + text_padding_x, check_bb.Min.y + text_padding_y ), label );
    PopStyleColor( );

    SetCursorPosY( GetCursorPosY( ) + square_size - 7.f );

    PopFont( );

    return pressed;
}
