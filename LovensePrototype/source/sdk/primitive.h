#pragma once

#include "instance.h"

#include "../math/vector.h"

namespace rbx {
	class primitive : public rbx::base {
	public:
		math::vec3 get_position( void ) const noexcept;
		math::vec3 get_size( void ) const noexcept;

		void lerp_position( const math::vec3& value, const float duration = 1.0f ) noexcept;
		void set_position( const math::vec3& value ) noexcept;
		void set_size( const math::vec3& value ) noexcept;
	};
}