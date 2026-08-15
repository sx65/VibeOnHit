#include "vector3_value.h"

namespace rbx {
	math::vec3 vector3_value::get_value( void ) const noexcept {
		const auto address = get_address();

		if ( !address )
			return math::vec3();

		return memory.read<math::vec3>( address + Offsets::Core::Value );
	}
}