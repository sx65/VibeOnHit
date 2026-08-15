#include "../framework.h"

using namespace ImGui;

void framework::text( const char* fmt, ... )
{
    va_list args;
    va_start( args, fmt );
    text_v( fmt, args );
    va_end( args );
}

void framework::text_v( const char* fmt, va_list args )
{
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return;

    const char* text, * text_end;
    ImFormatStringToTempBufferV( &text, &text_end, fmt, args );
    text_ex( text, text_end, ImGuiTextFlags_NoWidthForLargeClippedText );
}

void framework::text_ex( const char* text, const char* text_end, ImGuiTextFlags flags )
{
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return;
    ImGuiContext& g = *GImGui;

    if ( text == text_end )
        text = text_end = "";

    const char* text_begin = text;
    if ( text_end == NULL )
        text_end = text + ImStrlen( text ); // FIXME-OPT

    const ImVec2 text_pos( window->DC.CursorPos.x + 4.f, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset );
    const float wrap_pos_x = window->DC.TextWrapPos;
    const bool wrap_enabled = ( wrap_pos_x >= 0.0f );
    if ( text_end - text <= 2000 || wrap_enabled )
    {
        // Common case
        const float wrap_width = wrap_enabled ? CalcWrapWidthForPos( window->DC.CursorPos, wrap_pos_x ) : 0.0f;
        const ImVec2 text_size = CalcTextSize( text_begin, text_end, false, wrap_width );

        ImRect bb( text_pos, text_pos + text_size );
        ItemSize( text_size, 0.0f );
        if ( !ItemAdd( bb, 0 ) )
            return;

        if ( text != text_end )
            window->DrawList->AddText( g.Font, g.FontSize, bb.Min, GetColorU32( ImGuiCol_Text ), text_begin, text_end, wrap_width );
    }
    else
    {
        // Long text!
        // Perform manual coarse clipping to optimize for long multi-line text
        // - From this point we will only compute the width of lines that are visible. Optimization only available when word-wrapping is disabled.
        // - We also don't vertically center the text within the line full height, which is unlikely to matter because we are likely the biggest and only item on the line.
        // - We use memchr(), pay attention that well optimized versions of those str/mem functions are much faster than a casually written loop.
        const char* line = text;
        const float line_height = GetTextLineHeight( );
        ImVec2 text_size( 0, 0 );

        // Lines to skip (can't skip when logging text)
        ImVec2 pos = text_pos;
        if ( !g.LogEnabled )
        {
            int lines_skippable = ( int )( ( window->ClipRect.Min.y - text_pos.y ) / line_height );
            if ( lines_skippable > 0 )
            {
                int lines_skipped = 0;
                while ( line < text_end && lines_skipped < lines_skippable )
                {
                    const char* line_end = ( const char* )ImMemchr( line, '\n', text_end - line );
                    if ( !line_end )
                        line_end = text_end;
                    if ( ( flags & ImGuiTextFlags_NoWidthForLargeClippedText ) == 0 )
                        text_size.x = ImMax( text_size.x, CalcTextSize( line, line_end ).x );
                    line = line_end + 1;
                    lines_skipped++;
                }
                pos.y += lines_skipped * line_height;
            }
        }

        // Lines to render
        if ( line < text_end )
        {
            ImRect line_rect( pos, pos + ImVec2( FLT_MAX, line_height ) );
            while ( line < text_end )
            {
                if ( IsClippedEx( line_rect, 0 ) )
                    break;

                const char* line_end = ( const char* )ImMemchr( line, '\n', text_end - line );
                if ( !line_end )
                    line_end = text_end;
                text_size.x = ImMax( text_size.x, CalcTextSize( line, line_end ).x );
                RenderText( pos, line, line_end, false );
                line = line_end + 1;
                line_rect.Min.y += line_height;
                line_rect.Max.y += line_height;
                pos.y += line_height;
            }

            // Count remaining lines
            int lines_skipped = 0;
            while ( line < text_end )
            {
                const char* line_end = ( const char* )ImMemchr( line, '\n', text_end - line );
                if ( !line_end )
                    line_end = text_end;
                if ( ( flags & ImGuiTextFlags_NoWidthForLargeClippedText ) == 0 )
                    text_size.x = ImMax( text_size.x, CalcTextSize( line, line_end ).x );
                line = line_end + 1;
                lines_skipped++;
            }
            pos.y += lines_skipped * line_height;
        }
        text_size.y = ( pos - text_pos ).y;

        ImRect bb( text_pos, text_pos + text_size );
        ItemSize( text_size, 0.0f );
        ItemAdd( bb, 0 );
    }
}
