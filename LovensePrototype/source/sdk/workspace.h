#pragma once

#include "instance.h"
#include "camera.h"

namespace rbx {
	class workspace : public rbx::instance {
	public:
		rbx::camera get_camera( void ) const noexcept;
	};
}