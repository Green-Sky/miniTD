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

#include <mm/services/imgui_menu_bar.hpp>
#include <mm/services/engine_tools.hpp>
#include <mm/services/opengl_renderer_tools.hpp>
#include <mm/services/screen_director_tools.hpp>
#include <mm/services/scene_tools.hpp>
#include <mm/services/sound_tools.hpp>

#include <services/mm_tox/tox_service.hpp>
#include <services/mm_tox/tox_chat.hpp>
#include <services/mm_tox/tox_net_channeled.hpp>

#include "./setup_meta.hpp"

#include <entt/entity/registry.hpp>

template<>
bool setup_service<MM::Services::Tox::ToxService>(MM::Engine& engine, [[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
	auto& tox_service = engine.addService<MM::Services::Tox::ToxService>(engine, "save.tox");

	tox_service._app_name = "miniTD";

	if (!engine.enableService<MM::Services::Tox::ToxService>()) {
		SPDLOG_ERROR("failed to enable service {}", engine.getService<MM::Services::Tox::ToxService>().name());
		return false;
	}

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

		MM::Services::Tox::ToxService,
		MM::Services::Tox::ToxChat,
		MM::Services::Tox::ToxNetChanneled,

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

