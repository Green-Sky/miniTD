#include "./game_sp.hpp"

#include <mm/services/opengl_renderer.hpp>
#include <mm/services/imgui_s.hpp>
#include <mm/services/organizer_scene.hpp>
#include "../services/game_hud.hpp"

#include <mm/opengl/render_tasks/imgui.hpp>

#include <entt/entity/registry.hpp>
#include <entt/entity/organizer.hpp>

// systems
#include "../systems/successful_enemies.hpp"
#include "../systems/progress_enemies.hpp"
#include "../systems/progress_to_position.hpp"
#include "../systems/spawn_group_update.hpp"

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

		scene.set<Components::PlayerLives>(100, 100);
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
		org.emplace<Systems::spawn_group_update>("spawn_group_update");
		org.emplace<Systems::successful_enemies>("successful_enemies");
		org.emplace<Systems::progress_enemies>("progress_enemies");
		org.emplace<Systems::progress_to_position>("progress_to_position");

#if 0
		{ // spawn tset enemy
			for (size_t i = 0; i < 9; i++) {
				Entities::spawn_enemy(scene, i + 1, -0.1f*i);
			}
		}
#endif
		{ // spawn groups test
			auto e = scene.create();
			auto& sg = scene.emplace<Components::SpawnGroup>(e);
			sg.level = 1;
			sg.count = 16;
			sg.interval = 1.3f;
		}
		{ // spawn groups test
			auto e = scene.create();
			auto& sg = scene.emplace<Components::SpawnGroup>(e);
			sg.level = 2;
			sg.count = 4;
			sg.interval = 3.f;
			sg.time_accu = -5.f;
		}
		{ // spawn groups test
			auto e = scene.create();
			auto& sg = scene.emplace<Components::SpawnGroup>(e);
			sg.level = 1;
			sg.count = 40;
			sg.interval = 0.2f;
			sg.time_accu = -20.f;
		}

		engine.getService<MM::Services::OrganizerSceneService>().changeSceneNow(std::move(new_scene));
	}
}

void create_game_sp(MM::Engine& engine, MM::Services::ScreenDirector::Screen& screen) {
	// start enable
	screen.start_enable.push_back(engine.type<MM::Services::OpenGLRenderer>());
	screen.start_enable.push_back(engine.type<MM::Services::ImGuiService>());
	screen.start_enable.push_back(engine.type<MM::Services::OrganizerSceneService>());
	screen.start_enable.push_back(engine.type<mini_td::Services::GameHUD>());

	// start disable
	//screen.start_disable.push_back();

	// ####################
	// end disable
	//screen.end_disable.push_back(engine.type<mini_td::Services::MainMenu>());
	screen.end_disable.push_back(engine.type<mini_td::Services::GameHUD>());
	screen.end_disable.push_back(engine.type<MM::Services::OrganizerSceneService>());

	screen.start_fn = game_sp_start_fn;
}

} // mini_td::Screens

