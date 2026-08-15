#include "primitive.h"

namespace rbx {
    math::vec3 primitive::get_position( void ) const noexcept {
		const auto address = get_address();

		if ( !address )
			return math::vec3();

		return memory.read<math::vec3>( address + Offsets::primitive::Position );
    }

	math::vec3 primitive::get_size( void ) const noexcept {
		const auto address = get_address();

		if ( !address )
			return math::vec3();

		return memory.read<math::vec3>( address + Offsets::primitive::Size );
	}

	void primitive::lerp_position( const math::vec3& value, const float duration ) noexcept {
		const auto address = get_address();
		if ( !address )
			return;

		const auto current_pos = memory.read<math::vec3>( address + Offsets::primitive::Position );

		const float frame_time = 1.0f / 60.0f;
		const int total_steps = static_cast< int >( duration / frame_time );

		if ( total_steps <= 0 ) {
			set_position( value );
			return;
		}

		for ( int step = 0; step <= total_steps; ++step ) {
			const float t = static_cast< float >( step ) / static_cast< float >( total_steps );

			const math::vec3 lerped_pos = {
				current_pos.x + t * ( value.x - current_pos.x ),
				current_pos.y + t * ( value.y - current_pos.y ),
				current_pos.z + t * ( value.z - current_pos.z )
			};

			memory.write<math::vec3>( address + Offsets::primitive::Position, lerped_pos );

			Sleep( static_cast< int >( frame_time * 1000 ) );
		}

		set_position( value );
	}

	void primitive::set_position( const math::vec3& value ) noexcept {
		const auto address = get_address();

		if ( !address )
			return;

		memory.write<math::vec3>( address + Offsets::primitive::Position, value );
	}

	void primitive::set_size( const math::vec3& value ) noexcept {
		const auto address = get_address();

		if ( !address )
			return;

		memory.write<math::vec3>( address + Offsets::primitive::Size, value );
	}
}