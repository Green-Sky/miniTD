#pragma once

#include <mm/engine.hpp>

#include <nlohmann/json_fwd.hpp>

#include <optional>
#include <unordered_map>

namespace Services {

// relies on net
class LobbyHost : public MM::Services::Service {
	private: // service
		const char* name(void) override { return "Lobby"; }

		bool enable(MM::Engine& engine, std::vector<MM::UpdateStrategies::TaskInfo>& task_array) override;
		void disable(MM::Engine& engine) override;

		void update(MM::Engine& engine);

	public:
		LobbyHost(void) = default;

	public: // host
		// host lobby, dont touch net interface
		bool host(void);
		// host lobby, change net interface
		bool host(MM::Engine::service_id_type service_id);

		// idk
		void hostSetMeta(const nlohmann::json& meta_json);
};

} // Services

