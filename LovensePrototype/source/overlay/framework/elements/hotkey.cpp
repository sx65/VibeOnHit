#include "../framework.h"

using namespace ImGui;

static int imgui_key_to_vk( ImGuiKey key )
{
    switch ( key )
    {
    case ImGuiKey_Tab: return VK_TAB;
    case ImGuiKey_LeftArrow: return VK_LEFT;
    case ImGuiKey_RightArrow: return VK_RIGHT;
    case ImGuiKey_UpArrow: return VK_UP;
    case ImGuiKey_DownArrow: return VK_DOWN;
    case ImGuiKey_PageUp: return VK_PRIOR;
    case ImGuiKey_PageDown: return VK_NEXT;
    case ImGuiKey_Home: return VK_HOME;
    case ImGuiKey_End: return VK_END;
    case ImGuiKey_Insert: return VK_INSERT;
    case ImGuiKey_Delete: return VK_DELETE;
    case ImGuiKey_Backspace: return VK_BACK;
    case ImGuiKey_Space: return VK_SPACE;
    case ImGuiKey_Enter: return VK_RETURN;
    case ImGuiKey_Escape: return VK_ESCAPE;
    case ImGuiKey_LeftCtrl: return VK_LCONTROL;
    case ImGuiKey_LeftShift: return VK_LSHIFT;
    case ImGuiKey_LeftAlt: return VK_LMENU;
    case ImGuiKey_RightCtrl: return VK_RCONTROL;
    case ImGuiKey_RightShift: return VK_RSHIFT;
    case ImGuiKey_RightAlt: return VK_RMENU;
    case ImGuiKey_Menu: return VK_APPS;
    case ImGuiKey_0: return '0';
    case ImGuiKey_1: return '1';
    case ImGuiKey_2: return '2';
    case ImGuiKey_3: return '3';
    case ImGuiKey_4: return '4';
    case ImGuiKey_5: return '5';
    case ImGuiKey_6: return '6';
    case ImGuiKey_7: return '7';
    case ImGuiKey_8: return '8';
    case ImGuiKey_9: return '9';
    case ImGuiKey_A: return 'A';
    case ImGuiKey_B: return 'B';
    case ImGuiKey_C: return 'C';
    case ImGuiKey_D: return 'D';
    case ImGuiKey_E: return 'E';
    case ImGuiKey_F: return 'F';
    case ImGuiKey_G: return 'G';
    case ImGuiKey_H: return 'H';
    case ImGuiKey_I: return 'I';
    case ImGuiKey_J: return 'J';
    case ImGuiKey_K: return 'K';
    case ImGuiKey_L: return 'L';
    case ImGuiKey_M: return 'M';
    case ImGuiKey_N: return 'N';
    case ImGuiKey_O: return 'O';
    case ImGuiKey_P: return 'P';
    case ImGuiKey_Q: return 'Q';
    case ImGuiKey_R: return 'R';
    case ImGuiKey_S: return 'S';
    case ImGuiKey_T: return 'T';
    case ImGuiKey_U: return 'U';
    case ImGuiKey_V: return 'V';
    case ImGuiKey_W: return 'W';
    case ImGuiKey_X: return 'X';
    case ImGuiKey_Y: return 'Y';
    case ImGuiKey_Z: return 'Z';
    case ImGuiKey_F1: return VK_F1;
    case ImGuiKey_F2: return VK_F2;
    case ImGuiKey_F3: return VK_F3;
    case ImGuiKey_F4: return VK_F4;
    case ImGuiKey_F5: return VK_F5;
    case ImGuiKey_F6: return VK_F6;
    case ImGuiKey_F7: return VK_F7;
    case ImGuiKey_F8: return VK_F8;
    case ImGuiKey_F9: return VK_F9;
    case ImGuiKey_F10: return VK_F10;
    case ImGuiKey_F11: return VK_F11;
    case ImGuiKey_F12: return VK_F12;
    case ImGuiKey_F13: return VK_F13;
    case ImGuiKey_F14: return VK_F14;
    case ImGuiKey_F15: return VK_F15;
    case ImGuiKey_F16: return VK_F16;
    case ImGuiKey_F17: return VK_F17;
    case ImGuiKey_F18: return VK_F18;
    case ImGuiKey_F19: return VK_F19;
    case ImGuiKey_F20: return VK_F20;
    case ImGuiKey_F21: return VK_F21;
    case ImGuiKey_F22: return VK_F22;
    case ImGuiKey_F23: return VK_F23;
    case ImGuiKey_F24: return VK_F24;
    case ImGuiKey_Apostrophe: return VK_OEM_7;
    case ImGuiKey_Comma: return VK_OEM_COMMA;
    case ImGuiKey_Minus: return VK_OEM_MINUS;
    case ImGuiKey_Period: return VK_OEM_PERIOD;
    case ImGuiKey_Slash: return VK_OEM_2;
    case ImGuiKey_Semicolon: return VK_OEM_1;
    case ImGuiKey_Equal: return VK_OEM_PLUS;
    case ImGuiKey_LeftBracket: return VK_OEM_4;
    case ImGuiKey_Backslash: return VK_OEM_5;
    case ImGuiKey_RightBracket: return VK_OEM_6;
    case ImGuiKey_GraveAccent: return VK_OEM_3;
    case ImGuiKey_CapsLock: return VK_CAPITAL;
    case ImGuiKey_ScrollLock: return VK_SCROLL;
    case ImGuiKey_NumLock: return VK_NUMLOCK;
    case ImGuiKey_PrintScreen: return VK_SNAPSHOT;
    case ImGuiKey_Pause: return VK_PAUSE;
    case ImGuiKey_Keypad0: return VK_NUMPAD0;
    case ImGuiKey_Keypad1: return VK_NUMPAD1;
    case ImGuiKey_Keypad2: return VK_NUMPAD2;
    case ImGuiKey_Keypad3: return VK_NUMPAD3;
    case ImGuiKey_Keypad4: return VK_NUMPAD4;
    case ImGuiKey_Keypad5: return VK_NUMPAD5;
    case ImGuiKey_Keypad6: return VK_NUMPAD6;
    case ImGuiKey_Keypad7: return VK_NUMPAD7;
    case ImGuiKey_Keypad8: return VK_NUMPAD8;
    case ImGuiKey_Keypad9: return VK_NUMPAD9;
    case ImGuiKey_KeypadDecimal: return VK_DECIMAL;
    case ImGuiKey_KeypadDivide: return VK_DIVIDE;
    case ImGuiKey_KeypadMultiply: return VK_MULTIPLY;
    case ImGuiKey_KeypadSubtract: return VK_SUBTRACT;
    case ImGuiKey_KeypadAdd: return VK_ADD;
    case ImGuiKey_KeypadEnter: return VK_RETURN;
    case ImGuiKey_KeypadEqual: return VK_OEM_PLUS;
    default: return 0;
    }
}

void framework::hotkey( const char* label, hotkey_t* hotkey )
{
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return;

    SameLine( );
    PushFont( fonts::main_12px );

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID( label );

    const float width = GetColumnWidth( );
    const ImVec2 pos = window->DC.CursorPos + ImVec2( 0.f, 7.f );

    char context_name[64] = {};
    ImFormatString( context_name, sizeof( context_name ), "HotKeyContext%s", label );

    char text[64] = {};
    const char* hotkeyText = ( hotkey->virtual_key != 0 && g.ActiveId != id ) ? key_names[hotkey->virtual_key] : ( g.ActiveId == id ) ? "..." : "NONE";
    ImFormatString( text, sizeof( text ), "[%s]", hotkeyText );

    const ImVec2 text_size = CalcTextSize( text, NULL, true ); // 11 pixels
    const ImRect total_bb( ImVec2( pos.x + width - ( text_size.x + 1.f ), pos.y - style.FramePadding.y ), ImVec2( pos.x + width, pos.y + text_size.y ) );

    PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0.f, 0.f ) );
    ItemSize( total_bb );
    PopStyleVar( );

    if ( !ItemAdd( total_bb, id ) )
    {
        PopFont( );
        return;
    }

    const bool hovered = ItemHoverable( total_bb, id, 0 );
    if ( hovered )
        SetHoveredID( id );

    if ( hovered && ( g.IO.MouseClicked[0] || g.IO.MouseDoubleClicked[0] ) )
    {
        if ( g.ActiveId != id )
            hotkey->virtual_key = 0;

        SetActiveID( id, window );
    }

    bool changed = false;
    int key_code = hotkey->virtual_key;
    if ( g.ActiveId == id && !g.ActiveIdIsJustActivated )
    {
        for ( int n = 0; n < IM_ARRAYSIZE( g.IO.MouseDown ); n++ )
        {
            if ( IsMouseClicked( n ) )
            {
                switch ( n )
                {
                case 0:
                    key_code = 1;
                    break;
                case 1:
                    key_code = 2;
                    break;
                case 2:
                    key_code = 4;
                    break;
                case 3:
                    key_code = 5;
                    break;
                case 4:
                    key_code = 6;
                    break;
                }

                changed = true;
                ClearActiveID( );
            }
        }

        if ( !changed )
        {
            for ( int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; ++key )
            {
                const ImGuiKey current_key = static_cast< ImGuiKey >( key );
                if ( IsKeyPressed( current_key ) )
                {
                    if ( current_key == ImGuiKey_Escape )
                    {
                        key_code = 0;
                        changed = true;
                        ClearActiveID( );
                        break;
                    }

                    int vk_code = imgui_key_to_vk( current_key );
                    if ( vk_code != 0 )
                    {
                        key_code = vk_code;
                        changed = true;
                        ClearActiveID( );
                        break;
                    }
                }
            }
        }

        if ( changed )
            hotkey->virtual_key = key_code;
    }

    PushStyleVar( ImGuiStyleVar_WindowMinSize, ImVec2( 80, 65 ) );

    if ( begin_popup_context_item( context_name ) )
    {
        SetWindowPos( ImVec2( total_bb.Min.x, total_bb.Max.y ) );

        if ( selectable( "Hold", hotkey->mode == hotkey_t::hold ) )
            hotkey->mode = hotkey_t::hold;

        if ( selectable( "Toggle", hotkey->mode == hotkey_t::toggle ) )
            hotkey->mode = hotkey_t::toggle;

        if ( selectable( "Always On", hotkey->mode == hotkey_t::always_on ) )
            hotkey->mode = hotkey_t::always_on;

        if ( selectable( "Force Disable", hotkey->mode == hotkey_t::force_disable ) )
            hotkey->mode = hotkey_t::force_disable;

        End( );
    }

    window->DrawList->AddText( total_bb.Min, ( g.ActiveId == id ) ? colors::text_active : ( hovered ? colors::text_hovered : colors::text ), text );
    PopFont( );

    SetCursorPosY( GetCursorPosY( ) - 3.f );

    PopStyleVar( );
}
