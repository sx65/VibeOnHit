#pragma once

#include "instance.h"

#include "../math/vector.h"

namespace rbx {
	class vector3_value : public rbx::instance {
	public:
		math::vec3 get_value( void ) const noexcept;
	};
}