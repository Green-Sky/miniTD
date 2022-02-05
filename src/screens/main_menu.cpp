#include "./main_menu.hpp"

#include <mm/services/sound_service.hpp>
#include <mm/services/opengl_renderer.hpp>
#include <mm/services/imgui_s.hpp>
#include "../services/main_menu.hpp"

#include <mm/opengl/render_tasks/imgui.hpp>

#include <soloud_sfxr.h>
#include <soloud_wav.h>

#include <mm/resource_manager.hpp>

#include <mm/soloud_filesystem_file_impl.hpp>
#include <mm/sound_loader_wav.hpp>

#include <mm/logger.hpp>

namespace mini_td::Screens {

static void main_menu_start_fn(MM::Engine& engine) {
	using namespace entt::literals;

	SPDLOG_INFO("starting main_menu screen");

	{ // rendering
		auto& rs = engine.getService<MM::Services::OpenGLRenderer>();
		rs.render_tasks.clear();
		rs.addRenderTask<MM::OpenGL::RenderTasks::ImGuiRT>(engine);
	}

#if 0
	{ // play coin sound
		static SoLoud::Sfxr fx; // needs to be alive for the duration of the playback <.<
		fx.loadPreset(SoLoud::Sfxr::COIN, 0);
		engine.getService<MM::Services::SoundService>().engine.play(fx, 0.3f);
	}
#endif
	{ // start music loop
		auto& rm = MM::ResourceManager<SoLoud::Wav>::ref();

		if (!rm.contains("music"_hs)) { // only once, assume it gos on for ever
			if (!rm.load<MM::SoundLoaderWavFile>("music", "/sound/miniTD_short_loop_tmp.wav", engine)) {
				SPDLOG_ERROR("failed opening music");
			} else {
				auto& s_e = engine.getService<MM::Services::SoundService>().engine;
				s_e.setGlobalVolume(0.3f); // TODO: move

				auto& wave = *rm.get("music"_hs);
				wave.setVolume(0.f);
				wave.setLooping(true);
				auto handle = s_e.playBackground(wave, -1.f, true);
				s_e.fadeVolume(handle, 0.5f, 2.f);
				s_e.setPause(handle, false);
			}
		}
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

