#pragma once

#include "instance.h"

namespace rbx {
	class bool_value : public rbx::instance {
	public:
		bool get_value( void ) const noexcept;
	};
}