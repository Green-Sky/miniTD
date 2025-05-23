#include <mm/engine.hpp>
#include <mm/logger.hpp>
#include <spdlog/cfg/env.h>

#include "./setup_services.hpp"

#include <mm/services/sound_service.hpp>
#include <mm/services/opengl_renderer.hpp>
#include <mm/services/screen_director.hpp>
#include <mm/services/imgui_s.hpp>
#include <mm/services/organizer_scene.hpp>
#include "./services/main_menu.hpp"
#include "./services/game_hud.hpp"
#include "./services/lobby_host.hpp"
#include "./services/lobby_client.hpp"

#include <mm/services/imgui_menu_bar.hpp>
#include <mm/services/engine_tools.hpp>
#include <mm/services/opengl_renderer_tools.hpp>
#include <mm/services/screen_director_tools.hpp>
#include <mm/services/scene_tools.hpp>
#include <mm/services/sound_tools.hpp>

#include <mm_tox/services/tox_service.hpp>
#include <mm_tox/services/tox_chat.hpp>
#include <mm_tox/services/tox_net_channeled.hpp>

#include "./setup_meta.hpp"

#include <entt/entity/registry.hpp>

template<>
bool setup_service<MM::Tox::Services::ToxService>(MM::Engine& engine, [[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
	auto& tox_service = engine.addService<MM::Tox::Services::ToxService>(engine, "save.tox");

	tox_service._app_name = "miniTD";

	if (!engine.enableService<MM::Tox::Services::ToxService>()) {
		SPDLOG_ERROR("failed to enable service {}", engine.getService<MM::Tox::Services::ToxService>().name());
		return false;
	}

	return true;
}

template<>
bool setup_service<Services::LobbyClient>(MM::Engine& engine, [[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
	auto& service = engine.addService<Services::LobbyClient>();

	// ???
	service.registerHandler<MM::Tox::Services::ToxNetChanneled>("tox", engine);

	// dont auto enable
	//if (!engine.enableService<Services::LobbyClient>()) {
		//SPDLOG_ERROR("failed to enable service {}", ((MM::Services::Service*)engine.tryService<Services::LobbyClient>())->name());
		//return false;
	//}

	return true;
}

bool setup_engine(MM::Engine& engine, int argc, char** argv) {
	return setup_engine_t<
		MM::Services::SDLService, // enabled by default
		MM::Services::FilesystemService, // enabled by default

		MM::Services::SoundService,

		MM::Services::OpenGLRenderer,

		MM::Services::ImGuiService,
		MM::Services::ImGuiMenuBar,
		MM::Services::ImGuiEngineTools,
		MM::Services::ImGuiOpenGLRendererTools,
		MM::Services::ImGuiScreenDirectorTools,
		MM::Services::ImGuiSceneToolsService,
		MM::Services::ImGuiSoundTools,

		MM::Services::OrganizerSceneService,

		MM::Tox::Services::ToxService,
		MM::Tox::Services::ToxChat,
		MM::Tox::Services::ToxNetChanneled,

		Services::LobbyHost,
		Services::LobbyClient,

		mini_td::Services::MainMenu,
		mini_td::Services::GameHUD,

		// sd is allways the last, since it kicks of the first screen
		MM::Services::ScreenDirector
	>(engine, argc, argv);
}

static MM::Engine engine;

int main(int argc, char** argv) {
	spdlog::cfg::load_env_levels();

	if (!setup_engine(engine, argc, argv)) {
		SPDLOG_ERROR("SETUP FAILED!");
		return 1;
	}

	mini_td::setup_meta_comps();

	// queue first screen

	engine.run();

#ifndef __EMSCRIPTEN__
	engine.cleanup();
#endif

	return 0;
}

