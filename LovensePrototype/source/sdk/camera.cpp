#include "camera.h"

namespace rbx {
    math::matrix3 camera::get_rotation( void ) const noexcept {
		const auto address = get_address();

		if ( !address )
			return math::matrix3();

		return memory.read<math::matrix3>( address + Offsets::camera::Rotation );
    }

    math::vec3 camera::get_position( void ) const noexcept {
		const auto address = get_address();

		if ( !address )
			return math::vec3();

		return memory.read<math::vec3>( address + Offsets::camera::Position );
    }

    math::vec3 camera::get_focus_position( void ) const noexcept {
		const auto address = get_address();

		if ( !address )
			return math::vec3();

		return memory.read<math::vec3>( address + Offsets::camera::FocusPosition );
    }

	void camera::set_rotation( math::matrix3& value ) const noexcept {
		const auto address = get_address();

		if ( !address )
			return;

		memory.write<math::matrix3>( address + Offsets::camera::Rotation, value );
	}

	void camera::set_position( math::vec3& value ) const noexcept {
		const auto address = get_address();

		if ( !address )
			return;

		memory.write<math::vec3>( address + Offsets::camera::Position, value );
	}
}