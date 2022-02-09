#include "./setup_services.hpp"

#include <mm/screens/mm_logo_screen.hpp>
#include "./screens/main_menu.hpp"
#include "./screens/game_sp.hpp"
#include "./screens/debug.hpp"
#include "./screens/end_screen.hpp"

template<>
bool setup_service<MM::Services::FilesystemService>(MM::Engine& engine, [[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
	engine.addService<MM::Services::FilesystemService>(argv[0], "miniTD", true, true
#ifndef NDEBUG
		// hack for local machine
		,std::vector<std::tuple<std::string, std::string, bool>>{{"../res/", "/", true}}
#endif
	);

	// enable by default
	if (!engine.enableService<MM::Services::FilesystemService>()) {
		SPDLOG_ERROR("failed to enable service {}", engine.getService<MM::Services::FilesystemService>().name());
		return false;
	}

	return true;
}

template<>
bool setup_service<MM::Services::SDLService>(MM::Engine& engine, [[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
	auto& sdl = engine.addService<MM::Services::SDLService>(
		SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER
	);

	// enable by default
	if (!engine.enableService<MM::Services::SDLService>()) {
		SPDLOG_ERROR("failed to enable service {}", sdl.name());
		return false;
	}

	sdl.createGLWindow(argv[0], 1280, 720,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	// after window creation
	SDL_GL_SetSwapInterval(1); // 0 == vsync off

	return true;
}

template<>
bool setup_service<MM::Services::ScreenDirector>(MM::Engine& engine, [[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
	auto& sd = engine.addService<MM::Services::ScreenDirector>();

	// add screens before enable
	MM::Screens::create_mm_logo(
		engine,
		sd.screens["MM::Screens::mm_logo"],
		"mini_td::Screens::main_menu"
	);
	mini_td::Screens::create_main_menu(engine, sd.screens["mini_td::Screens::main_menu"]);
	mini_td::Screens::create_game_sp(engine, sd.screens["mini_td::Screens::game_sp"]);
	mini_td::Screens::create_end_screen(engine, sd.screens["mini_td::Screens::end_screen"]);
	mini_td::Screens::create_enable_debug(
		engine,
		sd.screens["mini_td::Screens::enable_debug"],
		"MM::Screens::mm_logo"
	);

	// queue the inital screen before enable
#if NDEBUG
	sd.queueChangeScreenTo("MM::Screens::mm_logo"); // if release
#else
	sd.queueChangeScreenTo("mini_td::Screens::enable_debug"); // if debug
#endif
	//sd.queueChangeScreenTo("mini_td::Screens::main_menu");

	// enable by default, starts first screen
	if (!engine.enableService<MM::Services::ScreenDirector>()) {
		SPDLOG_ERROR("failed to enable service {}", sd.name());
		return false;
	}

	return true;
}

