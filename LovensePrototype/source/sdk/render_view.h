#pragma once

#include "base.h"
#include "visual_engine.h"
#include "datamodel.h"

namespace rbx {
	class render_view : public rbx::base {
	public:
		static rbx::render_view get( void );

		visual_engine get_visual_engine( void );
		datamodel get_datamodel( void );
	};
}