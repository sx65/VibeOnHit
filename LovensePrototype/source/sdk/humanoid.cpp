#include "humanoid.h"

namespace rbx {
    rig_type humanoid::get_rig_type( void ) const noexcept {
        const auto address = get_address();
        if ( !address )
            return rig_type::Unknown;

        return memory.read<rig_type>( address + Offsets::humanoid::rig_type );
    }

    float humanoid::get_health( void ) const noexcept {
        const auto address = get_address();
        if ( !address )
            return -1;

        return memory.read<float>( address + Offsets::humanoid::health );
    }

    float humanoid::get_max_health( void ) const noexcept {
        const auto address = get_address();
        if ( !address )
            return -1;

        return memory.read<float>( address + Offsets::humanoid::max_health );
    }
}