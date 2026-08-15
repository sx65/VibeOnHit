#include "part.h"

namespace rbx {
    primitive part::get_primitive( void ) const noexcept {
		const auto address = get_address();

		if ( !address )
			return primitive();

		return memory.read<primitive>( address + Offsets::part::primitive );
    }
}