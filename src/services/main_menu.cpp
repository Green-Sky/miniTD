#include "./main_menu.hpp"

#include <mm/services/screen_director.hpp>

#include <imgui/imgui.h>

namespace mini_td::Services {

bool MainMenu::enable(MM::Engine& engine, std::vector<MM::UpdateStrategies::TaskInfo>& task_array) {
	// setup tasks
	task_array.push_back(
		MM::UpdateStrategies::TaskInfo{"MainMenu::render"}
		.fn([this](MM::Engine& e){ render(e); })
		.phase(MM::UpdateStrategies::update_phase_t::PRE)
		.succeed("ImGuiService::new_frame")
	);

	return true;
}

void MainMenu::disable(MM::Engine&) {
}

void MainMenu::render(MM::Engine& engine) {
	if (ImGui::Begin("MainMenu")) {
		if (ImGui::Button("start game hax")) {
			engine.getService<MM::Services::ScreenDirector>().queueChangeScreenTo("mini_td::Screens::game_sp");
		}

		if (ImGui::Button("Settings")) {
		}
	}
	ImGui::End();
}

} // mini_td::Services

