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
#include "../systems/projectile_bounds_check.hpp"
#include "../systems/damage.hpp"
#include "../systems/target_first_best.hpp"
#include "../systems/target_first.hpp"
#include "../systems/tower_cooldown.hpp"
#include "../systems/tower_projectile_spawner.hpp"
#include "../systems/tower_ray_shooter.hpp"
#include <fx_draw/systems/fx_timer.hpp>

#include "../entities/spawn_group.hpp"
#include "../entities/projectile.hpp"
#include "../entities/tower.hpp"

#include "../components/player_lives.hpp"
#include "../components/path.hpp"
#include "../components/money.hpp"

#include "../opengl/render_tasks/map.hpp"
#include "../opengl/render_tasks/enemies.hpp"
#include "../opengl/render_tasks/projectiles.hpp"
#include "../opengl/render_tasks/towers.hpp"
#include <fx_draw/opengl/render_tasks/fx_renderer.hpp>

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
		rs.addRenderTask<mini_td::OpenGL::RenderTasks::Towers>(engine);
		rs.addRenderTask<mini_td::OpenGL::RenderTasks::Projectiles>(engine);
		rs.addRenderTask<fx_draw::OpenGL::RenderTasks::FXDrawRenderTask>(engine);
		rs.addRenderTask<MM::OpenGL::RenderTasks::ImGuiRT>(engine);
	}

	// TODO: in dire need of a relocation
	{ // scene setup
		std::unique_ptr<MM::Scene> new_scene = std::make_unique<MM::Scene>();
		auto& scene = *new_scene;

		scene.set<MM::Engine&>(engine);

		scene.set<Components::PlayerLives>(100, 100);
		scene.set<Components::Money>(0);
		auto& path = scene.set<Components::Path>();
		{ // map
			path.points = {
				{0.f, 5.f},
				{2.5f, 2.5f},
				{7.5f, 7.5f},
				{10.f, 5.f},
			};
			path.extents = {10.f, 10.f};

			auto& path_util = scene.set<Components::PathUtil>(path);
			SPDLOG_INFO("path length: {}", path_util.total_length);
		}

		{ // cam
			auto& cam = scene.set<MM::OpenGL::Camera3D>();
			cam.translation = glm::vec3(path.extents/2.f, 0.f);
			cam.horizontalViewPortSize = glm::max(path.extents.x, path.extents.y) * cam.screenRatio;
			cam.setOrthographic();
			cam.updateView();
		}

		auto& org = scene.set<entt::organizer>();
		org.emplace<fx_draw::Systems::fx_timer>("fx_timer");
		org.emplace<Systems::spawn_group_update>("spawn_group_update");
		org.emplace<Systems::successful_enemies>("successful_enemies");
		org.emplace<Systems::progress_enemies>("progress_enemies");
		org.emplace<Systems::progress_to_position>("progress_to_position");
		org.emplace<Systems::projectile_velocity>("projectile_velocity");
		org.emplace<MM::Systems::simple_positional_velocity>("simple_positional_velocity");
		org.emplace<Systems::projectile_collision>("projectile_collision");
		org.emplace<Systems::projectile_bounds_check>("projectile_bounds_check");
		org.emplace<Systems::tower_ray_shooter>("tower_ray_shooter");
		org.emplace<Systems::damage>("damage");
		org.emplace<Systems::target_first_best>("target_first_best");
		org.emplace<Systems::target_first>("target_first");
		org.emplace<Systems::tower_cooldown>("tower_cooldown");
		org.emplace<Systems::tower_projectile_spawner>("tower_projectile_spawner");

		{ // spawn groups test
			Entities::spawn_spawn_group(scene, 1, 16, 1.3f);

			Entities::spawn_spawn_group(scene, 2, 4, 3.f, -6.f);

			Entities::spawn_spawn_group(scene, 1, 40, 0.2f, -24.f);
		}

#if 0
		{ // test tower
			auto e = scene.create();
			scene.emplace<Components::Target>(e, 2.f);
			scene.emplace<Components::TargettingTag_FirstBest>(e);
			scene.emplace<MM::Components::Position2D>(e, glm::vec2{5.f, -4.5f});
			scene.emplace<Components::TowerCooldown>(e, 1.f, 0.f);
			auto& tpjs = scene.emplace<Components::TowerProjectileSpawner>(e);
			//tpjs.pj.damage = 1;
			scene.emplace<Components::TowerArtType4>(e);
		}
#endif
		{ // test tower
			Entities::spawn_tower_type1(scene, {5.f, 5.5f});
			Entities::spawn_tower_type4(scene, {5.f, 4.5f});
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

