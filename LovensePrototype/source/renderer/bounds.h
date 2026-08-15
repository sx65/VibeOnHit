#pragma once

#include <vector>

#include "../math/vector.h"
#include "../sdk/player.h"

namespace bounds {
	struct bounds {
		math::vec2 min;
		math::vec2 max;
	};

	enum class bounds_type {
		STATIC = 0,
		Bounding = 1
	};

	std::vector<math::vec3> GetCorners( const math::vec3& position, const math::vec3& size );
	bounds get_bounds( rbx::player& player, bounds_type type );
}