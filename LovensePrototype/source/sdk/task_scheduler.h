#pragma once

#include "base.h"

namespace rbx {
	class task_scheduler : public rbx::base {
	public:
		static rbx::task_scheduler get( void );

		std::uint64_t get_max_fps( void );
	};
}