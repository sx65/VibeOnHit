#include "instance.h"

namespace rbx {
	std::string instance::get_name( void ) const noexcept {
		const auto address = get_address();
		if ( !address )
			return {};

		const auto pointer = memory.read<std::uint64_t>( address + Offsets::instance::name );
		if ( !pointer )
			return {};

		return String::ReadString2( pointer );
	}

	std::string instance::get_class_name( void ) const noexcept {
		const auto address = get_address();
		if ( !address )
			return {};

		const auto descriptor = memory.read<std::uint64_t>( address + Offsets::instance::ClassDescriptor );
		if ( !descriptor )
			return {};

		const auto pointer = memory.read<std::uint64_t>( descriptor + Offsets::Core::Self );
		if ( !pointer )
			return {};

		return String::ReadString2( pointer );
	}

	std::string instance::get_full_path( void ) const noexcept {
        std::vector<std::string> path_components;
        std::string full_path;

        const auto address = get_address();
        if ( !address )
            return full_path;

        auto curr = *this;

        while ( curr.get_address() ) {
            std::string name = curr.get_name();
            if ( !name.empty() )
                path_components.push_back( name );
            else
                path_components.push_back( "[UNKNOWN???]" );

            auto parent = curr.get_parent<rbx::instance>();
            if ( !parent.get_address() )
                break;

            curr = parent;
        }

        if ( !path_components.empty() ) {
            for ( auto it = path_components.rbegin(); it != path_components.rend(); ++it ) {
                if ( it != path_components.rbegin() )
                    full_path += "->";

                full_path += *it;
            }
        }

        return full_path;
	}
}