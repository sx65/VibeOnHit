#include "../framework.h"

using namespace ImGui;

static ImGuiInputSource nav_calc_preferred_ref_pos_source( );
static ImVec2 nav_calc_preferred_ref_pos( );
static ImVec2 calc_next_scroll_from_scroll_target_and_clamp( ImGuiWindow* window );
static float calc_scroll_edge_snap( float target, float snap_min, float snap_max, float snap_threshold, float center_ratio );

bool framework::is_popup_open( ImGuiID id, ImGuiPopupFlags popup_flags )
{
    ImGuiContext& g = *GImGui;
    if ( popup_flags & ImGuiPopupFlags_AnyPopupId )
    {
        IM_ASSERT( id == 0 );
        if ( popup_flags & ImGuiPopupFlags_AnyPopupLevel )
            return g.OpenPopupStack.Size > 0;
        else
            return g.OpenPopupStack.Size > g.BeginPopupStack.Size;
    }
    else
    {
        if ( popup_flags & ImGuiPopupFlags_AnyPopupLevel )
        {
            for ( int n = 0; n < g.OpenPopupStack.Size; n++ )
                if ( g.OpenPopupStack[n].PopupId == id )
                    return true;
            return false;
        }
        else
            return g.OpenPopupStack.Size > g.BeginPopupStack.Size && g.OpenPopupStack[g.BeginPopupStack.Size].PopupId == id;
    }
}

bool framework::is_popup_open( const char* str_id, ImGuiPopupFlags popup_flags )
{
    ImGuiContext& g = *GImGui;
    ImGuiID id = ( popup_flags & ImGuiPopupFlags_AnyPopupId ) ? 0 : g.CurrentWindow->GetID( str_id );
    if ( ( popup_flags & ImGuiPopupFlags_AnyPopupLevel ) && id != 0 )
        IM_ASSERT( 0 && "Cannot use IsPopupOpen() with a string id and ImGuiPopupFlags_AnyPopupLevel." );
    return is_popup_open( id, popup_flags );
}

void framework::open_popup( const char* str_id, ImGuiPopupFlags popup_flags )
{
    ImGuiContext& g = *GImGui;
    ImGuiID id = g.CurrentWindow->GetID( str_id );
    IMGUI_DEBUG_LOG_POPUP( "[popup] OpenPopup(\"%s\" -> 0x%08X)\n", str_id, id );
    open_popup_ex( id, popup_flags );
}

void framework::open_popup_ex( ImGuiID id, ImGuiPopupFlags popup_flags )
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* parent_window = g.CurrentWindow;
    const int current_stack_size = g.BeginPopupStack.Size;

    if ( popup_flags & ImGuiPopupFlags_NoOpenOverExistingPopup )
        if ( IsPopupOpen( ( ImGuiID )0, ImGuiPopupFlags_AnyPopupId ) )
            return;

    ImGuiPopupData popup_ref;
    popup_ref.PopupId = id;
    popup_ref.Window = NULL;
    popup_ref.RestoreNavWindow = g.NavWindow;
    popup_ref.OpenFrameCount = g.FrameCount;
    popup_ref.OpenParentId = parent_window->IDStack.back( );
    popup_ref.OpenPopupPos = nav_calc_preferred_ref_pos( );
    popup_ref.OpenMousePos = IsMousePosValid( &g.IO.MousePos ) ? g.IO.MousePos : popup_ref.OpenPopupPos;

    IMGUI_DEBUG_LOG_POPUP( "[popup] OpenPopupEx(0x%08X)\n", id );
    if ( g.OpenPopupStack.Size < current_stack_size + 1 )
        g.OpenPopupStack.push_back( popup_ref );
    else
    {
        bool keep_existing = false;
        if ( g.OpenPopupStack[current_stack_size].PopupId == id )
            if ( ( g.OpenPopupStack[current_stack_size].OpenFrameCount == g.FrameCount - 1 ) || ( popup_flags & ImGuiPopupFlags_NoReopen ) )
                keep_existing = true;
        if ( keep_existing )
            g.OpenPopupStack[current_stack_size].OpenFrameCount = popup_ref.OpenFrameCount;
        else
        {
            ClosePopupToLevel( current_stack_size, true );
            g.OpenPopupStack.push_back( popup_ref );
        }
    }
}

bool framework::begin_popup( const char* str_id, ImGuiWindowFlags flags )
{
    ImGuiContext& g = *GImGui;
    if ( g.OpenPopupStack.Size <= g.BeginPopupStack.Size ) // Early out for performance
    {
        g.NextWindowData.ClearFlags( ); // We behave like Begin() and need to consume those values
        return false;
    }
    flags |= ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
    ImGuiID id = g.CurrentWindow->GetID( str_id );
    return begin_popup_ex( id, str_id, flags );
}

bool framework::begin_popup_ex( ImGuiID id, const char* title_bar, ImGuiWindowFlags extra_window_flags )
{
    ImGuiContext& g = *GImGui;
    if ( !is_popup_open( id, ImGuiPopupFlags_None ) )
    {
        g.NextWindowData.ClearFlags( );
        return false;
    }

    PushFont( fonts::main_12px );

    char name[20];
    IM_ASSERT( ( extra_window_flags & ImGuiWindowFlags_ChildMenu ) == 0 ); // Use BeginPopupMenuEx()
    ImFormatString( name, IM_ARRAYSIZE( name ), "##Popup_%08x", id ); // No recycling, so we can close/open during the same frame

    bool using_title_bar = extra_window_flags & ImGuiWindowFlags_PopupUseTitleBar;
    bool is_open = Begin( name, NULL, extra_window_flags | ImGuiWindowFlags_Popup );
    if ( !is_open )
        end_popup( );
    else
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;

        window->DrawList->AddRectFilled( window->Pos, window->Pos + window->Size, using_title_bar ? colors::child_bg : colors::title_bar );
        window->DrawList->AddRect( window->Pos, window->Pos + window->Size, colors::border, 0.f, 0, 3.f );

        if ( using_title_bar )
        {
            static constexpr float TITLE_BAR_HEIGHT = 22.f;

            ImGuiContext& g = *GImGui;
            ImGuiStyle& style = g.Style;

            float pad_l = style.FramePadding.x + 4.f;
            float pad_r = style.FramePadding.x + 4.f;

            ImRect title_bar_rect = ImRect( window->Pos, ImVec2( window->Pos.x + window->SizeFull.x, window->Pos.y + TITLE_BAR_HEIGHT ) );
            const ImVec2 text_size = CalcTextSize( name, NULL, true );

            if ( style.WindowTitleAlign.x > 0.0f && style.WindowTitleAlign.x < 1.0f )
            {
                float centerness = ImSaturate( 1.0f - ImFabs( style.WindowTitleAlign.x - 0.5f ) * 2.0f ); // 0.0f on either edges, 1.0f on center
                float pad_extend = ImMin( ImMax( pad_l, pad_r ), title_bar_rect.GetWidth( ) - pad_l - pad_r - text_size.x );
                pad_l = ImMax( pad_l, pad_extend * centerness );
                pad_r = ImMax( pad_r, pad_extend * centerness );
            }

            ImRect layout_r( title_bar_rect.Min.x + pad_l, title_bar_rect.Min.y, title_bar_rect.Max.x - pad_r, title_bar_rect.Max.y - 2.f );
            ImRect clip_r( layout_r.Min.x, layout_r.Min.y, ImMin( layout_r.Max.x + g.Style.ItemInnerSpacing.x, title_bar_rect.Max.x ), layout_r.Max.y );

            window->DrawList->AddRectFilled( window->Pos, window->Pos + ImVec2( window->Size.x, TITLE_BAR_HEIGHT ), colors::title_bar );
            window->DrawList->AddRectFilled( ImVec2( title_bar_rect.Min.x, title_bar_rect.Max.y ), ImVec2( title_bar_rect.Max.x, title_bar_rect.Max.y + 2.f ), framework::colors::accent );
            window->DrawList->AddRect( window->Pos, window->Pos + window->Size, colors::border, 0.f, 0, 3.f );

            PushStyleColor( ImGuiCol_Text, framework::colors::accent );
            RenderTextClipped( layout_r.Min, layout_r.Max, title_bar, NULL, &text_size, ImVec2( 0.f, 0.5f ), &clip_r );
            PopStyleColor( );

            SetCursorPosY( GetCursorPosY( ) + TITLE_BAR_HEIGHT * 0.5f + 2.f );
        }
    }

    PopFont( );

    return is_open;
}

void framework::end_popup( )
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    IM_ASSERT( window->Flags & ImGuiWindowFlags_Popup );
    IM_ASSERT( g.BeginPopupStack.Size > 0 );

    if ( g.NavWindow == window )
        NavMoveRequestTryWrapping( window, ImGuiNavMoveFlags_LoopY );

    const ImGuiID backup_within_end_child_id = g.WithinEndChildID;
    if ( window->Flags & ImGuiWindowFlags_ChildWindow )
        g.WithinEndChildID = window->ID;
    End( );
    g.WithinEndChildID = backup_within_end_child_id;
}

bool framework::begin_popup_context_item( const char* str_id, ImGuiPopupFlags popup_flags )
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if ( window->SkipItems )
        return false;
    ImGuiID id = str_id ? window->GetID( str_id ) : g.LastItemData.ID;    // If user hasn't passed an ID, we can use the LastItemID. Using LastItemID as a Popup ID won't conflict!
    IM_ASSERT( id != 0 );                                             // You cannot pass a NULL str_id if the last item has no identifier (e.g. a Text() item)
    int mouse_button = ( popup_flags & ImGuiPopupFlags_MouseButtonMask_ );
    if ( IsMouseReleased( mouse_button ) && IsItemHovered( ImGuiHoveredFlags_AllowWhenBlockedByPopup ) )
        open_popup_ex( id, popup_flags );
    return begin_popup_ex( id, str_id, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings );
}

static ImGuiInputSource nav_calc_preferred_ref_pos_source( )
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.NavWindow;
    const bool activated_shortcut = g.ActiveId != 0 && g.ActiveIdFromShortcut && g.ActiveId == g.LastItemData.ID;

    if ( ( !g.NavCursorVisible || !g.NavHighlightItemUnderNav || !window ) && !activated_shortcut )
        return ImGuiInputSource_Mouse;
    else
        return ImGuiInputSource_Keyboard;
}

static ImVec2 nav_calc_preferred_ref_pos( )
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.NavWindow;
    ImGuiInputSource source = nav_calc_preferred_ref_pos_source( );

    const bool activated_shortcut = g.ActiveId != 0 && g.ActiveIdFromShortcut && g.ActiveId == g.LastItemData.ID;

    if ( source == ImGuiInputSource_Mouse )
    {
        ImVec2 p = IsMousePosValid( &g.IO.MousePos ) ? g.IO.MousePos : g.MouseLastValidPos;
        return ImVec2( p.x + 1.0f, p.y );
    }
    else
    {
        ImRect ref_rect;
        if ( activated_shortcut )
            ref_rect = g.LastItemData.NavRect;
        else
            ref_rect = WindowRectRelToAbs( window, window->NavRectRel[g.NavLayer] );

        if ( window->LastFrameActive != g.FrameCount && ( window->ScrollTarget.x != FLT_MAX || window->ScrollTarget.y != FLT_MAX ) )
        {
            ImVec2 next_scroll = calc_next_scroll_from_scroll_target_and_clamp( window );
            ref_rect.Translate( window->Scroll - next_scroll );
        }
        ImVec2 pos = ImVec2( ref_rect.Min.x + ImMin( g.Style.FramePadding.x * 4, ref_rect.GetWidth( ) ), ref_rect.Max.y - ImMin( g.Style.FramePadding.y, ref_rect.GetHeight( ) ) );
        ImGuiViewport* viewport = GetMainViewport( );
        return ImTrunc( ImClamp( pos, viewport->Pos, viewport->Pos + viewport->Size ) ); // ImTrunc() is important because non-integer mouse position application in backend might be lossy and result in undesirable non-zero delta.
    }
}

static ImVec2 calc_next_scroll_from_scroll_target_and_clamp( ImGuiWindow* window )
{
    ImVec2 scroll = window->Scroll;
    ImVec2 decoration_size( window->DecoOuterSizeX1 + window->DecoInnerSizeX1 + window->DecoOuterSizeX2, window->DecoOuterSizeY1 + window->DecoInnerSizeY1 + window->DecoOuterSizeY2 );
    for ( int axis = 0; axis < 2; axis++ )
    {
        if ( window->ScrollTarget[axis] < FLT_MAX )
        {
            float center_ratio = window->ScrollTargetCenterRatio[axis];
            float scroll_target = window->ScrollTarget[axis];
            if ( window->ScrollTargetEdgeSnapDist[axis] > 0.0f )
            {
                float snap_min = 0.0f;
                float snap_max = window->ScrollMax[axis] + window->SizeFull[axis] - decoration_size[axis];
                scroll_target = calc_scroll_edge_snap( scroll_target, snap_min, snap_max, window->ScrollTargetEdgeSnapDist[axis], center_ratio );
            }
            scroll[axis] = scroll_target - center_ratio * ( window->SizeFull[axis] - decoration_size[axis] );
        }
        scroll[axis] = IM_ROUND( ImMax( scroll[axis], 0.0f ) );
        if ( !window->Collapsed && !window->SkipItems )
            scroll[axis] = ImMin( scroll[axis], window->ScrollMax[axis] );
    }
    return scroll;
}

static float calc_scroll_edge_snap( float target, float snap_min, float snap_max, float snap_threshold, float center_ratio )
{
    if ( target <= snap_min + snap_threshold )
        return ImLerp( snap_min, target, center_ratio );
    if ( target >= snap_max - snap_threshold )
        return ImLerp( target, snap_max, center_ratio );
    return target;
}
