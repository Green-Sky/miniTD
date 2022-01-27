#pragma once

#include <mm/services/service.hpp>

namespace mini_td::Services {

class MainMenu : public MM::Services::Service {
	private: // service interface
		const char* name(void) override { return "mini_td::MainMenu"; }

		bool enable(MM::Engine& engine, std::vector<MM::UpdateStrategies::TaskInfo>& task_array) override;
		void disable(MM::Engine& engine) override;

	private: // tasks
		void render(MM::Engine& engine);
};

} // mini_td::Services

