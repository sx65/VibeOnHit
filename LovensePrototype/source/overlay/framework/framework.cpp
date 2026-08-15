#include "framework.h"

#include "fonts/proggy_dotted.h"

#include <imgui/imgui_freetype.h>
#include <imgui/imgui_impl_dx11.h>
#include <string>
#include <functional>

#include "../../sdk/instance.h"
#include "../../game/game.h"
#include "../../config/config.h"
#include <print>
#include "../../sdk/string_value.h"
#include "../../sdk/int_value.h"
#include "../../sdk/number_value.h"
#include "../../sdk/bool_value.h"

using namespace ImGui;

ImFont* framework::fonts::main_13px = nullptr;
ImFont* framework::fonts::main_12px = nullptr;
ImFont* framework::fonts::main_11px = nullptr;
ImFont* framework::fonts::main_10px = nullptr;
ImFont* framework::fonts::main_9px = nullptr;

void sub_tab_explorer();
void sub_tab_player_list();

void framework::setup() {
	ImGuiIO& io = GetIO();

	ImFontConfig font_cfg;
	font_cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_Bitmap;
	font_cfg.FontDataOwnedByAtlas = false;

	io.Fonts->Clear();

	framework::fonts::main_13px = io.Fonts->AddFontFromMemoryTTF( proggy_dotted_data, sizeof proggy_dotted_data, 13.f, &font_cfg );
	framework::fonts::main_12px = io.Fonts->AddFontFromMemoryTTF( proggy_dotted_data, sizeof proggy_dotted_data, 12.f, &font_cfg );
	framework::fonts::main_11px = io.Fonts->AddFontFromMemoryTTF( proggy_dotted_data, sizeof proggy_dotted_data, 11.f, &font_cfg );
	framework::fonts::main_10px = io.Fonts->AddFontFromMemoryTTF( proggy_dotted_data, sizeof proggy_dotted_data, 10.f, &font_cfg );
	framework::fonts::main_9px = io.Fonts->AddFontFromMemoryTTF( proggy_dotted_data, sizeof proggy_dotted_data, 9.f, &font_cfg );

	io.Fonts->Build();
	//ImGui_ImplDX11_CreateDeviceObjects( );

	ImGuiStyle* style = &GetStyle();
	style->FramePadding.y = 8.f;
}

void framework::render( const char* name ) {
	PushStyleColor( ImGuiCol_WindowBg, colors::window );
	PushFont( fonts::main_13px );

	SetNextWindowSize( window_size, ImGuiCond_Once );

	if ( begin( name, NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize ) ) {
		static int curr_tab = 0;
		static int curr_aiming_sub_tab = 0;
		static int curr_visuals_sub_tab = 0;
		static int curr_misc_sub_tab = 0;
		static int curr_explorer_sub_tab = 0;
		static int curr_player_list_sub_tab = 0;

		PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 0 ) );
		if ( tab( "Aiming", curr_tab == 0 ) ) curr_tab = 0; SameLine();
		if ( tab( "Visuals", curr_tab == 1 ) ) curr_tab = 1; SameLine();
		if ( tab( "Miscellaneous", curr_tab == 2 ) ) curr_tab = 2; SameLine();
		if ( tab( "Explorer", curr_tab == 3 ) ) curr_tab = 3;

		if ( curr_tab == 0 ) {
			if ( sub_tab( "Aimbot", curr_aiming_sub_tab == 0 ) ) curr_aiming_sub_tab = 0; SameLine();
			if ( sub_tab( "Triggerbot", curr_aiming_sub_tab == 1 ) ) curr_aiming_sub_tab = 1; SameLine();
			if ( sub_tab( "Settings", curr_aiming_sub_tab == 2 ) ) curr_aiming_sub_tab = 2;
		} else if ( curr_tab == 1 ) {
			if ( sub_tab( "ESP", curr_visuals_sub_tab == 0 ) ) curr_visuals_sub_tab = 0; SameLine();
			if ( sub_tab( "Settings", curr_visuals_sub_tab == 1 ) ) curr_visuals_sub_tab = 1;
			//if ( sub_tab( "World", curr_visuals_sub_tab == 1 ) ) curr_visuals_sub_tab = 1;
		} else if ( curr_tab == 2 ) {
			if ( sub_tab( "Character", curr_misc_sub_tab == 0 ) ) curr_misc_sub_tab = 0; //SameLine( );
			//if ( sub_tab( "Universal", curr_misc_sub_tab == 1 ) ) curr_misc_sub_tab = 1;
		} else if ( curr_tab == 3 ) {
			if ( sub_tab( "Game", curr_explorer_sub_tab == 0 ) ) curr_explorer_sub_tab = 0; SameLine();
			if ( sub_tab( "Player List", curr_explorer_sub_tab == 1 ) ) curr_explorer_sub_tab = 1;
		}
		PopStyleVar();

		static ImVec2 cursor_pos = GetCursorPos();
		SetCursorPos( ImVec2( cursor_pos.x - 1.f, cursor_pos.y + 5.f ) );

		static constexpr float column_padding = 3.f;
		static const float column_width = window_size.x * 0.5f;

		static const float section_x = ( window_size.x * 0.5f ) - 11.f;
		static const float section_full_y = window_size.y - 66.f;

		BeginGroup();
		if ( curr_tab == 0 ) {
			if ( curr_aiming_sub_tab == 0 ) {
				columns( column_width - column_padding );

				begin_section( "General", ImVec2( section_x, section_full_y * 0.4475f + 0.5f ) );
				{
					checkbox( "Enabled", &config.aimbot.enabled );
					hotkey( "AimbotHotkey", &config.aimbot.hotkey );
					checkbox( "Sticky Aim", &config.aimbot.sticky_aim );
					combo( "Target Bone", &config.aimbot.bone, "Head\0" "UpperTorso\0" "HumanoidRootPart\0" "LowerTorso\0" );

					/*
					checkbox( "enabled", &aimbot.enabled );
					hotkey( "AimbotHotkey", &aimbot.hotkey );
					checkbox( "Sticky Aim", &aimbot.sticky_aim );

					combo( "Target Bone", &aimbot.bone, "Head\0" "UpperTorso\0" "PrimaryPart\0" "LowerTorso\0" );
					//combo( "Aim Movement", &aimbot.method, "Mouse\0" "Camera\0" "Silent Aim\0" );

					checkbox( "First Person", &aimbot.first_person );
					combo( "FP Aim Movement", &aimbot.first_person_mode, "Mouse\0" "Camera\0" "Silent Aim\0" );
					checkbox( "Third Person", &aimbot.third_person );
					combo( "TP Aim Movement", &aimbot.third_person_mode, "Mouse\0" "Camera\0" "Silent Aim\0" );*/
				}
				end_section();

				begin_section( "Rage", ImVec2( section_x, section_full_y * 0.5525f - 9.f ) );
				{
					/*
					checkbox( "Target Strafe", &target_strafe.enabled );
					checkbox( "View Target", &target_strafe.view_target );

					slider_float( "Radius", &target_strafe.radius, 0.0f, 50.0f );
					slider_float( "Speed", &target_strafe.speed, 0.0f, 50.0f );
					slider_float( "Height Offset", &target_strafe.height, -20.0f, 20.0f );*/
				}
				end_section();

				NextColumn();

				begin_section( "Settings", ImVec2( section_x, section_full_y ) );
				{
					/*
					checkbox( "Prediction", &aimbot.prediction );

					if ( aimbot.prediction ) {
						checkbox( "Auto-Resolver", &aimbot.resolver );

						slider_float( "Prediction X", &aimbot.prediction_value.x, 0.f, 0.5f );
						slider_float( "Prediction Y", &aimbot.prediction_value.y, 0.f, 0.5f );
					}

					//if ( !aimbot.bezier_enabled )
					checkbox( "Smoothness", &aimbot.smoothness );

					/*
					if ( !aimbot.smoothness )
					  checkbox( "Bezier Curves", &aimbot.bezier_enabled );

					if ( aimbot.bezier_enabled && aimbot.smoothness )
					  aimbot.smoothness = false;

					if ( aimbot.bezier_enabled )
					{
					  combo( "Easing Curve", &aimbot.bezier_curve,
						"Linear\0"
						"Quadratic\0"
						"Cubic\0"
						"Quartic\0"
						"Quintic\0"
						"Exponential\0"
						"Sine\0"
						"Circular\0"
						"Elastic\0"
						"Back\0"
						"Bounce\0"
					  );

					  combo( "Easing Type", &aimbot.bezier_type, "In\0" "Out\0" "In Out\0" );

					  slider_float( "Bezier Smoothness", &aimbot.bezier_smoothness, 1.0f, 10.0f );
					}

					if ( aimbot.smoothness )
						slider_float( "Smoothness Value", &aimbot.smoothness_value, 1.0f, 10.0f );

					checkbox( "Shake", &aimbot.shake );

					if ( aimbot.shake )
						slider_float( "Shake Value", &aimbot.shake_value, 0.0f, 20.0f );

					if ( aimbot.method == Enums::aimbot::Method::Mouse )
						slider_float( "Sensitivity", &aimbot.sensitivity, 0.0f, 1.0f );*/
				}
				end_section();
			} else if ( curr_aiming_sub_tab == 1 ) {
				columns( column_width - column_padding );

				//static auto& triggerbot = global.triggerbot;

				begin_section( "General", ImVec2( section_x, section_full_y ) );
				{
					/*
					checkbox( "enabled", &triggerbot.enabled );
					hotkey( "TriggerbotHotkey", &triggerbot.hotkey );
					checkbox( "Sticky Aim", &triggerbot.sticky_aim );

					combo( "Target Bone", &triggerbot.bone, "Head\0" "UpperTorso\0" "PrimaryPart\0" "LowerTorso\0" );

					checkbox( "Visualize Hitbox", &triggerbot.visualize );
					slider_float( "Hitbox Width", &triggerbot.hitbox.x, 0.0f, 5.0f );
					slider_float( "Hitbox Height", &triggerbot.hitbox.y, 0.0f, 5.0f );*/
				}
				end_section();

				NextColumn();

				begin_section( "Settings", ImVec2( section_x, section_full_y ) );
				{
					/*
					checkbox( "Prediction", &triggerbot.prediction );

					if ( triggerbot.prediction ) {
						checkbox( "Auto-Resolver", &triggerbot.resolver );

						slider_float( "Prediction X", &triggerbot.prediction_value.x, 0.f, 0.5f );
						slider_float( "Prediction Y", &triggerbot.prediction_value.y, 0.f, 0.5f );
					}

					slider_int( "Reaction Time", &triggerbot.reaction_time, 0, 300 );*/
				}
				end_section();
			} else if ( curr_aiming_sub_tab == 2 ) {
				columns( column_width - column_padding );

				//static auto& aiming_checks = global.aiming_checks;
				//static auto& roblox = global.roblox;

				begin_section( "Checks", ImVec2( section_x, section_full_y ) );
				{
					/*
					checkbox( "Team Check", &aiming_checks.team_check );
					checkbox( "Max distance", &aiming_checks.max_distance );
					slider_float( "Max distance Value", &aiming_checks.max_distance_value, 0.0f, 2000.0f, "m" );

					if ( roblox.place_id == Enums::Games::DaHood ) {
						checkbox( "K.O. Check", &aiming_checks.ko_check );
						checkbox( "Grabbed Check", &aiming_checks.grab_check );
					}*/
				}
				end_section();
			}
		} else if ( curr_tab == 1 ) {
			if ( curr_visuals_sub_tab == 0 ) {
				columns( column_width - column_padding );

				//static auto& visuals = global.visuals;

				//static int esp_settings = 0;

				//globals::esp_settings_t& settings =
				//    esp_settings == 0 ? visuals.enemy :
				//    esp_settings == 1 ? visuals.team :
				//    esp_settings == 2 ? visuals.local :
				//    visuals.target;

				//const char* category =
				//    esp_settings == 0 ? "Enemy" :
				//    esp_settings == 1 ? "Team" :
				//    esp_settings == 2 ? "Local" :
				//    "Target";

				begin_section( "Rendering", ImVec2( section_x, section_full_y ) );
				{
					checkbox( "Enabled", &config.esp.enabled ); color_edit_4( "ESP Color", config.esp.color );

					checkbox( "Box", &config.esp.box ); 
					checkbox( "Name", &config.esp.names );
					checkbox( "Distance", &config.esp.distance );
					checkbox( "Tool", &config.esp.tool );
					checkbox( "Health Bar", &config.esp.health_bar );
					combo( "Health Bar Position", &config.esp.health_bar_pos, "Top\0" "Bottom\0" );
					slider_float( "Max Distance", &config.esp.max_distance, 0.0f, 2500.0f, "m" );
					//checkbox( "Snaplines", &settings.snaplines.enabled );
					//checkbox( "Skeleton", &settings.skeleton.enabled );
					/*
					checkbox( "enabled", &visuals.enabled );

					combo( "esp Settings", &esp_settings, "Enemy\0" "Team\0" "Local\0" "Target\0" );

					checkbox( fmt::format( "{} esp", category ).c_str(), &settings.enabled );

					checkbox( "Box", &settings.box.enabled ); color_edit_4( "Box Color", settings.box.color );
					checkbox( "Box Fill", &settings.box.fill.enabled ); color_edit_4( "Box Fill Color", settings.box.fill.color );
					checkbox( "Name", &settings.name.enabled ); color_edit_4( "Name Color", settings.name.color );
					checkbox( "distance", &settings.distance.enabled ); color_edit_4( "distance Color", settings.distance.color );
					checkbox( "Tool", &settings.tool.enabled ); color_edit_4( "Tool Color", settings.tool.color );
					checkbox( "Health Bar", &settings.health_bar.enabled ); color_edit_4( "Health Bar Color", settings.health_bar.color );
					checkbox( "Snaplines", &settings.snaplines.enabled ); color_edit_4( "Snaplines Color", settings.snaplines.color );
					checkbox( "Skeleton", &settings.skeleton.enabled ); color_edit_4( "Skeleton Color", settings.skeleton.color );*/
				}
				end_section();

				NextColumn();

				begin_section( "Outlines", ImVec2( section_x, section_full_y ) );
				{
					/*
					checkbox( "Box Outline", &settings.box.outline.enabled ); color_edit_4( "Box Outline Color", settings.box.outline.color );
					checkbox( "Name Outline", &settings.name.outline.enabled ); color_edit_4( "Name Outline Color", settings.name.outline.color );
					checkbox( "distance Outline", &settings.distance.outline.enabled ); color_edit_4( "distance Outline Color", settings.distance.outline.color );
					checkbox( "Tool Outline", &settings.tool.outline.enabled ); color_edit_4( "Tool Outline Color", settings.tool.outline.color );
					checkbox( "Health Bar Outline", &settings.health_bar.outline.enabled ); color_edit_4( "Health Bar Outline Color", settings.health_bar.outline.color );
					checkbox( "Snaplines Outline", &settings.snaplines.outline.enabled ); color_edit_4( "Snaplines Outline Color", settings.snaplines.outline.color );
					checkbox( "Skeleton Outline", &settings.skeleton.outline.enabled ); color_edit_4( "Skeleton Outline Color", settings.skeleton.outline.color );
					*/
				}
				end_section();
			} else if ( curr_visuals_sub_tab == 1 ) {
				columns( column_width - column_padding );

				//static auto& visuals_checks = global.visuals_checks;
				//static auto& roblox = global.roblox;

				begin_section( "Checks", ImVec2( section_x, section_full_y ) );
				{
					/*
					checkbox( "Max distance", &visuals_checks.max_distance );
					slider_float( "Max distance Value", &visuals_checks.max_distance_value, 0.0f, 2000.0f, "m" );

					if ( roblox.place_id == Enums::Games::DaHood ) {
						checkbox( "K.O. Check", &visuals_checks.ko_check );
						checkbox( "Grabbed Check", &visuals_checks.grab_check );
					}
					*/
				}
				end_section();
			}
		} else if ( curr_tab == 2 ) {
			if ( curr_misc_sub_tab == 0 ) {
				columns( column_width - column_padding );

				//static auto& misc = global.misc;

				begin_section( "General", ImVec2( section_x, section_full_y ) );
				{
					if ( game->local_player.is_valid() && game->local_player.humanoid.get_address() != 0 ) {
						float health = game->local_player.humanoid.get_health();
						float max_health = game->local_player.humanoid.get_max_health();
						framework::text( "Local Player Health: %.2f / %.2f", health, max_health );
					}
					/*
					checkbox( "NoClip", &misc.noclip.enabled );
					hotkey( "NoclipHotkey", &misc.noclip.hotkey );
					checkbox( "Fly", &misc.fly.enabled );
					hotkey( "FlyHotkey", &misc.fly.hotkey );
					slider_float( "Fly Speed", &misc.fly.speed, 0.0f, 50.0f );
					slider_int( "WalkSpeed", &misc.humanoid.walkspeed, 0, 750 );
					slider_int( "JumpPower", &misc.humanoid.jumppower, 0, 750 );*/
				}
				end_section();
			}
		} else if ( curr_tab == 3 ) {
			if ( curr_explorer_sub_tab == 0 ) sub_tab_explorer();
			else if ( curr_explorer_sub_tab == 1 ) sub_tab_player_list();
		}
		EndGroup();

		end();
	}

	PopFont();
	PopStyleColor();
}

void framework::columns( float offset ) {
	Columns( 2, 0, false );
	SetColumnOffset( 1, offset );
}

void sub_tab_explorer() {
	static std::vector<rbx::instance> selected_instances;
	//static RBX::Instance& root_instance = game.datamodel;

	static auto& workspace = game->workspace;
	static auto& players = game->players;
	//static RBX::Instance& lighting = services.lighting;
	//static RBX::Instance& replicated_first = services.replicated_first;
	//static RBX::Instance& replicated_storage = services.replicated_storage;
	//static RBX::Instance& starter_gui = services.starter_gui;
	//static RBX::Instance& starter_pack = services.starter_pack;
	//static RBX::Instance& starter_player = services.starter_player;
	//static RBX::Instance& teams = services.teams;

	static const float size_x = framework::window_size.x - 14.f;
	static const float size_y = ( framework::window_size.y - 66.f );
	static const float size_y_65 = size_y * 0.65f;
	static const float size_y_35 = size_y * 0.325f;

	framework::begin_section( "Explorer", ImVec2( size_x, selected_instances.empty() ? size_y : size_y_65 ) );
	{
		BeginChild( "DexTree", ImVec2( 0, 0 ), 0, ImGuiWindowFlags_NoScrollbar );
		{
			std::function<void( rbx::instance& )> render_tree = [ & ]( rbx::instance& instance ) {
				std::string name = instance.get_name();
				std::string class_name = instance.get_class_name();
				std::string display_text = name + " [" + class_name + "]";

				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

				const bool is_empty = instance.get_children<rbx::instance>().size() <= 0;

				if ( is_empty )
					flags |= ImGuiTreeNodeFlags_Leaf;

				bool is_selected = std::find_if( selected_instances.begin(), selected_instances.end(),
					[ &instance ]( const rbx::instance& selected ) {
						return selected.address == instance.address;
					} ) != selected_instances.end();

				if ( !is_empty && is_selected )
					flags |= ImGuiTreeNodeFlags_Selected;

				PushID( ( void* )instance.address );
				bool is_open = framework::tree_node_ex( display_text.c_str(), flags );

				if ( IsItemClicked() ) {
					selected_instances.clear();
					selected_instances.push_back( instance );
				}

				if ( is_open ) {
					if ( class_name == "Players" ) {
						auto children = *game->player_list.get_snapshot();
						for ( auto& child : children )
							render_tree( child );
					} else {
						auto children = instance.get_children<rbx::instance>();
						for ( auto& child : children )
							render_tree( child );
					}
					TreePop();
				}

				PopID();
			};

			if ( workspace.get_address() )
				render_tree( workspace );
			if ( players.get_address() )
				render_tree( players );

			auto replicated_storage = game->datamodel.find_child_by_class<rbx::instance>( "ReplicatedStorage" );
			auto replicated_first = game->datamodel.find_child_by_class<rbx::instance>( "ReplicatedFirst" );

			/*if ( lighting.get_address() )
			  render_tree( lighting );*/
			if ( replicated_first.get_address() )
			  render_tree( replicated_first );
			if ( replicated_storage.get_address() )
			  render_tree( replicated_storage );
			/*if ( starter_gui.get_address() )
			  render_tree( starter_gui );
			if ( starter_pack.get_address() )
			  render_tree( starter_pack );
			if ( starter_player.get_address() )
			  render_tree( starter_player );
			if ( teams.get_address() )
			  render_tree( teams );*/
			framework::separator_ex( 0, 0.0f );
		}
		EndChild();
	}
	framework::end_section();

	if ( !selected_instances.empty() ) {
		framework::begin_section( "Properties", ImVec2( size_x, size_y_35 ) );
		{
			BeginChild( "Propertiess", ImVec2( 0, 0 ), 0, ImGuiWindowFlags_NoScrollbar );
			{
				auto& selected = selected_instances[ 0 ];

				std::string selected_class_name = selected.get_class_name();
				std::string selected_name = selected.get_name();

				PushStyleColor( ImGuiCol_Text, framework::colors::text );
				PushFont( framework::fonts::main_12px );

				framework::text( "Name: %s", selected_name.c_str() );
				framework::text( "Class Name: %s", selected_class_name.c_str() );
				framework::text( "Address: 0x%llX", selected.address );

				if ( selected_class_name == "Part" || selected_class_name == "MeshPart" || selected_class_name == "BasePart" ) {
					framework::separator();

					rbx::part part;
					part.address = selected.get_address();
					const auto& primitive = part.get_primitive();
					math::vec3 pos = primitive.get_position();
					math::vec3 size = primitive.get_size();
					//float transparency = primitive.GetTransparency( );

					framework::text( "Position: %.2f, %.2f, %.2f", pos.x, pos.y, pos.z );
					framework::text( "Size: %.2f, %.2f, %.2f", size.x, size.y, size.z );
					//framework::text( "Transparency: %.2f", transparency );
				}

				if ( selected_class_name == "Humanoid" ) {
					framework::separator();

					rbx::humanoid humanoid;
					humanoid.address = selected.get_address();
					float health = humanoid.get_health();
					float max_health = humanoid.get_max_health();

					framework::text( "Health: %.2f", health );
					framework::text( "Max Health: %.2f", max_health );
				}

				if ( selected_class_name == "Player" ) {
					framework::separator();

					rbx::player player;
					player.address = selected.get_address();
					//std::string team_name = player.get_team( ).GetName( );
					auto character = player.get_model_instance();
					auto character_address = character.get_address();

					//framework::text( "Team: %s", team_name.c_str( ) );

					if ( character_address != 0 ) {
						auto character_parent = character.get_parent<rbx::instance>();
						auto character_name = character.get_name();
						auto character_parent_name = character_parent.get_name();

						framework::text( "Character Name: %s", character_name.c_str() );
						framework::text( "Character Parent Name: %s", character_parent_name.c_str() );
						framework::text( "Character Address: 0x%llX", character_address );
					}
				}

				if ( selected_class_name == "StringValue" ) {
					framework::separator();

					rbx::string_value value;
					value.address = selected.get_address();
					auto val = value.get_value();

					framework::text( "Value: %s", val.c_str() );
				}

				if ( selected_class_name == "BoolValue" ) {
					framework::separator();

					rbx::bool_value value;
					value.address = selected.get_address();
					auto val = value.get_value();
					auto val_text = val == true ? "true" : "false";

					framework::text( "Value: %s", val_text );
				}

				if ( selected_class_name == "IntValue" ) {
					framework::separator();

					rbx::int_value value;
					value.address = selected.get_address();
					auto val = value.get_value();

					framework::text( "Value: %.1f", val );
				}

				if ( selected_class_name == "NumberValue" ) {
					framework::separator();

					rbx::number_value value;
					value.address = selected.get_address();
					auto val = value.get_value();

					framework::text( "Value: %.5f", val );
				}

				/*
				if ( selected_class_name == "SpecialMesh" )
				{
				  framework::separator( );

				  c_special_mesh mesh;
				  mesh.address = selected.get_address();
				  Math::Vector3 scale = mesh.get_scale( );
				  std::string mesh_id = mesh.get_mesh_id( );
				  std::string texture_id = mesh.get_texture_id( );
				  std::string mesh_type = mesh.get_mesh_type( );

				  framework::text( "Scale: %.2f, %.2f, %.2f", scale.x, scale.y, scale.z );
				  framework::text( "Mesh ID: %s", mesh_id.c_str( ) );
				  framework::text( "Mesh Type: %s", mesh_type.c_str( ) );
				  framework::text( "Texture ID: %s", texture_id.c_str( ) );
				}

				if ( selected_class_name == "ObjectValue" )
				{
				  framework::separator( );

				  c_object_value value;
				  value.address = selected.get_address();
				  auto val = value.get_value( );

				  framework::text( "Value Name: %s", val.GetName( ) );
				  framework::text( "Value Address: 0x%llX", val.get_address() );
				}

				if ( selected_class_name == "Shirt" )
				{
				  framework::separator( );

				  c_shirt shirt;
				  shirt.address = selected.get_address();
				  std::string texture_id = shirt.get_texture( );

				  framework::text( "Texture ID: %s", texture_id.c_str( ) );
				}

				if ( selected_class_name == "Pants" )
				{
				  framework::separator( );

				  c_pants pants;
				  pants.address = selected.get_address();
				  std::string texture_id = pants.get_texture( );

				  framework::text( "Texture ID: %s", texture_id.c_str( ) );
				}

				if ( selected_class_name == "Texture" )
				{
				  framework::separator( );

				  c_texture texture;
				  texture.address = selected.get_address();
				  std::string texture_id = texture.get_texture( );

				  framework::text( "Texture ID: %s", texture_id.c_str( ) );
				}

				if ( selected_class_name == "TextLabel" )
				{
				  framework::separator( );

				  c_text_label text_label;
				  text_label.address = selected.get_address();
				  std::string text_value = text_label.get_text( );
				  udim2_t position = text_label.get_position( );
				  udim2_t size = text_label.get_size( );

				  framework::text( "Text: %s", text_value.c_str( ) );
				  framework::text( "Position: {%.2f, %d}, {%.2f, %d}", position.scale_x, position.offset_x, position.scale_y, position.offset_y );
				  framework::text( "Size: {%.2f, %d}, {%.2f, %d}", size.scale_x, size.offset_x, size.scale_y, size.offset_y );
				}

				if ( selected_class_name == "Frame" )
				{
				  framework::separator( );

				  c_frame frame;
				  frame.address = selected.get_address();
				  udim2_t position = frame.get_position( );
				  udim2_t size = frame.get_size( );

				  framework::text( "Position: {%.2f, %d}, {%.2f, %d}", position.scale_x, position.offset_x, position.scale_y, position.offset_y );
				  framework::text( "Size: {%.2f, %d}, {%.2f, %d}", size.scale_x, size.offset_x, size.scale_y, size.offset_y );
				}

				if ( selected_class_name == "Weld" )
				{
				  c_weld weld;
				  weld.address = selected.get_address();
				  auto c0 = weld.get_c0( );
				  auto c1 = weld.get_c1( );

				  auto c0_address = c0.get_address();
				  auto c1_address = c1.get_address();

				  if ( c0_address )
				  {
					framework::separator( );

					std::string c0_name = c0.GetName( );
					std::string c0_class_name = c0.get_class_name( );

					framework::text( "C0 Address: 0x%llX", c0_address );
					framework::text( "C0 Name: %s", c0_name );
					framework::text( "C0 Class Name: %s", c0_class_name );
				  }

				  if ( c1_address )
				  {
					framework::separator( );

					std::string c1_name = c1.GetName( );
					std::string c1_class_name = c1.get_class_name( );

					framework::text( "C1 Address: 0x%llX", c1_address );
					framework::text( "C1 Name: %s", c1_name );
					framework::text( "C1 Class Name: %s", c1_class_name );
				  }
				}*/

				SetCursorPosY( GetCursorPosY() + 10.f );

				PopFont();
				PopStyleColor();
			}
			EndChild();
		}
		framework::end_section();
	}
}

void sub_tab_player_list() {
	static rbx::player selected_player;

	static auto& workspace = game->workspace;
	static auto& players = game->players;

	static const float size_x = framework::window_size.x - 14.f;
	static const float size_y = ( framework::window_size.y - 66.f );
	static const float size_y_65 = size_y * 0.65f;
	static const float size_y_35 = size_y * 0.325f;

	static bool is_spectating = false;
	auto is_valid = selected_player.get_address() > 0;

	framework::begin_section( "Player List", ImVec2( size_x, !is_valid ? size_y : size_y_65));
	{
		ImGui::BeginChild(
			"PlayerListChild",
			ImVec2( 0, 0 ),
			false,
			ImGuiWindowFlags_AlwaysVerticalScrollbar
		);

		auto& players = *game->player_list.get_snapshot();

		for ( const rbx::player& player : players ) {
			if ( player.get_address() == game->local_player.get_address() )
				continue;

			PushID( ( void* )player.get_address() );

			auto& list = config.lists.whitelisted;

			bool is_whitelisted = std::find_if( list.begin(), list.end(),
				[ & ]( const std::uintptr_t& whitelisted ) {
					return whitelisted == player.get_address();
				} ) != list.end();

			bool is_selected = ( player.get_address() == selected_player.get_address() );

			std::string label = player.get_name() + ( is_whitelisted ? " [Whitelisted]" : "" );

			if ( framework::selectable( label.c_str(), is_selected ) )
				selected_player = player;

			PopID();
		}

		framework::separator_ex( 0, 0.0f );

		EndChild();
	}
	framework::end_section();

	if ( is_valid ) {
		framework::begin_section( "Actions", ImVec2( size_x, size_y_35 ) );
		{
			BeginChild( "Actionsss", ImVec2( 0, 0 ), 0, ImGuiWindowFlags_NoScrollbar );
			{
				PushStyleColor( ImGuiCol_Text, framework::colors::text );
				PushFont( framework::fonts::main_12px );

				if ( framework::button( "Teleport", ImVec2( size_x - 15.0f, 25.0f ) ) ) {
					auto body_part = selected_player.body_parts.get( "HumanoidRootPart" );
					auto local_body_part = game->local_player.body_parts.get( "HumanoidRootPart" );

					if ( body_part.is_valid() && local_body_part.is_valid() ) {
						auto primitive = body_part.get_primitive();
						auto local_primitive = local_body_part.get_primitive();

						if ( primitive.is_valid() && local_primitive.is_valid() )
							local_primitive.set_position( primitive.get_position() );
					}
				}

				auto& list = config.lists.whitelisted;

				bool is_whitelisted = std::find_if( list.begin(), list.end(),
					[ & ]( const std::uintptr_t& whitelisted ) {
						return whitelisted == selected_player.address;
					} ) != list.end();

				if ( is_whitelisted ) {
					if ( framework::button( "Remove from whitelist", ImVec2( size_x - 15.0f, 25.0f ) ) ) {
						auto it = std::find( list.begin(), list.end(), selected_player.address );

						if ( *it )
							list.erase( it );
					}
				} else {
					if ( framework::button( "Whitelist", ImVec2( size_x - 15.0f, 25.0f ) ) )
						list.push_back( selected_player.address );
				}

				if ( !is_spectating ) {
					if ( framework::button( "Spectate", ImVec2( size_x - 15.0f, 25.0f ) ) ) {
						game->camera_obj.set_subject( selected_player.humanoid.address );
						is_spectating = true;
					}
				} else {
					if ( framework::button( "Unspectate", ImVec2( size_x - 15.0f, 25.0f ) ) ) {
						game->camera_obj.set_subject( game->local_player.humanoid.address );
						is_spectating = false;
					}
				}

				SetCursorPosY( GetCursorPosY() + 10.0f );

				PopFont();
				PopStyleColor();
			}
			EndChild();
		}
		framework::end_section();
	}
}