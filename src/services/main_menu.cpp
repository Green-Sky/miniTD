#include "./main_menu.hpp"

#include <mm/services/screen_director.hpp>
#include <mm/services/filesystem.hpp>
#include <mm/services/organizer_scene.hpp>

#include "../game_scene.hpp"

#include <entt/entity/registry.hpp>

#include <imgui/imgui.h>

#include <mm/logger.hpp>

namespace mini_td::Services {

static bool MyButton(const char* title) {
	return ImGui::Button(title, ImVec2(0, 25));
}

constexpr auto menu_window_flags =
	ImGuiWindowFlags_AlwaysAutoResize |
	ImGuiWindowFlags_NoCollapse
	//ImGuiWindowFlags_NoMove
;

bool MainMenu::enable(MM::Engine& engine, std::vector<MM::UpdateStrategies::TaskInfo>& task_array) {
	// setup tasks
	task_array.push_back(
		MM::UpdateStrategies::TaskInfo{"MainMenu::render"}
		.fn([this](MM::Engine& e){ render(e); })
		.phase(MM::UpdateStrategies::update_phase_t::PRE)
		.succeed("ImGuiService::new_frame")
	);

	// load missions
	auto& fs = engine.getService<MM::Services::FilesystemService>();
	const std::string missions_folder {"/missions/"};
	if (!fs.isDir(missions_folder.c_str())) {
		SPDLOG_ERROR("missing missions folder {}", missions_folder);
		return false;
	}

	_missions.clear();

	std::vector<std::string> mission_files {};
	fs.forEachIn(missions_folder.c_str(), [&mission_files, &missions_folder](const char* file_path) -> bool {
		mission_files.push_back(missions_folder + file_path);
		return true;
	});

	for (const auto& mission_file : mission_files) {
		if (fs.isFile(mission_file.c_str())) {
			try {
				_missions.push_back(fs.readJson(mission_file.c_str()));
			} catch (...) {
				SPDLOG_ERROR("error parsing mission {}", mission_file);
			}
		} else {
			SPDLOG_ERROR("not file in missions_folder {}", mission_file);
		}
	}


	return true;
}

void MainMenu::disable(MM::Engine&) {
}

void MainMenu::render(MM::Engine& engine) {
	switch (_state) {
		case State::MAIN_MENU:
			renderMainMenu(engine);
			break;
		case State::MISSIONS:
			renderMissions(engine);
			break;
		case State::SETTINGS:
			renderSettings(engine);
			break;
	}
}

void MainMenu::renderMainMenu(MM::Engine& engine) {
	if (ImGui::Begin("MainMenu", nullptr, menu_window_flags)) {
		if (MyButton("Play!")) {
			_state = State::MISSIONS;
		}

		if (MyButton("Settings")) {
			_state = State::SETTINGS;
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.65, 0.15, 0.15, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8, 0.3, 0.3, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 0.2, 0.2, 1));
		if (MyButton("Quit")) {
			engine.stop();
		}
		ImGui::PopStyleColor(3);
	}
	ImGui::End();
}

void MainMenu::renderMissions(MM::Engine& engine) {
	if (ImGui::Begin("Missions", nullptr, menu_window_flags)) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.65, 0.15, 0.15, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8, 0.3, 0.3, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 0.2, 0.2, 1));
		if (MyButton("<- Back")) {
			_state = State::MAIN_MENU;
		}
		ImGui::PopStyleColor(3);
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		for (const auto& mission : _missions) {
			if (MyButton(mission.title.c_str())) {
				engine.getService<MM::Services::OrganizerSceneService>().changeSceneNow(create_game_scene(engine, mission));
				engine.getService<MM::Services::ScreenDirector>().queueChangeScreenTo("mini_td::Screens::game_sp");
			}
		}
	}
	ImGui::End();
}

void MainMenu::renderSettings(MM::Engine& engine) {
	if (ImGui::Begin("Settings", nullptr, menu_window_flags)) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.65, 0.15, 0.15, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8, 0.3, 0.3, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 0.2, 0.2, 1));
		if (MyButton("<- Back")) {
			_state = State::MAIN_MENU;
		}
		ImGui::PopStyleColor(3);
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("TODO");

		if (ImGui::Button("enable debug")) {
			engine.getService<MM::Services::ScreenDirector>().queueChangeScreenTo("mini_td::Screens::enable_debug");
		}
	}
	ImGui::End();
}

} // mini_td::Services

