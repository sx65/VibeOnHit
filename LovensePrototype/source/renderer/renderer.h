#pragma once

#include <imgui/imgui.h>

namespace renderer {
	void name( const ImVec2& min, const ImVec2& max, const char* name, const float scale_factor, const float col[ 4 ] );
	void tool( const ImVec2& min, const ImVec2& max, const char* tool, const float scale_factor, const bool tool_equipped, const float col[ 4 ] );
	void distance( const ImVec2& min, const ImVec2& max, float distance, const float scale_factor, const bool tool_equipped, const float col[ 4 ] );
	void rectangle( const ImVec2& min, const ImVec2& max, const float col[ 4 ] );
	void health_bar( const ImVec2& min, const ImVec2& max, float health, float max_health, const float col[ 4 ] );
}