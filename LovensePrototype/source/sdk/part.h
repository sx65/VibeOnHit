#pragma once

#include "instance.h"
#include "primitive.h"

namespace rbx {
	class part : public rbx::instance {
	public:
		primitive get_primitive( void ) const noexcept;
	};
}