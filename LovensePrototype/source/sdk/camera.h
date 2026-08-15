#pragma once

#include "instance.h"

#include "../math/matrix.h"

namespace rbx {
	class camera : public rbx::instance {
	public:
		math::matrix3 get_rotation( void ) const noexcept;
		math::vec3 get_position( void ) const noexcept;
		math::vec3 get_focus_position( void ) const noexcept;

		template<typename T>
		T get_subject( void ) const noexcept {
			const auto address = get_address();

			if ( !address )
				return T();

			return memory.read<T>( address + Offsets::camera::Subject );
		}

		void set_rotation( math::matrix3& value ) const noexcept;
		void set_position( math::vec3& value ) const noexcept;

		template<typename T>
		void set_subject( T& value ) const noexcept {
			const auto address = get_address();

			if ( !address )
				return;

			memory.write<T>( address + Offsets::camera::Subject, value );
		}
	};
}