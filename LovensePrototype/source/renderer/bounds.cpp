#include "bounds.h"

#include "../game/game.h"

std::vector<std::string> r15_bone_vector = { "Head", "UpperTorso", "LowerTorso", "HumanoidRootPart", "RightUpperArm", "RightLowerArm", "RightHand", "LeftUpperArm", "LeftLowerArm", "LeftHand", "RightUpperLeg", "RightLowerLeg", "RightFoot", "LeftUpperLeg", "LeftLowerLeg", "LeftFoot", };
std::vector<std::string> r6_bone_vector = { "Head", "Torso", "HumanoidRootPart", "Right Arm", "Left Arm", "Right Leg", "Left Leg", };

namespace bounds {
	std::vector<math::vec3> GetCorners( const math::vec3& position, const math::vec3& size ) {
		std::vector<math::vec3> corners;

		for ( int X = -1; X <= 1; X += 2 )
			for ( int Y = -1; Y <= 1; Y += 2 )
				for ( int Z = -1; Z <= 1; Z += 2 ) {
					corners.push_back( {
						position.x + size.x * X,
						position.y + size.y * Y,
						position.z + size.z * Z
						} );
				}

		return corners;
	}

	bounds get_bounds( rbx::player& player, bounds_type type ) {
		bounds ret{ {-99.f, -99.f}, {-99.f, -99.f} };

		if ( type == bounds_type::STATIC ) {
			math::vec3 root_pos_3d = player.body_parts.get( "HumanoidRootPart" ).get_primitive().get_position();
			if ( root_pos_3d == 0 ) return ret;

			math::vec2 head_pos = game->visual_engine.w2s( root_pos_3d + math::vec3( 0.f, 0.9f, 0.f ) );
			if ( head_pos == 0 ) return ret;

			math::vec2 torso_pos = game->visual_engine.w2s( root_pos_3d - math::vec3( 0.f, 0.4f, 0.f ) );
			if ( torso_pos == 0 ) return ret;

			float width = std::fabs( head_pos.y - torso_pos.y ) * 1.9f;
			float height = width * 1.3f;

			if (
				std::isnan( width ) || std::isnan( height ) ||
				!std::isfinite( width ) || !std::isfinite( height ) ||
				height <= 0 || width <= 0
				) return ret;

			math::vec2 min = { torso_pos.x - width, torso_pos.y - height };
			math::vec2 max = { torso_pos.x + width, torso_pos.y + height };

			if (
				min.x <= 0 || min.y <= 0 ||
				min.x > game->dimensions.x || min.y > game->dimensions.y ||
				max.x <= 0 || max.y <= 0 ||
				max.x > game->dimensions.x || max.y > game->dimensions.y
				)
				return ret;

			ret.min = min;
			ret.max = max;
		} else if ( type == bounds_type::Bounding ) {
			std::vector<math::vec3> all_corners = {};

			const auto& part_vector = r15_bone_vector;

			for ( const auto& part_name : part_vector ) {
				rbx::part part = player.body_parts.get( part_name );
				if ( part.get_address() == 0 )
					continue;

				const rbx::primitive& primitive = part.get_primitive();
				if ( primitive.get_address() == 0 )
					continue;

				std::vector<math::vec3> corners = GetCorners( primitive.get_position(), primitive.get_size() * 0.5f );
				all_corners.insert( all_corners.end(), corners.begin(), corners.end() );
			}

			if ( all_corners.empty() )
				return ret;

			float min_x = game->dimensions.x, min_y = game->dimensions.y;
			float max_x = 0.0f, max_y = 0.0f;

			for ( math::vec3& corner : all_corners ) {
				math::vec2 part_position = game->visual_engine.w2s( corner );
				if ( part_position == -1.f )
					continue;

				if ( part_position.x > max_x ) max_x = part_position.x;
				if ( part_position.y > max_y ) max_y = part_position.y;
				if ( part_position.x < min_x ) min_x = part_position.x;
				if ( part_position.y < min_y ) min_y = part_position.y;
			}

			if ( max_x <= 0 || min_y >= game->dimensions.x || max_y <= 0 || max_y >= game->dimensions.y )
				return ret;

			ret.min = { min_x, min_y };
			ret.max = { max_x, max_y };
		}

		return ret;
	}
}