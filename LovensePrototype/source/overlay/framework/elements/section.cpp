#include "../framework.h"

using namespace ImGui;

bool framework::begin_section( const char* str_id, const ImVec2& size_arg, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags )
{
    ImGuiID id = GetCurrentWindow( )->GetID( str_id );
    return begin_section_ex( str_id, id, size_arg, child_flags, window_flags );
}

bool framework::begin_section_ex( const char* name, ImGuiID id, const ImVec2& size_arg, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags )
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* parent_window = g.CurrentWindow;
    IM_ASSERT( id != 0 );

    const ImGuiChildFlags ImGuiChildFlags_SupportedMask_ = ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_ResizeX | ImGuiChildFlags_ResizeY | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle | ImGuiChildFlags_NavFlattened;
    IM_UNUSED( ImGuiChildFlags_SupportedMask_ );
    IM_ASSERT( ( child_flags & ~ImGuiChildFlags_SupportedMask_ ) == 0 && "Illegal ImGuiChildFlags value. Did you pass ImGuiWindowFlags values instead of ImGuiChildFlags?" );
    IM_ASSERT( ( window_flags & ImGuiWindowFlags_AlwaysAutoResize ) == 0 && "Cannot specify ImGuiWindowFlags_AlwaysAutoResize for BeginChild(). Use ImGuiChildFlags_AlwaysAutoResize!" );
    if ( child_flags & ImGuiChildFlags_AlwaysAutoResize )
    {
        IM_ASSERT( ( child_flags & ( ImGuiChildFlags_ResizeX | ImGuiChildFlags_ResizeY ) ) == 0 && "Cannot use ImGuiChildFlags_ResizeX or ImGuiChildFlags_ResizeY with ImGuiChildFlags_AlwaysAutoResize!" );
        IM_ASSERT( ( child_flags & ( ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY ) ) != 0 && "Must use ImGuiChildFlags_AutoResizeX or ImGuiChildFlags_AutoResizeY with ImGuiChildFlags_AlwaysAutoResize!" );
    }
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    if ( window_flags & ImGuiWindowFlags_AlwaysUseWindowPadding )
        child_flags |= ImGuiChildFlags_AlwaysUseWindowPadding;
    if ( window_flags & ImGuiWindowFlags_NavFlattened )
        child_flags |= ImGuiChildFlags_NavFlattened;
#endif
    if ( child_flags & ImGuiChildFlags_AutoResizeX )
        child_flags &= ~ImGuiChildFlags_ResizeX;
    if ( child_flags & ImGuiChildFlags_AutoResizeY )
        child_flags &= ~ImGuiChildFlags_ResizeY;

    window_flags |= ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;
    window_flags |= ( parent_window->Flags & ImGuiWindowFlags_NoMove ); // Inherit the NoMove flag
    if ( child_flags & ( ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize ) )
        window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
    if ( ( child_flags & ( ImGuiChildFlags_ResizeX | ImGuiChildFlags_ResizeY ) ) == 0 )
        window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

    if ( child_flags & ImGuiChildFlags_FrameStyle )
    {
        PushStyleColor( ImGuiCol_ChildBg, g.Style.Colors[ImGuiCol_FrameBg] );
        PushStyleVar( ImGuiStyleVar_ChildRounding, g.Style.FrameRounding );
        PushStyleVar( ImGuiStyleVar_ChildBorderSize, g.Style.FrameBorderSize );
        PushStyleVar( ImGuiStyleVar_WindowPadding, g.Style.FramePadding );
        child_flags |= ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysUseWindowPadding;
        window_flags |= ImGuiWindowFlags_NoMove;
    }

    const ImVec2 size_avail = GetContentRegionAvail( );
    const ImVec2 size_default( ( child_flags & ImGuiChildFlags_AutoResizeX ) ? 0.0f : size_avail.x, ( child_flags & ImGuiChildFlags_AutoResizeY ) ? 0.0f : size_avail.y );
    ImVec2 size = CalcItemSize( size_arg, size_default.x, size_default.y );

    if ( ( g.NextWindowData.HasFlags & ImGuiNextWindowDataFlags_HasSize ) != 0 && ( g.NextWindowData.SizeCond & ImGuiCond_Always ) != 0 )
    {
        if ( g.NextWindowData.SizeVal.x > 0.0f )
        {
            size.x = g.NextWindowData.SizeVal.x;
            child_flags &= ~ImGuiChildFlags_ResizeX;
        }
        if ( g.NextWindowData.SizeVal.y > 0.0f )
        {
            size.y = g.NextWindowData.SizeVal.y;
            child_flags &= ~ImGuiChildFlags_ResizeY;
        }
    }
    SetNextWindowSize( size );

    if ( g.NextWindowData.HasFlags & ImGuiNextWindowDataFlags_HasChildFlags )
        g.NextWindowData.ChildFlags |= child_flags;
    else
        g.NextWindowData.ChildFlags = child_flags;
    g.NextWindowData.HasFlags |= ImGuiNextWindowDataFlags_HasChildFlags;

    const char* temp_window_name;
    if ( name )
        ImFormatStringToTempBuffer( &temp_window_name, NULL, "%s/%s_%08X", parent_window->Name, name, id );
    else
        ImFormatStringToTempBuffer( &temp_window_name, NULL, "%s/%08X", parent_window->Name, id );

    const float backup_border_size = g.Style.ChildBorderSize;
    if ( ( child_flags & ImGuiChildFlags_Borders ) == 0 )
        g.Style.ChildBorderSize = 0.0f;

    const bool ret = Begin( temp_window_name, NULL, window_flags );

    g.Style.ChildBorderSize = backup_border_size;
    if ( child_flags & ImGuiChildFlags_FrameStyle )
    {
        PopStyleVar( 3 );
        PopStyleColor( );
    }

    ImGuiWindow* child_window = g.CurrentWindow;
    child_window->ChildId = id;

    if ( child_window->BeginCount == 1 )
        parent_window->DC.CursorPos = child_window->Pos;

    const ImGuiID temp_id_for_activation = ImHashStr( "##Child", 0, id );
    if ( g.ActiveId == temp_id_for_activation )
        ClearActiveID( );
    if ( g.NavActivateId == id && !( child_flags & ImGuiChildFlags_NavFlattened ) && ( child_window->DC.NavLayersActiveMask != 0 || child_window->DC.NavWindowHasScrollY ) )
    {
        FocusWindow( child_window );
        NavInitWindow( child_window, false );
        SetActiveID( temp_id_for_activation, child_window );
        g.ActiveIdSource = g.NavInputSource;
    }

    constexpr float section_bar_height = 22.f;

    PushFont( fonts::main_12px );
    // section background
    child_window->DrawList->AddRectFilled( child_window->Pos, ImVec2( child_window->Pos.x + size_arg.x, child_window->Pos.y + size_arg.y ), colors::child_bg );

    // section title
    child_window->DrawList->AddRectFilled( child_window->Pos, ImVec2( child_window->Pos.x + size_arg.x, child_window->Pos.y + section_bar_height ), colors::title_bar );
    child_window->DrawList->AddText( child_window->Pos + ImVec2( 7.f, 3.f ), colors::text, name );

    // separator
    child_window->DrawList->AddLine( ImVec2( child_window->Pos.x, child_window->Pos.y + section_bar_height - 0.5f ), ImVec2( child_window->Pos.x + size_arg.x, child_window->Pos.y + section_bar_height - 0.5f ), colors::accent, 2.f );

    // border
    child_window->DrawList->AddRect( child_window->Pos, ImVec2( child_window->Pos.x + size_arg.x, child_window->Pos.y + size_arg.y ), colors::border );
    PopFont( );

    SetCursorPos( ImVec2( 7.f, section_bar_height + 10.f ) );

    char child_name[64] = {};
    ImFormatString( child_name, sizeof( child_name ), "SectionChild%s", name );
    BeginChild( child_name, size_arg - ImVec2( 12.f, section_bar_height + 10.f ), 0, ImGuiWindowFlags_NoScrollbar );
    PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0.f, 5.f ) );

    return ret;
}

void framework::end_section( )
{
    PopStyleVar( );
    EndChild( );

    ImGuiContext& g = *GImGui;
    ImGuiWindow* child_window = g.CurrentWindow;

    const ImGuiID backup_within_end_child_id = g.WithinEndChildID;
    IM_ASSERT( child_window->Flags & ImGuiWindowFlags_ChildWindow );

    g.WithinEndChildID = child_window->ID;
    ImVec2 child_size = child_window->Size;
    End( );
    if ( child_window->BeginCount == 1 )
    {
        ImGuiWindow* parent_window = g.CurrentWindow;
        ImRect bb( parent_window->DC.CursorPos, parent_window->DC.CursorPos + child_size );
        ItemSize( child_size );
        const bool nav_flattened = ( child_window->ChildFlags & ImGuiChildFlags_NavFlattened ) != 0;
        if ( ( child_window->DC.NavLayersActiveMask != 0 || child_window->DC.NavWindowHasScrollY ) && !nav_flattened )
        {
            ItemAdd( bb, child_window->ChildId );
            RenderNavCursor( bb, child_window->ChildId );

            if ( child_window->DC.NavLayersActiveMask == 0 && child_window == g.NavWindow )
                RenderNavCursor( ImRect( bb.Min - ImVec2( 2, 2 ), bb.Max + ImVec2( 2, 2 ) ), g.NavId, ImGuiNavRenderCursorFlags_Compact );
        }
        else
        {
            ItemAdd( bb, child_window->ChildId, NULL, ImGuiItemFlags_NoNav );

            if ( nav_flattened )
                parent_window->DC.NavLayersActiveMaskNext |= child_window->DC.NavLayersActiveMaskNext;
        }
        if ( g.HoveredWindow == child_window )
            g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;
        child_window->DC.ChildItemStatusFlags = g.LastItemData.StatusFlags;
    }
    else
    {
        SetLastItemData( child_window->ChildId, g.CurrentItemFlags, child_window->DC.ChildItemStatusFlags, child_window->Rect( ) );
    }

    g.WithinEndChildID = backup_within_end_child_id;
    g.LogLinePosY = -FLT_MAX;

    // 1 px padding
    Dummy( ImVec2( 0.f, 1.f ) );
}
