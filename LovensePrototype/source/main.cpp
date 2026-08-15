#include <print>

#include <memory/memory.h>
#include <iostream>

#include "game/game.h"
#include "overlay/overlay.h"
#include "cache/cache.h"
#include "features/features.h"

int main( void ) {
	std::println( "[+] initializing memory..." );

	if ( !memory.init( ) ) {
		std::println( "[-] failed to initialize memory factory" );
		std::cin.get();
		return -1;
	}

	std::println( "[+] attaching to process..." );

	if ( !memory.attach() ) {
		std::println( "[-] failed to attach to process" );
		std::cin.get();
		return -1;
	}

	std::println( "[+] initialized memory!\n" );

	std::println( "[*] process id: {}", memory.get_process_id() );
	std::println( "[*] window handle: {}", static_cast< void* >( memory.get_window_handle() ) );
	std::println( "[*] process handle: {}\n", static_cast< void* >( memory.get_process_handle() ) );

	std::println( "[+] initializing game..." );

	if ( !game->init() ) {
		std::println( "[-] failed to initialize game" );
		std::cin.get();
		return -1;
	}

	std::println( "[+] initialized game!\n" );

	std::println( "[*] datamodel: 0x{:X}", game->datamodel.get_address() );
	std::println( "[*] visual_engine: 0x{:X}\n", game->visual_engine.get_address() );

	game->thread_pool.enqueue( [] {
		game->scout();
	} );

	cache::start();
	features::start();

	overlay.start();
	//memory.detach();

	std::cin.get();

	return 0;
}