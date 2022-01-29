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
					if (towerButton("##pc", true, false, {60, 60})) {
					}
					if (ImGui::IsItemHovered()) {
						ImGui::BeginTooltip();

						ImGui::TextUnformatted("Particle Cannon");
						drawTower(true, false, {60, 60}, ImGui::GetCursorScreenPos());
						ImGui::Dummy({60, 60});
						ImGui::SameLine();
						ImGui::TextUnformatted(
							" - range: long\n"
							" - projectile:\n"
							"   - small\n"
							"   - fast\n"
							"   - low dmg\n"
							" - heat decay: low\n"
						);

						ImGui::EndTooltip();
					}

					ImGui::SameLine();

					if (towerButton("##nb", false, false, {60, 60})) {
					}
					if (ImGui::IsItemHovered()) {
						ImGui::BeginTooltip();
						ImGui::TextUnformatted("Neutrino Bomber");
						drawTower(false, false, {60, 60}, ImGui::GetCursorScreenPos());
						ImGui::Dummy({60, 60});
						ImGui::SameLine();
						ImGui::TextUnformatted(
							" - range: short-ish\n"
							" - projectile:\n"
							"   - large\n"
							"   - slow\n"
							"   - high dmg\n"
							" - heat decay: low\n"
						);
						ImGui::EndTooltip();
					}


					if (towerButton("##bl", false, true, {60, 60})) {
					}
					if (ImGui::IsItemHovered()) {
						ImGui::BeginTooltip();
						ImGui::TextUnformatted("Burst Laser");
						drawTower(false, true, {60, 60}, ImGui::GetCursorScreenPos());
						ImGui::Dummy({60, 60});
						ImGui::SameLine();
						ImGui::TextUnformatted(
							" - range: short-ish\n"
							" - ray:\n"
							"   - low dmg\n"
							" - heat decay: high\n"
						);
						ImGui::EndTooltip();
					}

					ImGui::SameLine();

					if (towerButton("##xl", true, true, {60, 60})) {
					}
					if (ImGui::IsItemHovered()) {
						ImGui::BeginTooltip();
						ImGui::TextUnformatted("X Laser");
						drawTower(true, true, {60, 60}, ImGui::GetCursorScreenPos());
						ImGui::Dummy({60, 60});
						ImGui::SameLine();
						ImGui::TextUnformatted(
							" - range: long (very)\n"
							" - ray:\n"
							"   - high dmg\n"
							" - heat decay: low\n"
						);
						ImGui::EndTooltip();
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
void GameHUD::drawTower(bool outer, bool inner, const ImVec2& size, const ImVec2& pos) {
	auto* dl = ImGui::GetWindowDrawList();
	//const auto& cp = ImGui::GetCursorScreenPos();

	const float padding = 0.05f;

	const ImVec2 padded_size {
		size.x - size.x*padding*2.f,
		size.y - size.y*padding*2.f
	};

	const ImVec2 padded_cp {
		pos.x + size.x*padding,
		pos.y + size.y*padding
	};


	// outer
	dl->AddCircle(
		{padded_cp.x + padded_size.x*0.5f, padded_cp.y + padded_size.y*0.5f},
		padded_size.x*0.5f,
		IM_COL32_WHITE,
		outer ? 4 : 0
	);

	// inner
	if (inner) {
		// cross
		dl->AddLine(
			{padded_cp.x + padded_size.x*0.5f, padded_cp.y},
			{padded_cp.x + padded_size.x*0.5f, padded_cp.y + padded_size.y},
			IM_COL32_WHITE
		);
		dl->AddLine(
			{padded_cp.x, padded_cp.y + padded_size.y*0.5f},
			{padded_cp.x + padded_size.x, padded_cp.y + padded_size.y*0.5f},
			IM_COL32_WHITE
		);
	} else {
		dl->AddCircle(
			{padded_cp.x + padded_size.x*0.5f, padded_cp.y + padded_size.y*0.5f},
			padded_size.x*0.5f*0.333f,
			IM_COL32_WHITE
		);
	}

	// frame
	dl->AddRect(
		pos,
		{pos.x + size.x, pos.y + size.y},
		IM_COL32_WHITE
	);
}

bool GameHUD::towerButton(const char* title, bool outer, bool inner, const ImVec2& size) {
	const auto cp = ImGui::GetCursorScreenPos();
	const bool pressed = ImGui::Button(title, size);
	drawTower(outer, inner, size, cp);
	return pressed;
}

} // mini_td::Services

