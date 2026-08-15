#include "../framework.h"

using namespace ImGui;

bool framework::begin_list_box( const char* label, const ImVec2& size_arg )
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = GetID( label );
    const ImVec2 label_size = CalcTextSize( label, NULL, true );

    // Size default to hold ~7.25 items.
    // Fractional number of items helps seeing that we can scroll down/up without looking at scrollbar.
    ImVec2 size = ImTrunc( CalcItemSize( size_arg, CalcItemWidth( ), GetTextLineHeightWithSpacing( ) * 7.25f + style.FramePadding.y * 2.0f ) );
    ImVec2 frame_size = ImVec2( size.x, ImMax( size.y, label_size.y ) );
    ImRect frame_bb( window->DC.CursorPos, window->DC.CursorPos + frame_size );
    ImRect bb( frame_bb.Min, frame_bb.Max + ImVec2( label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f ) );
    g.NextItemData.ClearFlags( );

    if ( !IsRectVisible( bb.Min, bb.Max ) )
    {
        ItemSize( bb.GetSize( ), style.FramePadding.y );
        ItemAdd( bb, 0, &frame_bb );
        g.NextWindowData.ClearFlags( ); // We behave like Begin() and need to consume those values
        return false;
    }

    // FIXME-OPT: We could omit the BeginGroup() if label_size.x == 0.0f but would need to omit the EndGroup() as well.
    BeginGroup( );
    if ( label_size.x > 0.0f )
    {
        ImVec2 label_pos = ImVec2( frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y );
        RenderText( label_pos, label );
        window->DC.CursorMaxPos = ImMax( window->DC.CursorMaxPos, label_pos + label_size );
        AlignTextToFramePadding( );
    }

    begin_child( id, frame_bb.GetSize( ), ImGuiChildFlags_FrameStyle );
    return true;
}

void framework::end_list_box( )
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    IM_ASSERT( ( window->Flags & ImGuiWindowFlags_ChildWindow ) && "Mismatched BeginListBox/EndListBox calls. Did you test the return value of BeginListBox?" );
    IM_UNUSED( window );

    end_child( );
    EndGroup( ); // This is only required to be able to do IsItemXXX query on the whole ListBox including label
}
