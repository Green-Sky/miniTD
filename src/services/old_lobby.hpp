#pragma once

#include <mm/engine.hpp>

#include <nlohmann/json_fwd.hpp>

#include <optional>
#include <unordered_map>

namespace Services {

// relies on net
class Lobby : public MM::Services::Service {
	public:
		enum State {
			NONE,

			RUNNING,
			LOADING,
		};

	private:
		State state {State::NONE};

		enum PKGType : uint8_t {
			JOIN_REQ = 0, // request join, might contain a secret?
			JOIN_ACK, // tell the client it can join

			META = 10, // ?
			META_JSON, // ?

			LOAD = 20,
			LOAD_ACK, // LOAD_DONE?

			START = 30,
			START_ACK,
		};

		std::unordered_map<std::string, MM::Engine::service_id_type> _uri_handlers;

		struct JoinInfo {
			MM::Engine::service_id_type net_interface;
			std::string rest_of_uri;
		};
		std::optional<JoinInfo> _next_join_info;
		std::optional<JoinInfo> _current_join_info;

	private: // service
		const char* name(void) override { return "Lobby"; }

		bool enable(MM::Engine& engine, std::vector<MM::UpdateStrategies::TaskInfo>& task_array) override;
		void disable(MM::Engine& engine) override;

		void update(MM::Engine& engine);

	private:
		bool handleJoin(std::string_view scheme, std::string_view rest_of_uri);

	public:
		Lobby(void);

	public: // handler
		bool registerHandler(std::string_view uri_scheme, MM::Engine::service_id_type service_id);

		template<typename Service>
		bool registerHandler(std::string_view uri_scheme, MM::Engine& engine) {
			return registerHandler(uri_scheme, engine.type<Service>());
		}

	public: // client
		// eg. tox:<TOXPUBID>
		// returns true if successfully queue
		bool join(std::string_view uri);

	public: // host
		// host lobby, dont touch net interface
		bool host(void);
		// host lobby, change net interface
		bool host(MM::Engine::service_id_type service_id);

		// idk
		void hostSetMeta(const nlohmann::json& meta_json);
};

} // Services

