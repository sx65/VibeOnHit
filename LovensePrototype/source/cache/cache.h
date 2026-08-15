#pragma once

#include "../sdk/player.h"

namespace cache {
	void UpdatePlayerInfo( rbx::player& player );
	void UpdatePlayerData( rbx::player& player );
	void UpdatePlayer( rbx::player& player );

	void start( void );
}