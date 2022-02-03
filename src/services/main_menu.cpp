#include "./main_menu.hpp"

#include <mm/services/screen_director.hpp>
#include <mm/services/filesystem.hpp>

#include <imgui/imgui.h>

#include <mm/logger.hpp>

namespace mini_td::Services {

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
	if (false) {
		renderMainMenu(engine);
	} else {
		renderMissions(engine);
	}
}

void MainMenu::renderMainMenu(MM::Engine& engine) {
	if (ImGui::Begin("MainMenu")) {
		if (ImGui::Button("start game hax")) {
			engine.getService<MM::Services::ScreenDirector>().queueChangeScreenTo("mini_td::Screens::game_sp");
		}

		if (ImGui::Button("Settings")) {
		}
	}
	ImGui::End();
}

void MainMenu::renderMissions(MM::Engine& engine) {
	if (ImGui::Begin("Missions")) {
		for (const auto& mission : _missions) {
			if (ImGui::Button(mission.title.c_str())) {
				engine.getService<MM::Services::ScreenDirector>().queueChangeScreenTo("mini_td::Screens::game_sp");
			}
		}
	}
	ImGui::End();
}

} // mini_td::Services

