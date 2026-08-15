#include "../framework.h"

using namespace ImGui;

void framework::separator( )
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if ( window->SkipItems )
        return;

    ImGuiSeparatorFlags flags = ( window->DC.LayoutType == ImGuiLayoutType_Horizontal ) ? ImGuiSeparatorFlags_Vertical : ImGuiSeparatorFlags_Horizontal;

    if ( window->DC.CurrentColumns )
        flags |= ImGuiSeparatorFlags_SpanAllColumns;

    separator_ex( flags, 1.0f );
}

void framework::separator_ex( ImGuiSeparatorFlags flags, float thickness )
{
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return;

    ImGuiContext& g = *GImGui;
    IM_ASSERT( ImIsPowerOfTwo( flags & ( ImGuiSeparatorFlags_Horizontal | ImGuiSeparatorFlags_Vertical ) ) );   // Check that only 1 option is selected
    IM_ASSERT( thickness > 0.0f );

    Dummy( ImVec2( 0.f, 3.f ) );
    if ( flags & ImGuiSeparatorFlags_Vertical )
    {
        float y1 = window->DC.CursorPos.y;
        float y2 = window->DC.CursorPos.y + window->DC.CurrLineSize.y;
        const ImRect bb( ImVec2( window->DC.CursorPos.x, y1 ), ImVec2( window->DC.CursorPos.x + thickness, y2 ) );
        ItemSize( ImVec2( thickness, 0.0f ) );
        if ( !ItemAdd( bb, 0 ) )
            return;

        window->DrawList->AddRectFilled( bb.Min, bb.Max, colors::border );
        if ( g.LogEnabled )
            LogText( " |" );
    }
    else if ( flags & ImGuiSeparatorFlags_Horizontal )
    {
        float x1 = window->DC.CursorPos.x;
        float x2 = window->WorkRect.Max.x;

        ImGuiOldColumns* columns = ( flags & ImGuiSeparatorFlags_SpanAllColumns ) ? window->DC.CurrentColumns : NULL;
        if ( columns )
        {
            x1 = window->Pos.x + window->DC.Indent.x; // Used to be Pos.x before 2023/10/03
            x2 = window->Pos.x + window->Size.x;
            PushColumnsBackground( );
        }

        const float thickness_for_layout = ( thickness == 1.0f ) ? 0.0f : thickness; // FIXME: See 1.70/1.71 Separator() change: makes legacy 1-px separator not affect layout yet. Should change.
        const ImRect bb( ImVec2( x1, window->DC.CursorPos.y ), ImVec2( x2, window->DC.CursorPos.y + thickness ) );
        ItemSize( ImVec2( 0.0f, thickness_for_layout ) );

        if ( ItemAdd( bb, 0 ) )
        {
            window->DrawList->AddRectFilled( bb.Min, bb.Max, colors::border );
            if ( g.LogEnabled )
                LogRenderedText( &bb.Min, "--------------------------------\n" );

        }
        if ( columns )
        {
            PopColumnsBackground( );
            columns->LineMinY = window->DC.CursorPos.y;
        }
    }
    SetCursorPosY( GetCursorPosY( ) + 2.f );
}
