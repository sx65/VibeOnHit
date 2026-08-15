#pragma once

#include "../overlay/framework/framework.h"
#include "../sdk/player.h"

namespace config_manager {
	using rgba = float[ 4 ];

	struct checks {
		bool max_distance = false;
		float max_distance_value = 250.0f;
	};

	struct fov {
		bool enabled = false;
		float value = 200.0f;
	};

	class config final {
	public:
		struct _aimbot {
			bool enabled = false;
			bool sticky_aim = true;
			bool resolver = false;

			config_manager::fov fov;
			config_manager::checks checks;

			int bone = 0;

			rbx::player curr_target;

			framework::hotkey_t hotkey;
		} aimbot;

		struct _esp {
			bool enabled = true;
			bool box = true;
			bool box_fill = true;
			bool names = true;
			bool tool = true;
			bool distance = true;

			bool health_bar = true;
			int health_bar_pos = 0;

			float max_distance = 750.0f;

			rgba color = { 1.0f, 0.792156863f, 0.898039216f, 0.0f };
		} esp;

		struct _lists {
			std::vector<std::uintptr_t> whitelisted;
		} lists;

		static config& get() {
			static config c;
			return c;
		}

		config( const config& ) = delete;
		config& operator=( const config& ) = delete;

	private:
		config() = default;
	};
}

#define config config_manager::config::get()