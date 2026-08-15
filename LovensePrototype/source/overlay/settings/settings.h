#pragma once

#include <string>
#include <vector>
#include <d3d11.h>
#include <memory>

class CVariables {
public:
	struct {
		ImGuiWindowFlags main_flags{ ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_Tooltip };
		ImGuiWindowFlags flags{ ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground };
		ImVec2 padding{ 0, 0 };
		ImVec2 spacing{ 5, 6 };
		float shadow_size{ 18 };
		float shadow_alpha{ 0.6f };
		float border_size{ 0 };
		float rounding{ 2 };
		float width{ 0 };
		float titlebar{ 20 };
		float scrollbar_size{ 2 };
		bool hover_hightlight{ true };
	} window;

	struct {
		bool show_loader{ false };
		bool loader_loaded{ false };
		float loader_alpha{ 1.0f };
		float transition_timer{ 0.0f };

		int loader_stage{ 0 };
		float loader_progress{ 0.0f };
		bool loader_stage_complete{ false };

		bool current_section[ 7 ];
		const char* section_icons[ IM_ARRAYSIZE( current_section ) ] = { "A", "B", "C", "D", "E", "F", "G" };

		float menu_alpha{ 0 };
		bool menu_opened{ false };
		int menu_key{ 45 };


		bool vsync{ true };
		bool optimize{ false };
	} gui;

	struct {
		ImFont* icons[ 2 ];
		ImFont* anta;
		ImFont* pixel;
		ImFont* verdana;
		ImFont* tahoma;

		ImFont* default_font;
		ImFont* icons_awesome;
	} font;

	struct {
		ID3D11ShaderResourceView* glow_light = nullptr;
		ID3D11ShaderResourceView* grid_bg = nullptr;
	} image;

};

inline std::shared_ptr<CVariables> variables = std::make_shared<CVariables>();