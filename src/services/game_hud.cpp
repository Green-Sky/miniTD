#include "./game_hud.hpp"

//#include <mm/services/screen_director.hpp>
#include <mm/services/scene_service_interface.hpp>

#include "../components/player_lives.hpp"

#include "../entities/spawn_group.hpp"

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
					ImGui::TextUnformatted("Particle Cannon");
					drawTower(true, false, {60, 60});
					ImGui::SameLine();
					ImGui::TextUnformatted(
						" - range: long\n"
						" - projectile:\n"
						"   - small\n"
						"   - fast\n"
						"   - high dmg (?)\n"
						" - heat decay: low\n"
					);

					ImGui::Separator();

					ImGui::TextUnformatted("Neutrino Bomber");
					drawTower(false, false, {60, 60});
					ImGui::SameLine();
					ImGui::TextUnformatted(
						" - range: short-ish\n"
						" - projectile:\n"
						"   - large\n"
						"   - slow\n"
						"   - high dmg\n"
						" - heat decay: low\n"
					);

					ImGui::Separator();

					ImGui::TextUnformatted("Burst Laser");
					drawTower(false, true, {60, 60});
					ImGui::SameLine();
					ImGui::TextUnformatted(
						" - range: short-ish\n"
						" - ray:\n"
						"   - low dmg\n"
						" - heat decay: high\n"
					);

					ImGui::Separator();

					ImGui::TextUnformatted("X Laser");
					drawTower(true, true, {60, 60});
					ImGui::SameLine();
					ImGui::TextUnformatted(
						" - range: long (very)\n"
						" - ray:\n"
						"   - high dmg\n"
						" - heat decay: low\n"
					);

					if (ImGui::Button("BuildTower")) {
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Settings")) {
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Debug")) {
					if (ImGui::TreeNode("Spawn SpawnGroup")) {
						static int64_t level = 1;
						ImGui::InputScalar("level", ImGuiDataType_S64, &level);
						static uint64_t count = 10;
						ImGui::InputScalar("count", ImGuiDataType_U64, &count);
						static float interval = 0.4f;
						ImGui::InputScalar("interval", ImGuiDataType_Float, &interval);
						static float initial_time_accu = 0.f;
						ImGui::InputScalar("inital_time_accu", ImGuiDataType_Float, &initial_time_accu);
						if (ImGui::Button("spawn")) {
							Entities::spawn_spawn_group(scene, level, count, interval, initial_time_accu);
						}
						ImGui::TreePop();
					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::End();
	}
}

// TODO: rendering is a hack
void GameHUD::drawTower(bool outer, bool inner, const ImVec2& size) {
	auto* dl = ImGui::GetWindowDrawList();
	const auto& cp = ImGui::GetCursorScreenPos();

	// outer
	dl->AddCircle({cp.x + size.x*0.5f, cp.y + size.y*0.5f}, size.x*0.5f, IM_COL32_WHITE, outer ? 4 : 0);

	// inner
	if (inner) {
		// cross
		dl->AddLine({cp.x + size.x*0.5f, cp.y}, {cp.x + size.x*0.5f, cp.y + size.y}, IM_COL32_WHITE);
		dl->AddLine({cp.x, cp.y + size.y*0.5f}, {cp.x + size.x, cp.y + size.y*0.5f}, IM_COL32_WHITE);
	} else {
		dl->AddCircle({cp.x + size.x*0.5f, cp.y + size.y*0.5f}, size.x*0.5f*0.333f, IM_COL32_WHITE);
	}

	ImGui::Dummy(size);
}

} // mini_td::Services

