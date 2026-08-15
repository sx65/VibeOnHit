#include "bool_value.h"

namespace rbx {
    bool bool_value::get_value( void ) const noexcept {
		const auto address = get_address();

		if ( !address )
			return false;

		return memory.read<bool>( address + Offsets::Core::Value );
    }
}