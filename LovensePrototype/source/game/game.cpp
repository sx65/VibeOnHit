#include "game.h"

#include <memory/memory.h>

#include "../overlay/overlay.h"

namespace _game {
	bool game::init( void ) {
		std::printf( "[+] getting render view...\n" );

		render_view = rbx::render_view::get();

		if ( !render_view.get_address() )
			return false;

		std::printf( "[+] getting visual engine...\n" );

		visual_engine = render_view.get_visual_engine();

		if ( !visual_engine.get_address() )
			return false;
		std::printf( "[+] getting datamodel...\n" );

		datamodel = render_view.get_datamodel();

		if ( !datamodel.get_address() )
			return false;
		std::printf( "[+] getting players...\n" );

		players = datamodel.find_child_by_class<rbx::instance>( "Players" );

		if ( !players.get_address() )
			return false;
		std::printf( "[+] getting workspace...\n" );

		workspace = datamodel.find_child_by_class<rbx::workspace>( "Workspace" );

		if ( !workspace.get_address() )
			return false;
		std::printf( "[+] getting camera...\n" );

		camera_obj = workspace.get_camera();

		if ( !camera_obj.get_address() )
			return false;
		std::printf( "[+] getting place info...\n" );

		place_info = datamodel.get_place_info();

		std::printf( "[+] initialized\n" );

		return true;
	}

	void game::scout( void ) {
		std::uint64_t last_datamodel_addr = datamodel.address;
		int_fast16_t last_place_id = place_info.place_id;

		bool datamodel_changed = ( last_datamodel_addr != datamodel.address );
		bool place_id_changed = ( last_place_id != place_info.place_id );
		bool is_teleporting = ( !datamodel.get_address() || datamodel_changed || place_id_changed || datamodel.get_name() != "Ugc" );

		while ( true ) {
			Sleep( 250 );

			const HWND& window_handle = memory.get_window_handle( memory.window_class_name );
			const std::uint64_t& process_id = memory.get_process_id( memory.process_name );

			if ( window_handle != memory.get_window_handle() || process_id != memory.get_process_id() ) {
				exit( 0 );
				_exit( 0 );
				quick_exit( 0 );
			}

			focused = overlay.top_hwnd == memory.get_window_handle();
			enabled = focused || overlay.top_hwnd == overlay.overlay_hwnd;

			rbx::datamodel new_datamodel = render_view.get_datamodel();

			if ( new_datamodel.address != datamodel.address ) {
				datamodel = new_datamodel;
				place_info = datamodel.get_place_info();
			}

			datamodel_changed = ( last_datamodel_addr != datamodel.address );
			place_id_changed = ( last_place_id != place_info.place_id );
			is_teleporting = ( !datamodel.get_address() || datamodel_changed || place_id_changed || datamodel.get_name() != "Ugc" );

			if ( is_teleporting ) {
				enabled = false;

				Sleep( 400 );

				visual_engine = render_view.get_visual_engine();

				if ( !visual_engine.get_address() )
					continue;

				Sleep( 150 );

				datamodel = render_view.get_datamodel();

				if ( !datamodel.get_address() )
					continue;

				Sleep( 150 );

				players = datamodel.find_child_by_class<rbx::instance>( "Players" );

				if ( !players.get_address() )
					continue;

				workspace = datamodel.find_child_by_class<rbx::workspace>( "Workspace" );

				if ( !workspace.get_address() )
					continue;

				camera_obj = workspace.get_camera();

				if ( !camera_obj.get_address() )
					continue;

				Sleep( 250 );

				last_datamodel_addr = datamodel.address;
				last_place_id = place_info.place_id;

				enabled = true;
			}
		}
	}
}