#pragma once

#include <mm/services/service.hpp>

#include "../mission.hpp"

namespace mini_td::Services {

class MainMenu : public MM::Services::Service {
	private: // service interface
		const char* name(void) override { return "mini_td::MainMenu"; }

		bool enable(MM::Engine& engine, std::vector<MM::UpdateStrategies::TaskInfo>& task_array) override;
		void disable(MM::Engine& engine) override;

	private: // tasks
		void render(MM::Engine& engine);
		void renderMainMenu(MM::Engine& engine);
		void renderMissions(MM::Engine& engine);
		void renderSettings(MM::Engine& engine);

	private: // mission
		std::vector<Mission1> _missions {};

		enum class State {
			MAIN_MENU,
			MISSIONS,
			SETTINGS
		};
		State _state {State::MAIN_MENU};
};

} // mini_td::Services

