#include "int_value.h"

namespace rbx {
	float int_value::get_value( void ) const noexcept {
		const auto address = get_address();

		if ( !address )
			return 0.0f;

		return memory.read<float>( address + Offsets::Core::Value );
	}
}