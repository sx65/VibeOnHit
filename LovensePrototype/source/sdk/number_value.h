#pragma once

#include "instance.h"

namespace rbx {
	class number_value : public rbx::instance {
	public:
		double get_value( void ) const noexcept;
	};
}