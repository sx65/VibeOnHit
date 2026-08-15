#pragma once

#include "instance.h"

namespace rbx {
	class string_value : public rbx::instance {
	public:
		std::string get_value( void ) const noexcept;
	};
}