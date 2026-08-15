#include "features.h"

#include "../game/game.h"
#include "../overlay/overlay.h"
#include "../config/config.h"

namespace features {
	void start( void ) noexcept {
		game->thread_pool.enqueue([] {
			while ( true ) {
				if ( !game->enabled || !config.esp.enabled ) {
					overlay.render_list.clear();

					Sleep( 500 );
					continue;
				}

				features::visuals::esp();

				overlay.render_list.update();
			}
		} );

		game->thread_pool.enqueue( [] {
			features::aiming::aimbot();
		} );
	}
}