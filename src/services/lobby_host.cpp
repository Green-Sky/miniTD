#include "./lobby_host.hpp"

#include <mm/services/net_channeled_interface.hpp>

#include <mm/logger.hpp>
#include <string_view>

namespace Services {

bool LobbyHost::enable(MM::Engine& engine, std::vector<MM::UpdateStrategies::TaskInfo>& task_array) {
	auto* net = engine.tryService<MM::Services::NetChanneledInterface>();
	if (net == nullptr) {
		SPDLOG_ERROR("LobbyHost needs NetChanneledInterface");
		return false;
	}

	// setup tasks
	task_array.push_back(
		MM::UpdateStrategies::TaskInfo{"Lobby::update"}
		.fn([this](MM::Engine& e){ update(e); })
		.phase(MM::UpdateStrategies::update_phase_t::PRE)
		//.succeed("ImGuiService::new_frame")
	);


	return true;
}

void LobbyHost::disable(MM::Engine& engine) {
	// *shrug*
}

void LobbyHost::update(MM::Engine& engine) {
	auto* net = engine.tryService<MM::Services::NetChanneledInterface>();
}

} // Services

