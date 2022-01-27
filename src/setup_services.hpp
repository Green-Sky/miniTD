#pragma once

#include <mm/engine.hpp>
#include <mm/logger.hpp>

#include <mm/services/filesystem.hpp>
#include <mm/services/sdl_service.hpp>
#include <mm/services/screen_director.hpp>

// templates
template<typename Service>
bool setup_service(MM::Engine& engine, [[maybe_unused]] int argc, [[maybe_unused]] char** argv) {

	engine.addService<Service>();
	// we use screens, we dont enable by default
	//if (!engine.enableService<Service>()) {
		//SPDLOG_ERROR("failed to enable service {}", engine.getService<Service>().name());
		//return false;
	//}

	return true;
}

// entrypoint
template<typename ...Services>
bool setup_engine_t(MM::Engine& engine, int argc, char** argv) {
	return (setup_service<Services>(engine, argc, argv) && ...);
}


template<>
bool setup_service<MM::Services::FilesystemService>(MM::Engine& engine, int argc, char** argv);

template<>
bool setup_service<MM::Services::SDLService>(MM::Engine& engine, int argc, char** argv);

template<>
bool setup_service<MM::Services::ScreenDirector>(MM::Engine& engine, int argc, char** argv);

