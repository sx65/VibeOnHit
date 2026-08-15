#include "render_view.h"

#include <memory/memory.h>
#include "offsets.h"

namespace rbx {
	render_view render_view::get( void ) {
		const auto render_job = memory.find_pattern( "52 65 6E 64 65 72 4A 6F 62 28 50 6F 73 74 52 65 6E 64 65 72 3B" );

		if ( !render_job )
			return rbx::render_view();

		return memory.read<rbx::render_view>( render_job + Offsets::Core::render_view_offset );
	}

	visual_engine render_view::get_visual_engine( void ) {
		const auto address = get_address();

		if ( !address )
			return rbx::visual_engine();

		return memory.read<rbx::visual_engine>( address + Offsets::Core::visual_engine );
	}

	datamodel render_view::get_datamodel( void ) {
		const auto address = get_address();

		if ( !address )
			return rbx::datamodel();

		const auto fake_datamodel = memory.read<std::uintptr_t>( address + Offsets::Core::DatamodelHolder );

		if ( !fake_datamodel )
			return rbx::datamodel();

		return memory.read<rbx::datamodel>( fake_datamodel + Offsets::Core::Datamodel );
	}
}