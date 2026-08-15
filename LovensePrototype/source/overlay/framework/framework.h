#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <windows.h>
#include <vector>

namespace framework {
    struct hotkey_t {
        int virtual_key;
        bool value = false;

        enum type {
            hold,
            toggle,
            always_on,
            force_disable
        } mode = hold;

        bool update() noexcept {
            switch ( mode ) {
            case hold:
                value = GetAsyncKeyState( virtual_key );
                break;

            case toggle:
                if ( GetAsyncKeyState( virtual_key ) & 0x01 )
                    value = !value;
                break;

            case always_on:
                value = true;
                break;

            default:
                value = false;
                break;
            }

            return value;
        }
    };

    const char* const key_names[] =
    {
        "UNK",
        "MOUSE1",
        "MOUSE2",
        "CANCEL",
        "MOUSE3",
        "MOUSE4",
        "MOUSE5",
        "UNK",
        "BACK",
        "TAB",
        "UNK",
        "UNK",
        "CLEAR",
        "RETURN",
        "UNK",
        "UNK",
        "SHIFT",
        "CONTROL",
        "MENU",
        "PAUSE",
        "CAPITAL",
        "KANA",
        "UNK",
        "JUNJA",
        "FINAL",
        "KANJI",
        "UNK",
        "ESCAPE",
        "CONVERT",
        "NONCONVERT",
        "ACCEPT",
        "MODECHANGE",
        "SPACE",
        "PRIOR",
        "NEXT",
        "END",
        "HOME",
        "LEFT",
        "UP",
        "RIGHT",
        "DOWN",
        "SELECT",
        "PRINT",
        "EXECUTE",
        "SNAPSHOT",
        "INSERT",
        "DELETE",
        "HELP",
        "0",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "UNK",
        "UNK",
        "UNK",
        "UNK",
        "UNK",
        "UNK",
        "UNK",
        "A",
        "B",
        "C",
        "D",
        "E",
        "F",
        "G",
        "H",
        "I",
        "J",
        "K",
        "L",
        "M",
        "N",
        "O",
        "P",
        "Q",
        "R",
        "S",
        "T",
        "U",
        "V",
        "W",
        "X",
        "Y",
        "Z",
        "LWIN",
        "RWIN",
        "APPS",
        "UNK",
        "SLEEP",
        "0",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "MULTIPLY",
        "ADD",
        "SEPARATOR",
        "SUBTRACT",
        "DECIMAL",
        "DIVIDE",
        "F1",
        "F2",
        "F3",
        "F4",
        "F5",
        "F6",
        "F7",
        "F8",
        "F9",
        "F10",
        "F11",
        "F12",
        "F13",
        "F14",
        "F15",
        "F16",
        "F17",
        "F18",
        "F19",
        "F20",
        "F21",
        "F22",
        "F23",
        "F24",
        "UNK",
        "UNK",
        "UNK",
        "UNK",
        "UNK",
        "UNK",
        "UNK",
        "UNK",
        "NUMLOCK",
        "SCROLL",
        "OEM_NEC_EQUAL",
        "OEM_FJ_MASSHOU",
        "OEM_FJ_TOUROKU",
        "OEM_FJ_LOYA",
        "OEM_FJ_ROYA",
        "UNK",
        "UNK",
        "UNK",
        "UNK",
        "UNK",
        "UNK",
        "UNK",
        "UNK",
        "UNK",
        "LSHIFT",
        "RSHIFT",
        "LCONTROL",
        "RCONTROL",
        "LMENU",
        "RMENU"
    };

    namespace colors {
        static ImU32 window = IM_COL32( 69, 44, 46, 255 );
        static ImU32 title_bar = IM_COL32( 47, 28, 30, 255 );
        static ImU32 child_bg = IM_COL32( 53, 32, 35, 255 );

        static ImU32 accent = IM_COL32( 238, 183, 190, 255 );
        static ImU32 border = IM_COL32( 82, 59, 64, 255 );

        static ImU32 text = IM_COL32( 144, 109, 121, 255 );
        static ImU32 text_hovered = IM_COL32( 209, 151, 164, 255 );
        static ImU32 text_active = IM_COL32( 238, 183, 190, 255 );
    }

    namespace fonts {
        extern ImFont* main_13px;
        extern ImFont* main_12px;
        extern ImFont* main_11px;
        extern ImFont* main_10px;
        extern ImFont* main_9px;
    }

    static ImVec2 window_size = ImVec2( 714.f, 405.f );

    void setup();
    void render( const char* name );

    bool begin( const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0, bool custom_title_bar = true );
    void end();

    bool tab( const char* name, bool selected );
    bool sub_tab( const char* name, bool selected );

    bool begin_section( const char* str_id, const ImVec2& size = ImVec2( 0, 0 ), ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0 );
    bool begin_section_ex( const char* name, ImGuiID id, const ImVec2& size_arg, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags );
    void end_section();

    bool checkbox( const char* label, bool* v );
    void hotkey( const char* label, hotkey_t* hotkey );

    bool slider_scalar( const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* suffix, const char* format, ImGuiSliderFlags flags );
    bool slider_float( const char* label, float* v, float v_min, float v_max, const char* suffix = "", const char* format = "%.3f", ImGuiSliderFlags flags = 0 );
    bool slider_int( const char* label, int* v, int v_min, int v_max, const char* suffix = "", const char* format = "%d", ImGuiSliderFlags flags = 0 );

    bool color_edit_4( const char* label, float col[ 4 ], ImGuiColorEditFlags flags = 0 );
    bool color_picker_4( const char* label, float col[ 4 ], ImGuiColorEditFlags flags = 0, const float* ref_col = NULL );

    bool begin_combo( const char* label, const char* preview_value, ImGuiComboFlags flags = 0 );
    void end_combo();

    bool combo( const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1 );
    bool combo( const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1 );
    bool combo( const char* label, int* current_item, const char* ( *getter )( void* user_data, int idx ), void* user_data, int items_count, int popup_max_height_in_items = -1 );

    bool multi_combo( const char* label, bool* selected, const char* const items[], int items_count, int popup_max_height_in_items = -1 );
    bool multi_combo( const char* label, bool* selected, const char* items_separated_by_zeros, int popup_max_height_in_items = -1 );
    bool multi_combo( const char* label, bool* bools[], const char* items_separated_by_zeros, int height_in_items = -1 );
    bool multi_combo( const char* label, bool* selected, const char* ( *getter )( void* user_data, int idx ), void* user_data, int items_count, int popup_max_height_in_items = -1 );

    bool is_popup_open( ImGuiID id, ImGuiPopupFlags popup_flags );
    bool is_popup_open( const char* str_id, ImGuiPopupFlags flags = 0 );
    void open_popup( const char* str_id, ImGuiPopupFlags popup_flags = 0 );
    void open_popup_ex( ImGuiID id, ImGuiPopupFlags popup_flags = ImGuiPopupFlags_None );

    bool begin_popup( const char* str_id, ImGuiWindowFlags flags = 0 );
    bool begin_popup_ex( ImGuiID id, const char* name, ImGuiWindowFlags extra_window_flags );
    void end_popup();

    bool begin_popup_context_item( const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1 );

    bool input_text( const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL );
    bool input_text_ex( const char* label, const char* hint, char* buf, int buf_size, const ImVec2& size_arg, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback = NULL, void* user_data = NULL );

    bool selectable( const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2( 0, 0 ) );

    bool tree_node_ex( const char* label, ImGuiTreeNodeFlags flags = 0 );
    bool tree_node_behavior( ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end = NULL );

    void text( const char* fmt, ... );
    void text_v( const char* fmt, va_list args );
    void text_ex( const char* text, const char* text_end = NULL, ImGuiTextFlags flags = 0 );

    void separator();
    void separator_ex( ImGuiSeparatorFlags flags, float thickness = 1.0f );

    bool begin_list_box( const char* label, const ImVec2& size = ImVec2( 0, 0 ) );
    void end_list_box();

    bool begin_child( const char* str_id, const ImVec2& size = ImVec2( 0, 0 ), ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0 );
    bool begin_child( ImGuiID id, const ImVec2& size = ImVec2( 0, 0 ), ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0 );
    bool begin_child_ex( const char* name, ImGuiID id, const ImVec2& size_arg, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags );
    void end_child();

    bool button( const char* label, const ImVec2& size = ImVec2( 0, 0 ) );
    bool button_ex( const char* label, const ImVec2& size = ImVec2( 0, 0 ), ImGuiButtonFlags flags = 0 );

    void columns( float offset );
}
