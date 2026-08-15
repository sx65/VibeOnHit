#include "../features.h"

#include <imgui/imgui.h>

#include "../../game/game.h"

#include "../../config/config.h"

#include "../../renderer/bounds.h"
#include "../../renderer/renderer.h"

namespace features {
	namespace visuals {
		void esp( void ) {
			auto players = *game->player_list.get_snapshot();

			std::sort( players.begin(), players.end(),
				[]( const rbx::player& a, const rbx::player& b ) {
					return a.distance > b.distance;
				} );

			for ( rbx::player& player : players ) {
				if ( !player.is_valid() ) continue;
				if ( player.get_address() == game->local_player.get_address() ) continue;
				if ( player.humanoid.get_address() == 0 ) continue;

				const float max_distance = config.esp.max_distance;

				if ( player.distance > max_distance || player.distance < 0 ) continue;

				float health = player.humanoid.get_health();
				float max_health = player.humanoid.get_max_health();

				if ( health <= 0 )
					continue;

				const bounds::bounds bounds = bounds::get_bounds( player, bounds::bounds_type::STATIC );

				const ImVec2 min = ImVec2( bounds.min.x, bounds.min.y );
				const ImVec2 max = ImVec2( bounds.max.x, bounds.max.y );

				if ( min.x == -99.f && min.y == -99.f && max.x == -99.f && max.y == -99.f ) continue;

				const float fade_percentage = 0.2f;
				const float fade_start = max_distance * ( 1.0f - fade_percentage );

				float alpha = 1.0f;
				if ( player.distance >= fade_start ) {
					alpha = 1.0f - ( ( player.distance - fade_start ) / ( max_distance - fade_start ) );
					alpha = std::clamp( alpha, 0.0f, 1.0f );
				}

				auto accent_col = config.esp.color;
				accent_col[ 3 ] = alpha;

				float base_font_size = 12.0f;
				float distance_multiplier = 0.85f + ( player.distance / 1000.0f );
				distance_multiplier = std::clamp( distance_multiplier, 0.9f, 1.0f );

				float final_font_size = base_font_size * distance_multiplier;

				const bool is_tool_equipped = config.esp.tool ? player.equipped_tool.is_valid() : false;

				renderer::rectangle( min, max, accent_col );
				renderer::health_bar( min, max, health, max_health, accent_col );
				renderer::name( min, max, player.username.c_str(), final_font_size, accent_col );
				renderer::tool( min, max, player.equipped_tool.get_name().c_str(), final_font_size, is_tool_equipped, accent_col );
				renderer::distance( min, max, player.distance, final_font_size, is_tool_equipped, accent_col );
			}
		}
	}
}