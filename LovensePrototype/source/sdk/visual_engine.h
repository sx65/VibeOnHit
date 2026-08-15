#pragma once

#include "base.h"

#include "../math/matrix.h"

namespace rbx {
	class visual_engine : public rbx::base {
	public:
		static rbx::visual_engine get( void );

		math::matrix4 get_view_matrix( void );
		math::vec2 get_dimensions( void );

		math::vec2 w2s( const math::vec3& position ) const noexcept;
	};
}