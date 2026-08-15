#include "task_scheduler.h"

#include <memory/memory.h>
#include "offsets.h"

namespace rbx {
    task_scheduler task_scheduler::get( void ) {
		const auto base_address = memory.get_base_addr();

		if ( !base_address )
			return rbx::task_scheduler();

		return memory.read<rbx::task_scheduler>( base_address + Offsets::task_scheduler::Pointer );
	}

    std::uint64_t task_scheduler::get_max_fps( void ) {
		const auto address = get_address();

		if ( !address )
			return -1;
		
		return static_cast< std::uint64_t >( 1.0 / memory.read<double>( address + Offsets::task_scheduler::max_fps ) );
	}
}