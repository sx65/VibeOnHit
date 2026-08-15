#include "string_value.h"

namespace rbx {
	std::string string_value::get_value( void ) const noexcept {
		const auto address = get_address();

		if ( !address )
			return std::string();

		const auto pointer = memory.read<std::uint64_t>( address + Offsets::Core::Value );
		if ( !pointer )
			return {};

		return String::ReadString2( pointer );
	}
}