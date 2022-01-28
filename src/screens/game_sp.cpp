#include "./game_sp.hpp"

#include <mm/services/opengl_renderer.hpp>
#include <mm/services/imgui_s.hpp>
#include <mm/services/organizer_scene.hpp>
#include "../services/game_hud.hpp"

#include <mm/opengl/render_tasks/imgui.hpp>

#include <entt/entity/registry.hpp>
#include <entt/entity/organizer.hpp>

// systems
#include <mm/systems/simple_velocity_system2d.hpp>
#include "../systems/successful_enemies.hpp"
#include "../systems/progress_enemies.hpp"
#include "../systems/progress_to_position.hpp"
#include "../systems/spawn_group_update.hpp"
#include "../systems/projectile_velocity.hpp"
#include "../systems/projectile_collision.hpp"
#include "../systems/damage.hpp"

#include "../entities/spawn_group.hpp"
#include "../entities/projectile.hpp"

#include "../components/player_lives.hpp"
#include "../components/path.hpp"

#include "../opengl/render_tasks/map.hpp"
#include "../opengl/render_tasks/enemies.hpp"
#include "../opengl/render_tasks/projectiles.hpp"

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
		rs.addRenderTask<mini_td::OpenGL::RenderTasks::Projectiles>(engine);
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
		org.emplace<Systems::projectile_velocity>("projectile_velocity");
		org.emplace<MM::Systems::simple_positional_velocity>("simple_positional_velocity");
		org.emplace<Systems::projectile_collision>("projectile_collision");
		org.emplace<Systems::damage>("damage");

#if 0
		{ // spawn tset enemy
			for (size_t i = 0; i < 9; i++) {
				Entities::spawn_enemy(scene, i + 1, -0.1f*i);
			}
		}
#endif
		{ // spawn groups test
			Entities::spawn_spawn_group(scene, 1, 16, 1.3f);

			Entities::spawn_spawn_group(scene, 2, 4, 3.f, -6.f);

			Entities::spawn_spawn_group(scene, 1, 40, 0.2f, -24.f);
		}

		{ // spawn test pj
			Entities::spawn_projectile(
				scene,
				0.5f,
				3,
				{5,-5},
				(0.5f - 0.125f) * glm::two_pi<float>(),
				0.1f
			);
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

