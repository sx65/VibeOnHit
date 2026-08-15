#pragma once

#include "instance.h"

namespace rbx {
	struct place_info {
		std::string game_name = "";
		int_fast16_t game_id = 0;
		int_fast16_t place_id = 0;
		int_fast16_t server_ip = 0;
	};

	class datamodel : public rbx::instance {
	public:
		static rbx::datamodel get( void ) noexcept;

		place_info get_place_info( void ) const noexcept;
	};
}