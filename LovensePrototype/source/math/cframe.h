#pragma once

#include "matrix.h"
#include "vector.h"

namespace math {
	struct CFrame final {
		matrix3 rotation;
		vec3 translation;
	};
}