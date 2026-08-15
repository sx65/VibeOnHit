#pragma once

#include "instance.h"

namespace rbx {
	class int_value : public rbx::instance {
	public:
		float get_value( void ) const noexcept;
	};
}