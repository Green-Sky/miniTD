#include "./main_menu.hpp"

#include <mm/services/opengl_renderer.hpp>
#include <mm/services/imgui_s.hpp>
#include "../services/main_menu.hpp"

#include <mm/opengl/render_tasks/imgui.hpp>

#include <mm/logger.hpp>

namespace mini_td::Screens {

static void main_menu_start_fn(MM::Engine& engine) {
		SPDLOG_INFO("starting main_menu screen");

		{ // rendering
			auto& rs = engine.getService<MM::Services::OpenGLRenderer>();
			rs.render_tasks.clear();
			rs.addRenderTask<MM::OpenGL::RenderTasks::ImGuiRT>(engine);
		}
}

void create_main_menu(MM::Engine& engine, MM::Services::ScreenDirector::Screen& screen) {
	// start enable
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

