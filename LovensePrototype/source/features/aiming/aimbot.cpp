#include "../features.h"

#include "../../sdk/player.h"
#include "../../game/game.h"
#include "../../config/config.h"

math::vec2 get_cursor_pos( void ) noexcept {
	POINT mouse;
	GetCursorPos( &mouse );
	ScreenToClient( static_cast< HWND >( memory.get_window_handle() ), &mouse );
	return { static_cast< float >( mouse.x ), static_cast< float >( mouse.y ) };
}

math::vec2 calc_rel_pos( math::vec2& cursor_position, math::vec2& position ) noexcept {
	return { position.x - cursor_position.x, position.y - cursor_position.y };
}

void move_mouse( math::vec2 position ) noexcept {
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.mouseData = 0;
	input.mi.time = 0;
	input.mi.dx = position.x;
	input.mi.dy = position.y;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;

	SendInput( 1, &input, sizeof( input ) );
}

float get_best_fov( const rbx::player& player ) noexcept {
	if ( !player.body_parts.has( "HumanoidRootPart" ) )
		return -99.9f;

	const auto& root_part_pos = player.body_parts.get( "HumanoidRootPart" ).get_primitive().get_position();

	const math::vec2 screen_pos = game->visual_engine.w2s( root_part_pos );
	if ( screen_pos == math::vec2{ -1, -1 } ) 
		return -99.9f;

	POINT mouse;
	GetCursorPos( &mouse );
	ScreenToClient( static_cast< HWND >( memory.get_window_handle() ), &mouse );
	return screen_pos.magnitude( { static_cast< float >( mouse.x ), static_cast< float >( mouse.y ) } );
}

rbx::player get_best_target( void ) noexcept {
	auto& aimbot = config.aimbot;

	auto& fov = aimbot.fov;
	auto& checks = aimbot.checks;

	rbx::player best_player;
	float best_fov = fov.enabled ? fov.value : 9e9;

	const auto& players = *game->player_list.get_snapshot();

	for ( const rbx::player& player : players ) {
		if ( !player.is_valid() || !player.is_alive() || game->local_player.get_address() == player.get_address() ) continue;
		if ( checks.max_distance && player.distance > checks.max_distance_value ) continue;

		if ( float fov = get_best_fov( player ); fov != -99.9f && fov < best_fov ) {
			best_fov = fov;
			best_player = player;
		}
	}

	return best_player;
}

namespace features {
	namespace aiming {
		void aimbot( void ) {
			struct _context {
				bool enabled = false;
				rbx::player target;

				math::vec3 resolved_velocity;
			} ctx;

			while ( true ) {
				if ( !game->enabled || !game->focused || !config.aimbot.enabled ) {
					Sleep( 500 );
					continue;
				}

				if ( !config.aimbot.hotkey.update() ) {
					config.aimbot.curr_target = {};

					ctx.target = {};
					ctx.enabled = false;

					Sleep( 10 );
					continue;
				}

				if ( config.aimbot.sticky_aim && config.aimbot.curr_target.is_valid() && config.aimbot.curr_target.is_alive() )
					ctx.target = config.aimbot.curr_target;
				else
					ctx.target = get_best_target();

				if ( !ctx.target.is_valid() || !ctx.target.is_alive() ) {
					config.aimbot.curr_target = {};

					ctx.target = {};
					ctx.enabled = false;

					continue;
				}

				const auto& part = ctx.target.body_parts.get(
					config.aimbot.bone == 0 ? "Head" :
					config.aimbot.bone == 1 ? "UpperTorso" :
					config.aimbot.bone == 2 ? "HumanoidRootPart" :
					"LowerTorso"
				);

				if ( !part.get_address() ) {
					config.aimbot.curr_target = {};

					ctx.target = {};
					ctx.enabled = false;

					continue;
				}

				const auto& part_primitive = part.get_primitive();

				if ( !part_primitive.get_address() ) {
					config.aimbot.curr_target = {};

					ctx.target = {};
					ctx.enabled = false;

					continue;
				}

				math::vec3 position_3d = part_primitive.get_position();
				math::vec2 position = game->visual_engine.w2s( position_3d );

				if ( position == -1 ) {
					config.aimbot.curr_target = {};

					ctx.target = {};
					ctx.enabled = false;

					continue;
				}

				math::vec2 cursor_position = get_cursor_pos();

				ctx.enabled = true;
				config.aimbot.curr_target = ctx.target;

				move_mouse( calc_rel_pos( cursor_position, position ) );

				Sleep( 1 );
			}
		}
	}
}