#include "./game_hud.hpp"

//#include <mm/services/screen_director.hpp>
#include <mm/services/scene_service_interface.hpp>

#include "../components/wave.hpp"
#include "../components/player_lives.hpp"
#include "../components/money.hpp"
#include <mm/components/position2d.hpp>
#include "../components/tower_art.hpp"
#include "../components/tower_preview.hpp"
#include "../components/path.hpp"
#include <mm/opengl/camera_3d.hpp>
#include "../components/camera_trauma.hpp"
#include "../components/game_constants.hpp"

#include "../entities/spawn_group.hpp"
#include "../entities/tower.hpp"

#include <glm/common.hpp>

#include <squirrel_noise/SmoothNoise.hpp>

#include <entt/entity/registry.hpp>

#include <imgui/imgui.h>

#include <mm/logger.hpp>

namespace mini_td::Services {

bool GameHUD::enable(MM::Engine&, std::vector<MM::UpdateStrategies::TaskInfo>& task_array) {
	// setup tasks
	task_array.push_back(
		MM::UpdateStrategies::TaskInfo{"GameHUD::update"}
		.fn([this](MM::Engine& e){ update(e); })
		.phase(MM::UpdateStrategies::update_phase_t::PRE)
		.succeed("ImGuiService::new_frame")
	);

	return true;
}

void GameHUD::disable(MM::Engine&) {
}

void GameHUD::update(MM::Engine& engine) {
	// TODO: move somewhere else? own update? a system?
	updateTowerPreview(engine);

	auto& ssi = engine.getService<MM::Services::SceneServiceInterface>();
	auto& scene = ssi.getScene();

	auto [display_w, display_h] = ImGui::GetIO().DisplaySize; // hack

	float game_portion = 1.f;
	float ui_portion = 0.2f;
	if (_toolbar) { // ui open
		game_portion -= ui_portion;
	}

	updateCamera(scene, ui_portion);

	// lives and money
	const float char_width = ImGui::GetFontSize()*0.8f; // guess
	const float char_count = 8 + 3 + 3;
	const float lives_window_width_estimate = char_width * char_count;
	ImGui::SetNextWindowPos({display_w*game_portion*0.5f - lives_window_width_estimate*0.5f, 0});
	if (ImGui::Begin("lives", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {
		const auto& w = scene.ctx<Components::Wave>().wave;
		ImGui::Text("wave: %ld", w);

		const auto& pl = scene.ctx<Components::PlayerLives>();
		ImGui::Text("lives: %ld/%ld", pl.lives, pl.max);

		const auto& m = scene.ctx<Components::Money>().count;
		ImGui::Text("money: %ld", m);
	}
	ImGui::End();

	if (_toolbar) {
		// ui (toolbar) to the right
		ImGui::SetNextWindowPos({display_w*(1.f - ui_portion), 0});
		ImGui::SetNextWindowSize({display_w*ui_portion, display_h});
		if (ImGui::Begin("toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar)) {
			if (ImGui::BeginTabBar("tabs")) {
				if (ImGui::BeginTabItem("Build")) {
					const bool enough_money = scene.ctx<Components::Money>().count >= 100;
					if (enough_money) {
						// green-ish
						//ImGui::PushStyleColor(ImGuiCol_Button, {1.f, 1.f, 1.f, 0.4f});
						//ImGui::PushStyleColor(ImGuiCol_ButtonActive, {1.f, 1.f, 1.f, 0.4f});
						//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {1.f, 1.f, 1.f, 0.4f});
					} else {
						// gray-ish
						const auto disabled_col = ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled);
						ImGui::PushStyleColor(ImGuiCol_Button, disabled_col);
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, disabled_col);
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disabled_col);
					}

					if (towerButton("##pc", true, false, {60, 60}) && enough_money) {
						if (scene.valid(_tower_placement_preview)) {
							scene.destroy(_tower_placement_preview);
						}
						_tower_placement_preview = scene.create();

						scene.emplace<MM::Components::Position2D>(_tower_placement_preview);
						auto& tp = scene.emplace<Components::TowerPreview>(_tower_placement_preview);
						tp.art = 3;
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

					if (towerButton("##nb", false, false, {60, 60}) && enough_money) {
						if (scene.valid(_tower_placement_preview)) {
							scene.destroy(_tower_placement_preview);
						}
						_tower_placement_preview = scene.create();

						scene.emplace<MM::Components::Position2D>(_tower_placement_preview);
						auto& tp = scene.emplace<Components::TowerPreview>(_tower_placement_preview);
						tp.art = 2;
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


					if (towerButton("##bl", false, true, {60, 60}) && enough_money) {
						if (scene.valid(_tower_placement_preview)) {
							scene.destroy(_tower_placement_preview);
						}
						_tower_placement_preview = scene.create();

						scene.emplace<MM::Components::Position2D>(_tower_placement_preview);
						auto& tp = scene.emplace<Components::TowerPreview>(_tower_placement_preview);
						tp.art = 1;
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

					if (towerButton("##xl", true, true, {60, 60}) && enough_money) {
						if (scene.valid(_tower_placement_preview)) {
							scene.destroy(_tower_placement_preview);
						}
						_tower_placement_preview = scene.create();

						scene.emplace<MM::Components::Position2D>(_tower_placement_preview);
						auto& tp = scene.emplace<Components::TowerPreview>(_tower_placement_preview);
						tp.art = 4;
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

					if (!enough_money) {
						ImGui::PopStyleColor(3);
					}

					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Stats")) {
					ImGui::Text("TODO");
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Debug")) {
					ImGui::InputScalar("money", ImGuiDataType_S64, &scene.ctx<Components::Money>().count);
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

void GameHUD::updateCamera(MM::Scene& scene, float fractional_offset) {
	const auto& path = scene.ctx<Components::Path>();
	auto& cam = scene.ctx<MM::OpenGL::Camera3D>();

	// center cam on map and
	// offset pos by fraction of width
	cam.translation = glm::vec3{
		path.extents.x * (0.5f + fractional_offset),
		path.extents.y * 0.5f,
		0.f
	};

	// fit map
	cam.horizontalViewPortSize = glm::max(path.extents.x, path.extents.y) * cam.screenRatio;

	// apply screen shake
	const auto& trauma = scene.ctx<Components::CameraTrauma>().trauma;
	//const auto& trauma = 1.f;
	const float trauma_squared = trauma * trauma;

	// HACK
	static float time = 0.f;
	time += 1.f/144.f;

	const auto& gc = scene.ctx<Components::GameConstants>();
	const float scale = 0.25f; // noise scale // TODO: expose to gc?

	cam.roll = gc.camera_shake_max_angle * trauma_squared * SquirrelNoise4::Compute1dPerlinNoise(time, scale, 2u, 0.7f, 2.f, true, 103u);

	cam.translation.x += gc.camera_shake_max_offset * cam.horizontalViewPortSize * trauma_squared * SquirrelNoise4::Compute1dPerlinNoise(time, scale, 2u, 0.7f, 2.f, true, 1021u);
	cam.translation.y += gc.camera_shake_max_offset * cam.horizontalViewPortSize * trauma_squared * SquirrelNoise4::Compute1dPerlinNoise(time, scale, 2u, 0.7f, 2.f, true, 3533u);


	// update
	cam.setOrthographic();
	cam.updateView();
}

// this is hacky
void GameHUD::updateTowerPreview(MM::Engine& engine) {
	auto& ssi = engine.getService<MM::Services::SceneServiceInterface>();
	auto& scene = ssi.getScene();

	if (!scene.valid(_tower_placement_preview)) {
		return;
	}

	{ // blink
		auto& tp = scene.get<Components::TowerPreview>(_tower_placement_preview);
		//tp.time_accu += 1.f/100.f; // TODO: actual delta
		tp.time_accu += 1.f/ImGui::GetIO().Framerate; // hack
		if (tp.time_accu >= 0.4f) {
			tp.time_accu = 0.f;
			if (scene.any_of<
				Components::TowerArtType1,
				Components::TowerArtType2,
				Components::TowerArtType3,
				Components::TowerArtType4
			>(_tower_placement_preview)) {
				scene.remove<
					Components::TowerArtType1,
					Components::TowerArtType2,
					Components::TowerArtType3,
					Components::TowerArtType4
				>(_tower_placement_preview);
			} else {
				// man this is disgusting
				switch (tp.art) {
					case 1: scene.emplace<Components::TowerArtType1>(_tower_placement_preview); break;
					case 2: scene.emplace<Components::TowerArtType2>(_tower_placement_preview); break;
					case 3: scene.emplace<Components::TowerArtType3>(_tower_placement_preview); break;
					case 4: scene.emplace<Components::TowerArtType4>(_tower_placement_preview); break;
					default: SPDLOG_ERROR("eh, invalid art {}", tp.art);
				}
			}
		}
	}

	{ // update position
		auto& e_pos = scene.get<MM::Components::Position2D>(_tower_placement_preview);
		const auto& cam = scene.ctx<MM::OpenGL::Camera3D>();

		const auto mouse_pos = ImGui::GetMousePos();
		const auto window_size = ImGui::GetIO().DisplaySize;

		// TODO: implement with glm::unProject ?
		// TODO: add to mm cam
		// get screen cords in NDC space [-1, 1]
		const glm::vec4 screen {
			(
				glm::vec2{mouse_pos.x, window_size.y - mouse_pos.y}
				/ (glm::vec2{window_size.x, window_size.y} * 0.5f)
			)
			- glm::vec2{1.f, 1.f}
			, 1.f, 1.f
		};
		const glm::vec4 world = cam.screenToWorld(screen);

		// set and clamp into world
		const auto& world_extent = scene.ctx<Components::Path>().extents;
		e_pos.pos = glm::clamp(glm::vec2{world.x, world.y}, {0, 0}, world_extent);
	}

	{ // place
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
			SPDLOG_ERROR("the click");

			const auto& pos = scene.get<MM::Components::Position2D>(_tower_placement_preview).pos;
			const auto& tp = scene.get<Components::TowerPreview>(_tower_placement_preview);
			// this is a real hack
			switch (tp.art) {
				case 1: Entities::spawn_tower_type3(scene, pos); break;
				case 2: Entities::spawn_tower_type2(scene, pos); break;
				case 3: Entities::spawn_tower_type1(scene, pos); break;
				case 4: Entities::spawn_tower_type4(scene, pos); break;
				default: SPDLOG_ERROR("eh, invalid type {}", tp.art);
			}

			scene.destroy(_tower_placement_preview);
			_tower_placement_preview = entt::null;

			scene.ctx<Components::Money>().count -= 100;
		}
	}
}

// TODO: rendering is a hack
void GameHUD::drawTower(bool outer, bool inner, const ImVec2& size, const ImVec2& pos) const {
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

bool GameHUD::towerButton(const char* title, bool outer, bool inner, const ImVec2& size) const {
	const auto cp = ImGui::GetCursorScreenPos();
	const bool pressed = ImGui::Button(title, size);
	drawTower(outer, inner, size, cp);
	return pressed;
}

} // mini_td::Services

