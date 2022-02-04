#include "./main_menu.hpp"

#include <mm/services/sound_service.hpp>
#include <mm/services/opengl_renderer.hpp>
#include <mm/services/imgui_s.hpp>
#include "../services/main_menu.hpp"

#include <mm/opengl/render_tasks/imgui.hpp>

#include <soloud_sfxr.h>

#include <mm/logger.hpp>

namespace mini_td::Screens {

static void main_menu_start_fn(MM::Engine& engine) {
		SPDLOG_INFO("starting main_menu screen");

		{ // rendering
			auto& rs = engine.getService<MM::Services::OpenGLRenderer>();
			rs.render_tasks.clear();
			rs.addRenderTask<MM::OpenGL::RenderTasks::ImGuiRT>(engine);
		}

		{ // play coin sound
			static SoLoud::Sfxr fx; // needs to be alive for the duration of the playback <.<
			fx.loadPreset(SoLoud::Sfxr::COIN, 0);
			engine.getService<MM::Services::SoundService>().engine.play(fx, 0.3f);
		}
}

void create_main_menu(MM::Engine& engine, MM::Services::ScreenDirector::Screen& screen) {
	// start enable
	screen.start_enable.push_back(engine.type<MM::Services::SoundService>());
	screen.start_enable.push_back(engine.type<MM::Services::OpenGLRenderer>());
	screen.start_enable.push_back(engine.type<MM::Services::ImGuiService>());
	screen.start_enable.push_back(engine.type<mini_td::Services::MainMenu>());

	// start disable
	//screen.start_disable.push_back();

	// ####################
	// end disable
	screen.end_disable.push_back(engine.type<mini_td::Services::MainMenu>());

	screen.start_fn = main_menu_start_fn;
}

} // mini_td::Screens

