#include "workspace.h"

namespace rbx {
    camera workspace::get_camera( void ) const noexcept {
		const auto address = get_address();

		if ( !address )
			return rbx::camera();

		return memory.read<rbx::camera>( address + Offsets::camera::camera );
    }
}