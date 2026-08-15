#include "cache.h"

#include <print>

#include "../sdk/utilities.h"
#include "../game/game.h"

namespace cache {
	rbx::player GetLocalPlayer() {
		const auto& player = game->player_list.front();

		if ( player.is_valid() )
			return player;
		else
			return rbx::player();
	}

	void UpdatePlayerInfo( rbx::player& player ) {
		if ( !player.get_address() )
			return;

		player.humanoid = player.character.find_child_by_class<rbx::humanoid>( "Humanoid" );
		player.username = player.get_name();
	}

	void UpdatePlayerData( rbx::player& player ) {
		if ( !player.character.is_valid() )
			return;

		player.body_parts.update( player.character );

		if ( !player.is_valid() )
			return;

		player.humanoid = player.character.find_child_by_class<rbx::humanoid>( "Humanoid" );

		if ( player.body_parts.has( "HumanoidRootPart" ) )
			player.distance = ( game->camera_obj.get_position() - player.body_parts.get( "HumanoidRootPart" ).get_primitive().get_position() ).length();

		player.equipped_tool = player.character.find_child_by_class<rbx::instance>( "Tool" );
	}

	void UpdatePlayer( rbx::player& player ) {
		rbx::model_instance new_character = player.get_model_instance();

		if ( !new_character.is_valid() )
			return;

		if ( !rbx::utils::update_instance( player.character, std::move( new_character ) ) )
			return;

		if ( !player.character.get_address() )
			return;

		UpdatePlayerInfo( player );
	}

	void start( void ) {
		game->thread_pool.enqueue( [ & ]() {
			while ( true ) {
				Sleep( 200 );

				std::vector<rbx::instance> children;
				game->players.get_children_into( children );

				if ( children.empty() )
					continue;

				std::vector<rbx::player> curr;

				for ( auto& instance : children ) {
					const auto address = instance.get_address();

					if ( !address )
						continue;

					rbx::player player;
					player.address = address;

					curr.push_back( std::move( player ) );
				}

				game->player_list.update_and_init( curr, []( rbx::player& player ) {
					cache::UpdatePlayer( player );
				} );

				game->local_player = cache::GetLocalPlayer();
			}
		} );

		game->thread_pool.enqueue( [ & ]() {
			static int print_counter = 0;
			while ( true ) {
				Sleep( 33 );

				game->player_list.update_data( []( rbx::player& player ) {
					cache::UpdatePlayer( player );
					cache::UpdatePlayerData( player );
				} );

				// Print local player health every ~1 second
				if ( ++print_counter >= 30 ) {
					print_counter = 0;
					if ( game->local_player.is_valid() && game->local_player.humanoid.get_address() != 0 ) {
						float health = game->local_player.humanoid.get_health();
						float max_health = game->local_player.humanoid.get_max_health();
						std::println( "[*] Local Player Health: {:.2f} / {:.2f}", health, max_health );
					}
				}
			}
		} );
	}
}