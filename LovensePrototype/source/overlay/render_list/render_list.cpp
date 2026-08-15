#include "render_list.h"

namespace render {
    void rect::render( ImDrawList* draw_list ) {
        filled ? draw_list->AddRectFilled( p1, p2, col ) : draw_list->AddRect( p1, p2, col, 0.0f, 0, thickness );
    }

    void glowing_rect::render( ImDrawList* draw_list ) {
        ImVec4 val = col.Value;
        auto col = ImColor( val.x, val.y, val.z, val.w );
        auto glow_col = ImColor( val.x, val.y, val.z, 0.2f );

        for ( int i = 0; i < 360; i += 45 ) {
            float num = i * ( 3.14159f / 180.0f );

            float dx = cosf( num ) * glow_size;
            float dy = sinf( num ) * glow_size;

            filled ? draw_list->AddRectFilled( ImVec2( p1.x + dx, p1.y + dy ), ImVec2( p2.x + dx, p2.y + dx ), glow_col ) : draw_list->AddRect( ImVec2( p1.x + dx, p1.y + dy ), ImVec2( p2.x + dx, p2.y + dx ), glow_col );
        }

        filled ? draw_list->AddRectFilled( p1, p2, col ) : draw_list->AddRect( p1, p2, col );
    }

    void line::render( ImDrawList* draw_list ) {
        draw_list->AddLine( p1, p2, col, thickness );
    }

    void text::render( ImDrawList* draw_list ) {
        center 
            ? draw_list->AddText( nullptr, size, ImVec2( p.x - ImGui::CalcTextSize( val.c_str() ).x * 0.5f, p.y ), col, val.c_str() )
            : draw_list->AddText( nullptr, size, ImVec2( p.x, p.y ), col, val.c_str() );
    }

    void circle::render( ImDrawList* draw_list ) {
        draw_list->AddCircle( ImVec2( p.x, p.y ), rad, col, 0, thickness );
    }

    void triangle::render( ImDrawList* draw_list ) {
        filled 
            ? draw_list->AddTriangleFilled( p1, p2, p3, col ) 
            : draw_list->AddTriangle( p1, p2, p3, col, thickness );
    }

    void render_list::clear( void ) {
        std::lock_guard<std::mutex> lk( mutex );

        list.clear();
        temp.clear();
    }

    void render_list::draw( ImDrawList* draw_list ) {
        std::lock_guard<std::mutex> lk( mutex );

        for ( const auto& item : list ) {
            if ( !item ) 
                continue;

            item->render( draw_list );
        }
    }

    void render_list::update( void ) noexcept {
        std::lock_guard<std::mutex> lk( mutex );
        list = std::move( temp );
        temp.clear();
    }
}