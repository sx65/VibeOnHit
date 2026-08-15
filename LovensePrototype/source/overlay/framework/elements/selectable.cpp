#include "../framework.h"

using namespace ImGui;

bool framework::selectable( const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg )
{
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    ImGuiID id = window->GetID( label );
    ImVec2 label_size = CalcTextSize( label, NULL, true );
    ImVec2 size( size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y );
    ImVec2 pos = window->DC.CursorPos;
    pos.y -= 2.f;
    pos.y += window->DC.CurrLineTextBaseOffset;
    ItemSize( size, 0.0f );

    const bool span_all_columns = ( flags & ImGuiSelectableFlags_SpanAllColumns ) != 0;
    const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
    const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
    if ( size_arg.x == 0.0f || ( flags & ImGuiSelectableFlags_SpanAvailWidth ) )
        size.x = ImMax( label_size.x, max_x - min_x );

    ImRect bb( min_x, pos.y, min_x + size.x, pos.y + size.y );
    if ( ( flags & ImGuiSelectableFlags_NoPadWithHalfSpacing ) == 0 )
    {
        const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
        const float spacing_y = style.ItemSpacing.y;
        const float spacing_L = IM_TRUNC( spacing_x * 0.50f );
        const float spacing_U = IM_TRUNC( spacing_y * 0.50f );
        bb.Min.x -= spacing_L;
        bb.Min.y -= spacing_U;
        bb.Max.x += ( spacing_x - spacing_L );
        bb.Max.y += ( spacing_y - spacing_U );
    }

    const bool disabled_item = ( flags & ImGuiSelectableFlags_Disabled ) != 0;
    const ImGuiItemFlags extra_item_flags = disabled_item ? ( ImGuiItemFlags )ImGuiItemFlags_Disabled : ImGuiItemFlags_None;
    bool is_visible;
    if ( span_all_columns )
    {
        const float backup_clip_rect_min_x = window->ClipRect.Min.x;
        const float backup_clip_rect_max_x = window->ClipRect.Max.x;
        window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
        window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
        is_visible = ItemAdd( bb, id, NULL, extra_item_flags );
        window->ClipRect.Min.x = backup_clip_rect_min_x;
        window->ClipRect.Max.x = backup_clip_rect_max_x;
    }
    else
        is_visible = ItemAdd( bb, id, NULL, extra_item_flags );

    const bool is_multi_select = ( g.LastItemData.ItemFlags & ImGuiItemFlags_IsMultiSelect ) != 0;
    if ( !is_visible )
        if ( !is_multi_select || !g.BoxSelectState.UnclipMode || !g.BoxSelectState.UnclipRect.Overlaps( bb ) ) // Extra layer of "no logic clip" for box-select support (would be more overhead to add to ItemAdd)
            return false;

    const bool disabled_global = ( g.CurrentItemFlags & ImGuiItemFlags_Disabled ) != 0;
    if ( disabled_item && !disabled_global )
        BeginDisabled( );

    if ( span_all_columns )
    {
        if ( g.CurrentTable )
            TablePushBackgroundChannel( );
        else if ( window->DC.CurrentColumns )
            PushColumnsBackground( );
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasClipRect;
        g.LastItemData.ClipRect = window->ClipRect;
    }

    ImGuiButtonFlags button_flags = 0;
    if ( flags & ImGuiSelectableFlags_NoHoldingActiveID ) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
    if ( flags & ImGuiSelectableFlags_NoSetKeyOwner ) { button_flags |= ImGuiButtonFlags_NoSetKeyOwner; }
    if ( flags & ImGuiSelectableFlags_SelectOnClick ) { button_flags |= ImGuiButtonFlags_PressedOnClick; }
    if ( flags & ImGuiSelectableFlags_SelectOnRelease ) { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
    if ( flags & ImGuiSelectableFlags_AllowDoubleClick ) { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }
    if ( ( flags & ImGuiSelectableFlags_AllowOverlap ) || ( g.LastItemData.ItemFlags & ImGuiItemFlags_AllowOverlap ) ) { button_flags |= ImGuiButtonFlags_AllowOverlap; }

    const bool was_selected = selected;
    if ( is_multi_select )
        MultiSelectItemHeader( id, &selected, &button_flags );

    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held, button_flags );

    if ( is_multi_select )
        MultiSelectItemFooter( id, &selected, &pressed );
    else
        if ( ( flags & ImGuiSelectableFlags_SelectOnNav ) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == g.CurrentFocusScopeId )
            if ( g.NavJustMovedToId == id )
                selected = pressed = true;

    if ( pressed || ( hovered && ( flags & ImGuiSelectableFlags_SetNavIdOnHover ) ) )
        if ( !g.NavHighlightItemUnderNav && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent )
        {
            SetNavID( id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId, WindowRectAbsToRel( window, bb ) ); // (bb == NavRect)
            if ( g.IO.ConfigNavCursorVisibleAuto )
                g.NavCursorVisible = false;
        }

    if ( pressed )
        MarkItemEdited( id );

    if ( selected != was_selected )
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

    if ( span_all_columns )
        if ( g.CurrentTable )
            TablePopBackgroundChannel( );
        else if ( window->DC.CurrentColumns )
            PopColumnsBackground( );

    if ( is_visible )
    {
        PushStyleColor( static_cast< ImGuiCol >( ImGuiCol_Text ), selected ? colors::text_active : ( hovered ? colors::text_hovered : colors::text ) );
        RenderTextClipped( pos, ImVec2( ImMin( pos.x + size.x, window->WorkRect.Max.x ), pos.y + size.y ), label, NULL, &label_size, style.SelectableTextAlign, &bb );
        PopStyleColor( );
    }

    // Automatically close popups
    if ( pressed && ( window->Flags & ImGuiWindowFlags_Popup ) && !( flags & ImGuiSelectableFlags_NoAutoClosePopups ) && ( g.LastItemData.ItemFlags & ImGuiItemFlags_AutoClosePopups ) )
        CloseCurrentPopup( );

    if ( disabled_item && !disabled_global )
        EndDisabled( );

    IMGUI_TEST_ENGINE_ITEM_INFO( id, label, g.LastItemData.StatusFlags );
    return pressed; //-V1020
}
