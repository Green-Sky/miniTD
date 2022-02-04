#include "./debug.hpp"

#include <mm/services/opengl_renderer.hpp>
#include <mm/services/imgui_menu_bar.hpp>
#include <mm/services/imgui_s.hpp>
#include <mm/services/opengl_renderer_tools.hpp>
#include <mm/services/scene_tools.hpp>
#include <mm/services/screen_director.hpp>
#include <mm/services/screen_director_tools.hpp>
#include <mm/services/sound_tools.hpp>

#include <mm/opengl/render_tasks/imgui.hpp>

#include <mm/logger.hpp>

namespace mini_td::Screens {

void create_enable_debug(MM::Engine& engine, MM::Services::ScreenDirector::Screen& screen, const char* next_screen) {
	// start enable
	screen.start_enable.push_back(engine.type<MM::Services::OpenGLRenderer>());
	screen.start_enable.push_back(engine.type<MM::Services::ImGuiService>());
	screen.start_enable.push_back(engine.type<MM::Services::ImGuiMenuBar>());
	screen.start_enable.push_back(engine.type<MM::Services::ImGuiEngineTools>());
	screen.start_enable.push_back(engine.type<MM::Services::ImGuiOpenGLRendererTools>());
	screen.start_enable.push_back(engine.type<MM::Services::ImGuiScreenDirectorTools>());
	screen.start_enable.push_back(engine.type<MM::Services::ImGuiSceneToolsService>());
	screen.start_enable.push_back(engine.type<MM::Services::ImGuiSoundTools>());

	// start disable
	//screen.start_disable.push_back();

	// ####################
	// end disable
	//screen.end_disable.push_back(engine.type<mini_td::Services::MainMenu>());

	screen.start_fn = [next_screen](MM::Engine& _engine) {
		SPDLOG_INFO("starting enable_debug screen");

		// TODO: this is hacky, we need a better solution for this
		{ // rendering (making sure imgui is there)
			auto& rs = _engine.getService<MM::Services::OpenGLRenderer>();
			bool imgui_rt_present = false;
			for (const auto& rt : rs.render_tasks) {
				if (rt->name() == std::string{"ImGuiRT"}) {
					imgui_rt_present = true;
					break;
				}
			}

			if (!imgui_rt_present) {
				rs.addRenderTask<MM::OpenGL::RenderTasks::ImGuiRT>(_engine);
			}
		}

		{ // hide menu bar
			_engine.getService<MM::Services::ImGuiMenuBar>().show_menu_bar = false;
		}

		_engine.getService<MM::Services::ScreenDirector>().queueChangeScreenTo(next_screen);
	};
}

} // mini_td::Screens

