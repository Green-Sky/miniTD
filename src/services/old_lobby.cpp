#include "./lobby.hpp"

#include <mm/services/net_channeled_interface.hpp>

#include <mm/logger.hpp>
#include <string_view>

namespace Services {

bool Lobby::enable(MM::Engine& engine, std::vector<MM::UpdateStrategies::TaskInfo>& task_array) {
	auto* net = engine.tryService<MM::Services::NetChanneledInterface>();
	if (net == nullptr) {
		SPDLOG_ERROR("lobby needs NetChanneledInterface");
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

void Lobby::disable(MM::Engine& engine) {
	// *shrug*
}

void Lobby::update(MM::Engine& engine) {
	auto* net = engine.tryService<MM::Services::NetChanneledInterface>();

	// should reset net interface
	if (_next_join_info.has_value()) {
		if (net != nullptr) {
			// TODO: send disconnect?
			net->clearPackets();
			net->clearPeerlist();
			SPDLOG_INFO("resetting net '{}'", net->name());
		}

		if (engine.provide(engine.type<MM::Services::NetChanneledInterface>(), _next_join_info->net_interface)) {
			net = engine.tryService<MM::Services::NetChanneledInterface>();
			_current_join_info = _next_join_info;
			_next_join_info.reset();
		} else {
			net = nullptr;
		}
	}

	switch (state) {
		case State::NONE: break;

		case State::CLIENT_LOBBY: break;
		case State::CLIENT_LOADING: break;
		case State::CLIENT_RUNNING: break;

		case State::HOST_OPEN: break;
		case State::HOST_LOADING: break;
		case State::HOST_RUNNING: break;
	}
}

bool Lobby::handleJoin(std::string_view scheme, std::string_view rest_of_uri) {
	assert(!scheme.empty());
	// rest_of_uri might be empty

	if (!_uri_handlers.count(std::string{scheme})) {
		SPDLOG_ERROR("unknown scheme '{}'", scheme);
		return false;
	}

	// set "reset net interface"
	_next_join_info = {
		_uri_handlers.at(std::string{scheme}),
		std::string{rest_of_uri}
	};

	return true;
}

Lobby::Lobby(void) {
}

bool Lobby::registerHandler(std::string_view uri_scheme, MM::Engine::service_id_type service_id) {
	_uri_handlers[std::string{uri_scheme}] = service_id;
	return true; // TODO: error checking
}

bool Lobby::join(std::string_view uri) {
	SPDLOG_INFO("join lobby with uri:'{}'", uri);
	// "scheme:f"
	//  01234567
	// size == 8

	auto pos = uri.find_first_of(":");
	if (pos == uri.npos || pos == 0) {
		SPDLOG_ERROR("uri missing scheme '{}'", uri);
		return false;
	}

	auto scheme = uri.substr(0, pos);
	std::string_view rest_of_uri {};

	// if scheme is not full uri (allowed)
	if (pos+1 < uri.size()) {
		rest_of_uri = uri.substr(pos+1);
	}

	return handleJoin(scheme, rest_of_uri);
}

} // Services

