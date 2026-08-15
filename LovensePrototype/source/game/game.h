#pragma once

#include <thread_pool/thread_pool.h>

#include "../sdk/instance.h"
#include "../sdk/task_scheduler.h"
#include "../sdk/datamodel.h"
#include "../sdk/visual_engine.h"
#include "../sdk/render_view.h"
#include "../sdk/player.h"
#include "../sdk/workspace.h"
#include "../sdk/camera.h"

#include <memory>
#include <safe_vector/safe_vector.h>

namespace _game {
	class game final {
	public:
		bool enabled = true;
		bool focused = false;

		thread_pool::thread_pool thread_pool;
		safe::vector<rbx::player> player_list;

		rbx::render_view render_view;
		rbx::datamodel datamodel;
		rbx::visual_engine visual_engine;
		rbx::workspace workspace;
		rbx::instance players;
		rbx::camera camera_obj;
		rbx::task_scheduler scheduler;
		rbx::player local_player;

		math::vec2 dimensions;
		math::matrix4 view_matrix;

		int max_fps = 0;
		rbx::place_info place_info;

		bool init( void );
		void scout( void );
	};
}

inline std::shared_ptr<_game::game> game = std::make_shared<_game::game>();