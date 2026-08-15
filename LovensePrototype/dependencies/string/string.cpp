#include "string.h"

#include <memory/memory.h>

namespace String {
    std::string ReadString2( const std::uint64_t string_addr ) noexcept {
        if ( !string_addr || !memory.is_valid_addr( string_addr ) )
            return {};

        const int length = memory.read<int>( string_addr + 0x18 );
        if ( length <= 0 || length > 200 )
            return {};

        const bool is_long_string = length >= 16;
        std::uint64_t target_addr;

        if ( is_long_string ) {
            target_addr = memory.read<std::uint64_t>( string_addr );
            if ( !target_addr || !memory.is_valid_addr( target_addr ) )
                return {};
        } else {
            target_addr = string_addr;
        }

        return memory.read_string( target_addr, length );
	}
}