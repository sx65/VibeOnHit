#include "datamodel.h"

#include <memory/memory.h>
#include "offsets.h"

namespace rbx {
	datamodel datamodel::get( void ) noexcept {
		const auto base_address = memory.get_base_addr();

		if ( !base_address )
			return rbx::datamodel();

		std::uint64_t fake_datamodel = memory.read<std::uint64_t>( base_address + Offsets::FakeDataModel::Pointer );

		if ( !fake_datamodel )
			return rbx::datamodel();

		return memory.read<rbx::datamodel>( fake_datamodel + Offsets::FakeDataModel::RealDataModel );
	}
	
	place_info datamodel::get_place_info( void ) const noexcept {
		place_info place_info;

		const auto address = get_address();

		if ( !address )
			return place_info;

		place_info.game_id = memory.read<int_fast16_t>( address + Offsets::Game::GameID );
		place_info.place_id = memory.read<int_fast16_t>( address + Offsets::Game::place_id );
		place_info.server_ip = memory.read<int_fast16_t>( address + Offsets::Game::server_ip );

		return place_info;
	}
}