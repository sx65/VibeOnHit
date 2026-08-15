#include "number_value.h"

namespace rbx {
	double number_value::get_value( void ) const noexcept {
		const auto address = get_address();

		if ( !address )
			return 0.0;

		return memory.read<double>( address + Offsets::Core::Value );
	}
}