#include "../framework.h"
#include <map>

using namespace ImGui;

bool framework::slider_scalar( const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* suffix, const char* format, ImGuiSliderFlags flags )
{
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    PushFont( fonts::main_12px );

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID( label );
    const float w = ( GetContentRegionMax( ).x - 4.f );
    const float y_size = 17.f;
    const ImVec2 pos = window->DC.CursorPos + ImVec2( 1.f, 0.f );

    const ImVec2 label_size = CalcTextSize( label, NULL, true );
    const ImRect frame_bb( pos + ImVec2( 0.f, 15.f ), pos + ImVec2( w, label_size.y + y_size ) );
    const ImRect total_bb( frame_bb.Min, frame_bb.Max + ImVec2( style.ItemInnerSpacing.x + label_size.x, 0.0f ) );

    PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0.f, 0.f ) );
    ItemSize( total_bb, style.FramePadding.y );
    PopStyleVar( );

    if ( !ItemAdd( total_bb, id, &frame_bb, 0 ) )
    {
        PopFont( );
        return false;
    }

    if ( format == NULL )
        format = DataTypeGetInfo( data_type )->PrintFmt;

    const bool hovered = ItemHoverable( frame_bb, id, g.LastItemData.ItemFlags );

    const bool clicked = hovered && IsMouseClicked( 0, ImGuiInputFlags_None, id );
    const bool make_active = ( clicked || g.NavActivateId == id );
    if ( make_active && clicked )
        SetKeyOwner( ImGuiKey_MouseLeft, id );

    if ( make_active )
        memcpy( &g.ActiveIdValueOnActivation, p_data, DataTypeGetInfo( data_type )->Size );

    if ( make_active )
    {
        SetActiveID( id, window );
        SetFocusID( id, window );
        FocusWindow( window );
        g.ActiveIdUsingNavDirMask |= ( 1 << ImGuiDir_Left ) | ( 1 << ImGuiDir_Right );
    }

    RenderNavCursor( frame_bb, id );
    window->DrawList->AddRectFilled( frame_bb.Min, frame_bb.Max, colors::title_bar );

    float normalized_value = 0.0f;
    if ( p_min != nullptr && p_max != nullptr )
    {
        if ( data_type == ImGuiDataType_Float || data_type == ImGuiDataType_Double )
        {
            double v, mn, mx;
            if ( data_type == ImGuiDataType_Float )
            {
                v = *( const float* )p_data;
                mn = *( const float* )p_min;
                mx = *( const float* )p_max;
            }
            else
            {
                v = *( const double* )p_data;
                mn = *( const double* )p_min;
                mx = *( const double* )p_max;
            }
            if ( mx != mn )
                normalized_value = static_cast< float >( ( v - mn ) / ( mx - mn ) );
        }
        else
        {
            ImS64 v = 0, mn = 0, mx = 0;
            switch ( data_type )
            {
            case ImGuiDataType_S8: v = *( const ImS8* )p_data; mn = *( const ImS8* )p_min; mx = *( const ImS8* )p_max; break;
            case ImGuiDataType_U8: v = *( const ImU8* )p_data; mn = *( const ImU8* )p_min; mx = *( const ImU8* )p_max; break;
            case ImGuiDataType_S16: v = *( const ImS16* )p_data; mn = *( const ImS16* )p_min; mx = *( const ImS16* )p_max; break;
            case ImGuiDataType_U16: v = *( const ImU16* )p_data; mn = *( const ImU16* )p_min; mx = *( const ImU16* )p_max; break;
            case ImGuiDataType_S32: v = *( const ImS32* )p_data; mn = *( const ImS32* )p_min; mx = *( const ImS32* )p_max; break;
            case ImGuiDataType_U32: v = *( const ImU32* )p_data; mn = *( const ImU32* )p_min; mx = *( const ImU32* )p_max; break;
            case ImGuiDataType_S64: v = *( const ImS64* )p_data; mn = *( const ImS64* )p_min; mx = *( const ImS64* )p_max; break;
            case ImGuiDataType_U64: v = ( ImS64 ) * ( const ImU64* )p_data; mn = ( ImS64 ) * ( const ImU64* )p_min; mx = ( ImS64 ) * ( const ImU64* )p_max; break;
            default: IM_ASSERT( 0 ); break;
            }
            if ( mx != mn )
              normalized_value = static_cast< float >( static_cast< double >( v - mn ) / ( mx - mn ) );
        }
        normalized_value = ImClamp( normalized_value, 0.0f, 1.0f );
    }

    ImRect grab_bb;
    const bool value_changed = SliderBehavior( ImRect( frame_bb.Min - ImVec2( 8.f, 0.f ), frame_bb.Max + ImVec2( 7.f, 0.f ) ), id, data_type, p_data, p_min, p_max, format, flags, &grab_bb );
    if ( value_changed )
        MarkItemEdited( id );

    float grab_center_x = grab_bb.Min.x + ( grab_bb.Max.x - grab_bb.Min.x ) * 0.5f;
    ImVec2 fill_max = ImVec2( grab_center_x, frame_bb.Max.y );

    window->DrawList->AddRectFilled( frame_bb.Min, fill_max, colors::accent );
    window->DrawList->AddRect( frame_bb.Min, frame_bb.Max, colors::border );

    char value_buf[64];
    int value_len = DataTypeFormatString( value_buf, IM_ARRAYSIZE( value_buf ), data_type, p_data, format );
    if ( suffix && suffix[0] != '\0' )
        strncat( value_buf, suffix, IM_ARRAYSIZE( value_buf ) - strlen( value_buf ) - 1 );
    const char* value_buf_end = value_buf + strlen( value_buf );

    if ( g.LogEnabled )
        LogSetNextTextDecoration( "{", "}" );

    PushStyleColor( ImGuiCol_Text, hovered ? colors::text_hovered : colors::text );
    RenderTextClipped( pos - ImVec2( 0.f, 5.f ), pos + ImVec2( w, y_size - 5.f ), value_buf, value_buf_end, NULL, ImVec2( 1.f, 0.5f ) );
    RenderText( ImVec2( pos.x, pos.y - 3.f ), label );
    PopStyleColor( );

    IMGUI_TEST_ENGINE_ITEM_INFO( id, label, g.LastItemData.StatusFlags | ( temp_input_allowed ? ImGuiItemStatusFlags_Inputable : 0 ) );

    SetCursorPosY( GetCursorPosY( ) + y_size + 6.f );

    PopFont( );

    return value_changed;
}

bool framework::slider_float( const char* label, float* v, float v_min, float v_max, const char* suffix, const char* format, ImGuiSliderFlags flags )
{
    return slider_scalar( label, ImGuiDataType_Float, v, &v_min, &v_max, suffix, format, flags );
}

bool framework::slider_int( const char* label, int* v, int v_min, int v_max, const char* suffix, const char* format, ImGuiSliderFlags flags )
{
  return slider_scalar( label, ImGuiDataType_S32, v, &v_min, &v_max, suffix, format, flags );
}
