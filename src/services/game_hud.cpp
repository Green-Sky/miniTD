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
#include <mm/components/time_delta.hpp>

#include "../entities/spawn_group.hpp"
#include "../entities/tower.hpp"

#include <glm/common.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

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

	_last_time = clock::now();
	_screen_shake_time = 0.f;

	return true;
}

void GameHUD::disable(MM::Engine&) {
}

void GameHUD::update(MM::Engine& engine) {
	updateTimer();

	// TODO: move somewhere else? own update? a system?
	updateTowerPreview(engine);

	auto& ssi = engine.getService<MM::Services::SceneServiceInterface>();
	auto& scene = ssi.getScene();

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	//auto [display_w, display_h] = ImGui::GetIO().DisplaySize; // hack
	//auto [viewport_w, viewport_h] = viewport->WorkSize;

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
	ImGui::SetNextWindowPos({viewport->WorkSize.x*game_portion*0.5f - lives_window_width_estimate*0.5f, viewport->WorkPos.y});
	if (ImGui::Begin("lives", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {
		const auto& w = scene.ctx().get<Components::Wave>();
		if (w.final_wave >= 0) {
			ImGui::Text("wave: %ld/%ld", (long)w.wave, (long)w.final_wave);
		} else {
			ImGui::Text("wave: %ld/inf", (long)w.wave);
		}

		const auto& pl = scene.ctx().get<Components::PlayerLives>();
		ImGui::Text("lives: %ld/%ld", (long)pl.lives, (long)pl.max);

		const auto& m = scene.ctx().get<Components::Money>().count;
		ImGui::Text("money: %ld", (long)m);

		ImGui::Text("wave time: %lds", (long)w.time_into_wave);
	}
	ImGui::End();

	if (_toolbar) {
		// ui (toolbar) to the right
		ImGui::SetNextWindowPos({viewport->WorkSize.x*(1.f - ui_portion), viewport->WorkPos.y});
		ImGui::SetNextWindowSize({viewport->WorkSize.x*ui_portion, viewport->WorkSize.y});
		if (ImGui::Begin("toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar)) {
			const float lower_tools_height = 100.f;
			const float upper_tools_height = glm::max(ImGui::GetContentRegionAvail().y - lower_tools_height, 1.f);

			if (ImGui::BeginChild("upper_tools", {0, upper_tools_height})) {
				if (ImGui::BeginTabBar("tabs")) {
					if (ImGui::BeginTabItem("Build")) {
						renderToolbarBuild(engine);
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Stats")) {
						renderToolbarStats(engine);
						ImGui::EndTabItem();
					}
					if (debug && ImGui::BeginTabItem("Debug")) {
						renderToolbarDebug(engine);
						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}
			}
			ImGui::EndChild();

			if (ImGui::BeginChild("lower_tools", {0, lower_tools_height}, true)) {
				// TODO: -1?
				const float button_height = ImGui::GetContentRegionAvail().y;
				const float padding = 0.05f;

				const ImVec2 padded_size {
					button_height - button_height*padding*2.f,
					button_height - button_height*padding*2.f
				};

				{ // speed button, 1x and 4x ?
					auto& wave = scene.ctx().get<Components::Wave>();
					const bool wave_active = wave.active;
					auto& deltaFactor = scene.ctx().get<MM::Components::TimeDelta>().deltaFactor;
					const bool double_arrow = wave_active && deltaFactor != 1.f;

					const auto cp = ImGui::GetCursorScreenPos();
					// color yellow if in wave, green otherwise
					if (wave_active) {
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.65, 0.65, 0.15, 1));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8, 0.8, 0.3, 1));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 0.2, 1));
					} else {
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15, 0.65, 0.15, 1));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3, 0.8, 0.3, 1));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2, 1, 0.2, 1));
					}
					if (ImGui::Button("##play", {button_height, button_height})) {
						if (wave_active) { // if active toggle speed
							if (deltaFactor == 1.f) {
								deltaFactor = 3.f;
							} else {
								deltaFactor = 1.f;
							}
						} else {
							wave.start = true;
						}
					}
					ImGui::PopStyleColor(3);

					auto* dl = ImGui::GetWindowDrawList();

					const ImVec2 padded_cp {
						cp.x + button_height*padding,
						cp.y + button_height*padding
					};

					if (double_arrow) {
						const ImVec2 padded_center1 {
							padded_cp.x + padded_size.x*0.25f,
							padded_cp.y + padded_size.y*0.5f
						};

						dl->AddCircleFilled(
							padded_center1,
							padded_size.x * 0.25f,
							IM_COL32_WHITE,
							3
						);

						const ImVec2 padded_center2 {
							padded_cp.x + padded_size.x*0.75f,
							padded_cp.y + padded_size.y*0.5f
						};

						dl->AddCircleFilled(
							padded_center2,
							padded_size.x * 0.25f,
							IM_COL32_WHITE,
							3
						);
					} else {
						const ImVec2 padded_center {
							padded_cp.x + padded_size.x*0.5f,
							padded_cp.y + padded_size.y*0.5f
						};

						dl->AddCircleFilled(
							padded_center,
							padded_size.x * 0.5f,
							IM_COL32_WHITE,
							3
						);
					}
				}

				ImGui::SameLine();

				{ // auto button
					auto& auto_proceed = scene.ctx().get<Components::Wave>().auto_proceed;
					const auto cp = ImGui::GetCursorScreenPos();
					const bool active_col = auto_proceed;
					if (active_col) {
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15, 0.65, 0.15, 1));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3, 0.8, 0.3, 1));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2, 1, 0.2, 1));
					}
					if (ImGui::Button("##auto", {button_height, button_height})) {
						auto_proceed = !auto_proceed;
					}
					if (active_col) {
						ImGui::PopStyleColor(3);
					}

					auto* dl = ImGui::GetWindowDrawList();

					const ImVec2 padded_cp {
						cp.x + button_height*padding,
						cp.y + button_height*padding
					};

					const ImVec2 padded_center {
						padded_cp.x + padded_size.x*0.5f,
						padded_cp.y + padded_size.y*0.5f
					};

					const float circle_radius = padded_size.x*0.4f;

					// draw loop
					{ // draw circle
						dl->AddCircle(
							padded_center,
							circle_radius,
							IM_COL32_WHITE,
							0
						);
					}

					// draw 2 arrows
					//const float tip_angular_add = 0.3f;
					const float back_dist_add = 0.5f;
					const float back_spread = 0.5f;

					// HACK OMG
					static float angle_time = 0.f;
					if (auto_proceed) {
						angle_time += _delta;
						if (angle_time >= glm::two_pi<float>()) {
							angle_time -= glm::two_pi<float>();
						}
					}
					const float angle_1 = 0.f + angle_time;
					const float angle_2 = angle_1 + glm::two_pi<float>()* 1.f/3.f;
					const float angle_3 = angle_1 + glm::two_pi<float>()* 2.f/3.f;
					auto draw_arrow = [&](const float angle) {
						// tip
						ImVec2 tip {
							padded_center.x + glm::cos(angle) * circle_radius,
							padded_center.y + glm::sin(angle) * circle_radius,
						};

						// back 1
						ImVec2 back1 {
							tip.x + glm::cos(angle - glm::half_pi<float>() * (1.f - back_spread * 0.5f)) * circle_radius * back_dist_add,
							tip.y + glm::sin(angle - glm::half_pi<float>() * (1.f - back_spread * 0.5f)) * circle_radius * back_dist_add,
						};
						ImVec2 back2 {
							tip.x + glm::cos(angle - glm::half_pi<float>() * (1.f + back_spread)) * circle_radius * back_dist_add,
							tip.y + glm::sin(angle - glm::half_pi<float>() * (1.f + back_spread)) * circle_radius * back_dist_add,
						};
						dl->AddLine(back1, tip, IM_COL32_WHITE);
						dl->AddLine(back2, tip, IM_COL32_WHITE);
					};

					draw_arrow(angle_1);
					draw_arrow(angle_2);
					draw_arrow(angle_3);
				}
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}
}

void GameHUD::updateTimer(void) {
	auto newNow = clock::now();
	std::chrono::duration<double, std::ratio<1, 1>> deltaTime = newNow - _last_time;
	_last_time = newNow;

	_delta = deltaTime.count();
}

void GameHUD::updateCamera(MM::Scene& scene, float fractional_offset) {
	const auto& path = scene.ctx().get<Components::Path>();
	auto& cam = scene.ctx().get<MM::OpenGL::Camera3D>();

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
	const auto& trauma = scene.ctx().get<Components::CameraTrauma>().trauma;
	//const auto& trauma = 1.f;
	const float trauma_squared = trauma * trauma;

	// HACK
	_screen_shake_time += _delta;

	const auto& gc = scene.ctx().get<Components::GameConstants>();
	const float scale = 0.25f; // noise scale // TODO: expose to gc?

	cam.roll = gc.camera_shake_max_angle * trauma_squared * SquirrelNoise4::Compute1dPerlinNoise(_screen_shake_time, scale, 2u, 0.7f, 2.f, true, 103u);

	cam.translation.x += gc.camera_shake_max_offset * cam.horizontalViewPortSize * trauma_squared * SquirrelNoise4::Compute1dPerlinNoise(_screen_shake_time, scale, 2u, 0.7f, 2.f, true, 1021u);
	cam.translation.y += gc.camera_shake_max_offset * cam.horizontalViewPortSize * trauma_squared * SquirrelNoise4::Compute1dPerlinNoise(_screen_shake_time, scale, 2u, 0.7f, 2.f, true, 3533u);


	// update
	cam.setOrthographic();
	cam.updateView();
}

void GameHUD::renderToolbarBuild(MM::Engine& engine) {
	auto& ssi = engine.getService<MM::Services::SceneServiceInterface>();
	auto& scene = ssi.getScene();

	const bool enough_money = scene.ctx().get<Components::Money>().count >= 100;
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

}

void GameHUD::renderToolbarStats(MM::Engine& engine) {
	auto& ssi = engine.getService<MM::Services::SceneServiceInterface>();
	auto& scene = ssi.getScene();

	ImGui::Text("TODO");
}

void GameHUD::renderToolbarDebug(MM::Engine& engine) {
	auto& ssi = engine.getService<MM::Services::SceneServiceInterface>();
	auto& scene = ssi.getScene();

	ImGui::InputScalar("money", ImGuiDataType_S64, &scene.ctx().get<Components::Money>().count);
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
		tp.time_accu += _delta;
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
		const auto& cam = scene.ctx().get<MM::OpenGL::Camera3D>();

		const auto mouse_pos = ImGui::GetMousePos();
		const auto window_size = ImGui::GetIO().DisplaySize;

		// TODO: implement with glm::unProject ?
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
		const auto& world_extent = scene.ctx().get<Components::Path>().extents;
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

			scene.ctx().get<Components::Money>().count -= 100;
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

