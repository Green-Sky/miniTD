#include "./game_hud.hpp"

//#include <mm/services/screen_director.hpp>
#include <mm/services/scene_service_interface.hpp>

#include "../components/player_lives.hpp"

#include <entt/entity/registry.hpp>

#include <imgui/imgui.h>

namespace mini_td::Services {

bool GameHUD::enable(MM::Engine& engine, std::vector<MM::UpdateStrategies::TaskInfo>& task_array) {
	// setup tasks
	task_array.push_back(
		MM::UpdateStrategies::TaskInfo{"GameHUD::render"}
		.fn([this](MM::Engine& e){ render(e); })
		.phase(MM::UpdateStrategies::update_phase_t::PRE)
		.succeed("ImGuiService::new_frame")
	);

	return true;
}

void GameHUD::disable(MM::Engine&) {
}

void GameHUD::render(MM::Engine& engine) {
	auto& ssi = engine.getService<MM::Services::SceneServiceInterface>();
	auto& scene = ssi.getScene();

	auto [display_w, display_h] = ImGui::GetIO().DisplaySize; // hack

	float game_portion = 1.f;
	float ui_portion = 0.2f;
	if (_toolbar) { // ui open
		game_portion -= ui_portion;
	}

	// lives
	const float char_width = ImGui::GetFontSize()*0.8f; // guess
	const float char_count = 8 + 3 + 3;
	const float lives_window_width_estimate = char_width * char_count;
	ImGui::SetNextWindowPos({display_w*game_portion*0.5f - lives_window_width_estimate*0.5f, 0});
	if (ImGui::Begin("lives", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {
		const auto& pl = scene.ctx<Components::PlayerLives>();
		ImGui::Text("lives: %ld/%ld", pl.lives, pl.max);
	}
	ImGui::End();

	if (_toolbar) {
		// ui (toolbar) to the right
		ImGui::SetNextWindowPos({display_w*(1.f - ui_portion), 0});
		ImGui::SetNextWindowSize({display_w*ui_portion, display_h});
		if (ImGui::Begin("toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar)) {
			if (ImGui::BeginTabBar("tabs")) {
				if (ImGui::BeginTabItem("Build")) {
					if (ImGui::Button("BuildTower")) {
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Settings")) {
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::End();
	}
}

} // mini_td::Services

