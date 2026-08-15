#include "../framework.h"
#include <string>

using namespace ImGui;

static const char* items_array_getter( void* data, int idx );
static const char* items_single_string_getter( void* data, int idx );
static float calc_max_popup_height_from_item_count( int items_count );

bool framework::begin_combo( const char* label, const char* preview_value, ImGuiComboFlags flags )
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow( );

    g.NextWindowData.ClearFlags( );
    if ( window->SkipItems )
        return false;

    PushFont( fonts::main_12px );

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID( label );

    const float w = ( GetContentRegionMax( ).x - 4.f );
    const float y_size = 19.f;
    const ImVec2 pos = window->DC.CursorPos + ImVec2( 1.f, 0.f );

    const ImRect bb( pos, pos + ImVec2( w, y_size + 21.f ) );
    const ImRect rect( pos + ImVec2( 0.f, 17.f ), pos + ImVec2( w, y_size + 17.f ) );

    PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0.f, 0.f ) );
    ItemSize( bb, 0.f );
    PopStyleVar( );

    if ( !ItemAdd( bb, id ) )
    {
        PopFont( );
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held );

    window->DrawList->AddText( pos, colors::text, label );

    PopFont( );

    window->DrawList->AddRectFilled( rect.Min, rect.Max, colors::title_bar );
    window->DrawList->AddRect( rect.Min, rect.Max, colors::border );

    PushFont( fonts::main_11px );
    ImVec2 preview_text_size = CalcTextSize( preview_value );

    if ( pressed )
        open_popup( label, ImGuiWindowFlags_Popup );

    if ( is_popup_open( label, ImGuiWindowFlags_Popup ) )
    {
        ImVec2 popup_pos = ImVec2( bb.Min.x, bb.Max.y );
        SetNextWindowPos( popup_pos );
        SetNextWindowSize( ImVec2( bb.GetWidth( ), 0.0f ) );

        if ( begin_popup( label, ImGuiWindowFlags_Popup ) )
        {
            window->DrawList->AddText( ImVec2( rect.Min.x + 5.f, rect.Min.y + ( y_size - preview_text_size.y ) * 0.5f - 1 ), colors::text, preview_value );
            PopFont( );

            return true;
        }
    }
    else
        window->DrawList->AddText( ImVec2( rect.Min.x + 5.f, rect.Min.y + ( y_size - preview_text_size.y ) * 0.5f - 1 ), hovered ? colors::text_hovered : colors::text, preview_value );

    PopFont( );

    return false;
}

void framework::end_combo( )
{
    EndPopup( );
}

bool framework::combo( const char* label, int* current_item, const char* const items[], int items_count, int height_in_items )
{
    const bool value_changed = combo( label, current_item, items_array_getter, ( void* )items, items_count, height_in_items );
    return value_changed;
}

bool framework::combo( const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items )
{
    int items_count = 0;
    const char* p = items_separated_by_zeros;
    while ( *p )
    {
        p += ImStrlen( p ) + 1;
        items_count++;
    }
    bool value_changed = combo( label, current_item, items_single_string_getter, ( void* )items_separated_by_zeros, items_count, height_in_items );
    return value_changed;
}

bool framework::combo( const char* label, int* current_item, const char* ( *getter )( void* user_data, int idx ), void* user_data, int items_count, int popup_max_height_in_items )
{
    ImGuiContext& g = *GImGui;

    const char* preview_value = NULL;
    if ( *current_item >= 0 && *current_item < items_count )
        preview_value = getter( user_data, *current_item );

    if ( popup_max_height_in_items != -1 && !( g.NextWindowData.HasFlags & ImGuiNextWindowDataFlags_HasSizeConstraint ) )
        SetNextWindowSizeConstraints( ImVec2( 0, 0 ), ImVec2( FLT_MAX, calc_max_popup_height_from_item_count( popup_max_height_in_items ) ) );

    if ( !begin_combo( label, preview_value, ImGuiComboFlags_None ) )
        return false;

    bool value_changed = false;
    ImGuiListClipper clipper;
    clipper.Begin( items_count );
    clipper.IncludeItemByIndex( *current_item );
    while ( clipper.Step( ) )
        for ( int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++ )
        {
            const char* item_text = getter( user_data, i );
            if ( item_text == NULL )
                item_text = "*Unknown item*";

            PushID( i );
            const bool item_selected = ( i == *current_item );
            PushFont( fonts::main_11px );
            if ( selectable( item_text, item_selected ) && *current_item != i )
            {
                value_changed = true;
                *current_item = i;
            }
            PopFont( );
            if ( item_selected )
                SetItemDefaultFocus( );
            PopID( );
        }

    end_combo( );
    if ( value_changed )
        MarkItemEdited( g.LastItemData.ID );

    return value_changed;
}

bool framework::multi_combo( const char* label, bool* selected, const char* const items[], int items_count, int height_in_items )
{
    return multi_combo( label, selected, items_array_getter, ( void* )items, items_count, height_in_items );
}

bool framework::multi_combo( const char* label, bool* selected, const char* items_separated_by_zeros, int height_in_items )
{
    int items_count = 0;
    const char* p = items_separated_by_zeros;
    while ( *p )
    {
        p += ImStrlen( p ) + 1;
        items_count++;
    }
    return multi_combo( label, selected, items_single_string_getter, ( void* )items_separated_by_zeros, items_count, height_in_items );
}

bool framework::multi_combo( const char* label, bool* bools[], const char* items_separated_by_zeros, int height_in_items )
{
    int count = 0;
    const char* p = items_separated_by_zeros;
    while ( *p )
    {
        p += ImStrlen( p ) + 1;
        count++;
    }

    std::vector<unsigned char> temp_selected( count );

    for ( int i = 0; i < count; i++ )
        temp_selected[i] = static_cast< unsigned char >( *bools[i] );

    bool changed = multi_combo( label, reinterpret_cast< bool* >( temp_selected.data( ) ), items_separated_by_zeros, height_in_items );

    if ( changed )
        for ( int i = 0; i < count; i++ )
            *bools[i] = static_cast< bool >( temp_selected[i] );

    return changed;
}

bool framework::multi_combo( const char* label, bool* selected, const char* ( *getter )( void* user_data, int idx ), void* user_data, int items_count, int height_in_items )
{
    ImGuiContext& g = *GImGui;

    std::string preview;
    for ( int i = 0; i < items_count; ++i )
    {
        if ( selected[i] )
        {
            const char* item_text = getter( user_data, i );
            if ( item_text )
            {
                if ( !preview.empty( ) )
                    preview += ", ";
                preview += item_text;
            }
        }
    }
    if ( preview.empty( ) )
        preview = "None";

    if ( height_in_items != -1 && !( g.NextWindowData.HasFlags & ImGuiNextWindowDataFlags_HasSizeConstraint ) )
    {
        float max_height = calc_max_popup_height_from_item_count( height_in_items );
        SetNextWindowSizeConstraints( ImVec2( 0, 0 ), ImVec2( FLT_MAX, max_height ) );
    }

    if ( !begin_combo( label, preview.c_str( ) ) )
        return false;

    bool value_changed = false;
    ImGuiListClipper clipper;
    clipper.Begin( items_count );
    while ( clipper.Step( ) )
    {
        for ( int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i )
        {
            const char* item_text = getter( user_data, i );
            if ( !item_text )
                item_text = "*Unknown item*";

            bool item_selected = selected[i];
            PushFont( fonts::main_11px );
            if ( selectable( item_text, item_selected, ImGuiSelectableFlags_DontClosePopups ) )
            {
                selected[i] = !selected[i];
                value_changed = true;
            }
            PopFont( );
            if ( item_selected )
                SetItemDefaultFocus( );
        }
    }
    end_combo( );

    if ( value_changed )
        MarkItemEdited( g.LastItemData.ID );

    return value_changed;
}

static const char* items_array_getter( void* data, int idx )
{
    const char* const* items = ( const char* const* )data;
    return items[idx];
}

static const char* items_single_string_getter( void* data, int idx )
{
    const char* items_separated_by_zeros = ( const char* )data;
    int items_count = 0;
    const char* p = items_separated_by_zeros;
    while ( *p )
    {
        if ( idx == items_count )
            break;
        p += ImStrlen( p ) + 1;
        items_count++;
    }
    return *p ? p : NULL;
}

static float calc_max_popup_height_from_item_count( int items_count )
{
    ImGuiContext& g = *GImGui;
    if ( items_count <= 0 )
        return FLT_MAX;
    return ( g.FontSize + g.Style.ItemSpacing.y ) * items_count - g.Style.ItemSpacing.y + ( g.Style.WindowPadding.y * 2 );
}
