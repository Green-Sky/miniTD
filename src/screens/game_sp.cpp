#include "./game_sp.hpp"

#include <mm/services/opengl_renderer.hpp>
#include <mm/services/imgui_s.hpp>
#include <mm/services/organizer_scene.hpp>

#include <mm/opengl/render_tasks/imgui.hpp>

#include <entt/entity/registry.hpp>
#include <entt/entity/organizer.hpp>

// systems
#include "../systems/successful_enemies.hpp"
#include "../systems/progress_enemies.hpp"
#include "../systems/progress_to_position.hpp"

#include "../entities/enemy.hpp"

#include "../components/player_lives.hpp"
#include "../components/path.hpp"

#include "../opengl/render_tasks/map.hpp"
#include "../opengl/render_tasks/enemies.hpp"

#include <mm/opengl/camera_3d.hpp>

#include <mm/logger.hpp>

#include <memory>
#include <utility>

namespace mini_td::Screens {

static void game_sp_start_fn(MM::Engine& engine) {
	SPDLOG_INFO("starting game_sp screen");

	{ // rendering
		auto& rs = engine.getService<MM::Services::OpenGLRenderer>();
		rs.render_tasks.clear();
		rs.addRenderTask<mini_td::OpenGL::RenderTasks::Map>(engine);
		rs.addRenderTask<mini_td::OpenGL::RenderTasks::Enemies>(engine);
		rs.addRenderTask<MM::OpenGL::RenderTasks::ImGuiRT>(engine);
		// TODO: fx_draw
	}

	// TODO: in dire need of a relocation
	{ // scene setup
		std::unique_ptr<MM::Scene> new_scene = std::make_unique<MM::Scene>();
		auto& scene = *new_scene;

		scene.set<MM::Engine&>(engine);

		scene.set<Components::PlayerLives>(100);
		auto& path = scene.set<Components::Path>();
		{ // map
			path.points = {
				{0.f, -5.f},
				{2.5f, -2.5f},
				{7.5f, -7.5f},
				{10.f, -5.f},
			};
			path.extents = {10.f, -10.f};
			path.updateValues();
			SPDLOG_INFO("path length: {}", path.total_length);
		}

		{ // cam
			auto& cam = scene.set<MM::OpenGL::Camera3D>();
			cam.translation = glm::vec3(path.extents/2.f, 0.f);
			cam.horizontalViewPortSize = glm::max(path.extents.x, glm::abs(path.extents.y)) * cam.screenRatio;
			cam.setOrthographic();
			cam.updateView();
		}

		auto& org = scene.set<entt::organizer>();
		org.emplace<Systems::successful_enemies>("successful_enemies");
		org.emplace<Systems::progress_enemies>("progress_enemies");
		org.emplace<Systems::progress_to_position>("progress_to_position");

		{ // spawn tset enemy
			Entities::spawn_enemy(scene, 1);
		}

		engine.getService<MM::Services::OrganizerSceneService>().changeSceneNow(std::move(new_scene));
	}
}

void create_game_sp(MM::Engine& engine, MM::Services::ScreenDirector::Screen& screen) {
	// start enable
	screen.start_enable.push_back(engine.type<MM::Services::OpenGLRenderer>());
	screen.start_enable.push_back(engine.type<MM::Services::ImGuiService>());
	screen.start_enable.push_back(engine.type<MM::Services::OrganizerSceneService>());

	// start disable
	//screen.start_disable.push_back();

	// ####################
	// end disable
	//screen.end_disable.push_back(engine.type<mini_td::Services::MainMenu>());
	screen.end_disable.push_back(engine.type<MM::Services::OrganizerSceneService>());

	screen.start_fn = game_sp_start_fn;
}

} // mini_td::Screens

