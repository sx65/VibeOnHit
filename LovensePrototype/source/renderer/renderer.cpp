#include "renderer.h"

#include "../overlay/settings/settings.h"
#include "../overlay/overlay.h"
#include "../config/config.h"

#include <algorithm>

namespace renderer {
	void name( const ImVec2& min, const ImVec2& max, const char* name, const float scale_factor, const float col[ 4 ] ) {
		if ( !config.esp.names )
			return;

		ImFont* font = variables->font.verdana;

		ImVec2 text_size = font->CalcTextSizeA( scale_factor, FLT_MAX, 0.0f, name );

		const float offset_up = 2.0f;
		ImVec2 text_pos = ImVec2( ( ( min.x + max.x ) * 0.5f ) - ( text_size.x * 0.5f ), min.y - text_size.y - offset_up );

		for ( int x = -1; x <= 1; ++x )
			for ( int y = -1; y <= 1; ++y )
				overlay.render_list.add< render::text >( ImVec2( text_pos.x + x, text_pos.y + y ), scale_factor, ImColor( 0.f, 0.f, 0.f, col[ 3 ] ), name, false );

		overlay.render_list.add< render::text >( text_pos, scale_factor, ImColor( col[ 0 ], col[ 1 ], col[ 2 ], col[ 3 ] ), name, false );
	}

	void tool( const ImVec2& min, const ImVec2& max, const char* tool, const float scale_factor, const bool tool_equipped, const float col[ 4 ] ) {
		if ( !config.esp.tool )
			return;

		ImFont* font = variables->font.verdana;

		ImVec2 text_size = font->CalcTextSizeA( scale_factor, FLT_MAX, 0.0f, tool );

		float offset_up = 2.0f;

		if ( config.esp.health_bar && config.esp.health_bar_pos == 1 )
			offset_up += 3.0f;

		ImVec2 text_pos = ImVec2( ( ( min.x + max.x ) * 0.5f ) - ( text_size.x * 0.5f ), max.y + offset_up );

		for ( int x = -1; x <= 1; ++x )
			for ( int y = -1; y <= 1; ++y )
				overlay.render_list.add< render::text >( ImVec2( text_pos.x + x, text_pos.y + y ), scale_factor, ImColor( 0.f, 0.f, 0.f, col[ 3 ] ), tool, false );

		overlay.render_list.add< render::text >( text_pos, scale_factor, ImColor( col[ 0 ], col[ 1 ], col[ 2 ], col[ 3 ] ), tool, false );
	}

	void distance( const ImVec2& min, const ImVec2& max, float distance, const float scale_factor, const bool tool_equipped, const float col[ 4 ] ) {
		if ( !config.esp.distance )
			return;
		
		ImFont* font = variables->font.verdana;

		char distance_text[ 32 ];
		snprintf( distance_text, sizeof( distance_text ), "%.0fm", distance );

		ImVec2 text_size = font->CalcTextSizeA( scale_factor, FLT_MAX, 0.0f, distance_text );

		float offset_up = 2.0f;
		if ( tool_equipped )
			offset_up = 13.0f;

		if ( config.esp.health_bar && config.esp.health_bar_pos == 1 )
			offset_up += 3.0f;

		ImVec2 text_pos = ImVec2( ( ( min.x + max.x ) * 0.5f ) - ( text_size.x * 0.5f ), max.y + offset_up );

		for ( int x = -1; x <= 1; ++x )
			for ( int y = -1; y <= 1; ++y )
				overlay.render_list.add< render::text >( ImVec2( text_pos.x + x, text_pos.y + y ), scale_factor, ImColor( 0.f, 0.f, 0.f, col[ 3 ] ), distance_text, false );

		overlay.render_list.add< render::text >( text_pos, scale_factor, ImColor( col[ 0 ], col[ 1 ], col[ 2 ], col[ 3 ] ), distance_text, false );
	}

	void rectangle( const ImVec2& min, const ImVec2& max, const float col[ 4 ] ) {
		if ( !config.esp.box )
			return;

		overlay.render_list.add< render::rect >( min, max, ImColor( 0.f, 0.f, 0.f, col[ 3 ] ), false, 2.0f );
		overlay.render_list.add< render::rect >( min, max, ImColor( col[ 0 ], col[ 1 ], col[ 2 ], col[ 3 ] ) );
	}

	void health_bar( const ImVec2& min, const ImVec2& max, float health, float max_health, const float col[ 4 ] ) {
		if ( !config.esp.health_bar )
			return;

		const float bar_width = 2.0f;
		const float bar_offset = 3.0f;

		ImVec2 bar_position, bar_size;
		float health_percent = std::clamp( health / max_health, 0.0f, 1.0f );

		if ( config.esp.health_bar_pos == 0 ) {
			const float bar_height = max.y - min.y;
			bar_position = ImVec2( min.x - bar_offset - bar_width, min.y );
			bar_size = ImVec2( min.x - bar_offset, max.y );

			float current_bar_height = bar_height * health_percent;

			overlay.render_list.add< render::rect >( ImVec2( bar_position.x - 1, bar_position.y - 1 ), ImVec2( bar_size.x + 1, bar_size.y + 1 ), ImColor( 0.f, 0.f, 0.f, col[ 3 ] ), true );
			overlay.render_list.add< render::rect >( bar_position, bar_size, ImColor( 0.0f, 0.0f, 0.0f, col[ 3 ] ), true );

			ImVec2 health_bar_top = ImVec2( bar_position.x, max.y - current_bar_height );
			ImVec2 health_bar_bottom = ImVec2( bar_size.x, max.y );

			overlay.render_list.add< render::rect >( health_bar_top, health_bar_bottom, ImColor( col[ 0 ], col[ 1 ], col[ 2 ], col[ 3 ] ), true );
		} else if ( config.esp.health_bar_pos == 1 ) {
			const float box_width = max.x - min.x;
			bar_position = ImVec2( min.x, max.y + bar_offset );
			bar_size = ImVec2( max.x, max.y + bar_offset + bar_width );

			float current_bar_width = box_width * health_percent;

			overlay.render_list.add< render::rect >( ImVec2( bar_position.x - 1, bar_position.y - 1 ), ImVec2( bar_size.x + 1, bar_size.y + 1 ), ImColor( 0.f, 0.f, 0.f, col[ 3 ] ), true );
			overlay.render_list.add< render::rect >( bar_position, bar_size, ImColor( 0.0f, 0.0f, 0.0f, col[ 3 ] ), true );

			ImVec2 health_bar_left = ImVec2( bar_position.x, bar_position.y );
			ImVec2 health_bar_right = ImVec2( min.x + current_bar_width, bar_size.y );

			overlay.render_list.add< render::rect >( health_bar_left, health_bar_right, ImColor( col[ 0 ], col[ 1 ], col[ 2 ], col[ 3 ] ), true );
		}
	}
}