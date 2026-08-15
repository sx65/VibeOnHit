#pragma once

#include "instance.h"

namespace rbx {
	enum rig_type : int {
		Unknown = -1,
		R6 = 0,
		R15 = 1
	};

	class humanoid : public rbx::instance {
	public:
		rig_type get_rig_type( void ) const noexcept;

		float get_health( void ) const noexcept;
		float get_max_health( void ) const noexcept;
	};
}