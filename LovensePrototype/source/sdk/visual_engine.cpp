#include "visual_engine.h"

#include <memory/memory.h>

#include "offsets.h"
#include "../game/game.h"

namespace rbx {
    visual_engine visual_engine::get( void ) {
		const auto base_address = memory.get_base_addr();

		if ( !base_address )
			return rbx::visual_engine();

		return memory.read<rbx::visual_engine>( base_address + Offsets::visual_engine::Pointer );
    }

	math::matrix4 visual_engine::get_view_matrix( void ) {
		const auto address = get_address();

		if ( !address )
			return math::matrix4();

		return memory.read<math::matrix4>( address + Offsets::visual_engine::view_matrix );
	}

	math::vec2 visual_engine::get_dimensions( void ) {
		const auto address = get_address();

		if ( !address )
			return math::vec2();

		return memory.read<math::vec2>( address + Offsets::visual_engine::dimensions );
	}

	math::vec2 visual_engine::w2s( const math::vec3& position ) const noexcept {
		const math::Vector4 comp = game->view_matrix * position;

		if ( comp.w < 0.1f ) return { -1.f, -1.f };

		const float width = game->dimensions.x, height = game->dimensions.y;

		const float inv_w = 1.0f / comp.w;

		const float half_width = width * 0.5f;
		const float half_height = height * 0.5f;

		const float screen_x = half_width + ( comp.x * inv_w * half_width );
		const float screen_y = half_height - ( comp.y * inv_w * half_height );

		if ( screen_x < 0 || screen_x >= width || screen_y < 0 || screen_y >= height ) return { -1.f, -1.f };

		return { screen_x, screen_y };
	}
}