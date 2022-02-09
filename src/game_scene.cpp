#include "./game_scene.hpp"

#include <mm/engine.hpp>
#include <mm/services/filesystem.hpp>

#include <entt/entity/registry.hpp>
#include <entt/entity/organizer.hpp>

#include <mm/opengl/camera_3d.hpp>

#include "./components/game_constants.hpp"
#include "./components/player_lives.hpp"
#include "./components/money.hpp"
#include "./components/path.hpp"
#include "./components/camera_trauma.hpp"

// systems
#include <mm/systems/simple_velocity_system2d.hpp>
#include "./systems/successful_enemies.hpp"
#include "./systems/progress_enemies.hpp"
#include "./systems/progress_to_position.hpp"
#include "./systems/spawn_group_update.hpp"
#include "./systems/projectile_velocity.hpp"
#include "./systems/projectile_collision.hpp"
#include "./systems/projectile_bounds_check.hpp"
#include "./systems/damage.hpp"
#include "./systems/target_first_best.hpp"
#include "./systems/target_first.hpp"
#include "./systems/tower_cooldown.hpp"
#include "./systems/tower_projectile_spawner.hpp"
#include "./systems/tower_ray_shooter.hpp"
#include "./systems/wave_logic.hpp"
#include "./systems/play_sound_on_damage.hpp"
#include "./systems/camera_trauma_update.hpp"
#include <fx_draw/systems/fx_timer.hpp>


#include <mm/logger.hpp>

namespace mini_td {

std::unique_ptr<MM::Scene> create_game_scene(MM::Engine& engine, const Mission1& mission) {
	auto& fs = engine.getService<MM::Services::FilesystemService>();
	std::unique_ptr<MM::Scene> new_scene = std::make_unique<MM::Scene>();
	auto& scene = *new_scene;

	scene.set<MM::Engine&>(engine);

	scene.set<Components::GameConstants>();

	scene.set<Components::PlayerLives>(mission.starting_lives, mission.starting_lives);
	scene.set<Components::Money>(mission.starting_money);

	{ // map
		auto& path = scene.set<Components::Path>(static_cast<Components::Path>(fs.readJson(mission.map_file_path.c_str())));
		//mission.map_file_path;
		//path.points = {
			//{0.f, 5.f},
			//{2.5f, 2.5f},
			//{7.5f, 7.5f},
			//{10.f, 5.f},
		//};
		//path.extents = {10.f, 10.f};

		auto& path_util = scene.set<Components::PathUtil>(path);
		SPDLOG_INFO("path length: {}", path_util.total_length);
	}

	scene.set<MM::OpenGL::Camera3D>();
	scene.set<Components::CameraTrauma>();

	auto& wave = scene.set<Components::Wave>();
	wave.wave = mission.starting_wave > 0 ? mission.starting_wave : 0;
	wave.final_wave = mission.final_wave;
	// TODO: different logic for endless vs normal mode
	wave.start = true;
	wave.auto_proceed = true; // TODO: off by default
	wave.money_per_completed_wave = mission.money_per_wave;
	// ss
	if (fs.isFile(mission.spawn_schedule_file_path.c_str())) {
		scene.set<Components::SpawnSchedule>() = fs.readJson(mission.spawn_schedule_file_path.c_str());
	} else {
		SPDLOG_ERROR("spawn schedule file not found '{}'", mission.spawn_schedule_file_path);
	}

	auto& org = scene.set<entt::organizer>();
	org.emplace<Systems::camera_trauma_update>("camera_trauma_update");
	org.emplace<fx_draw::Systems::fx_timer>("fx_timer");
	org.emplace<Systems::wave_logic>("wave_logic");
	org.emplace<Systems::spawn_group_update>("spawn_group_update");
	org.emplace<Systems::successful_enemies>("successful_enemies");
	org.emplace<Systems::progress_enemies>("progress_enemies");
	org.emplace<Systems::progress_to_position>("progress_to_position");
	org.emplace<Systems::projectile_velocity>("projectile_velocity");
	org.emplace<MM::Systems::simple_positional_velocity>("simple_positional_velocity");
	org.emplace<Systems::projectile_collision>("projectile_collision");
	org.emplace<Systems::projectile_bounds_check>("projectile_bounds_check");
	org.emplace<Systems::tower_ray_shooter>("tower_ray_shooter");
	org.emplace<Systems::play_sound_on_damage>("play_sound_on_damage");
	org.emplace<Systems::damage>("damage");
	org.emplace<Systems::target_first_best>("target_first_best");
	org.emplace<Systems::target_first>("target_first");
	org.emplace<Systems::tower_cooldown>("tower_cooldown");
	org.emplace<Systems::tower_projectile_spawner>("tower_projectile_spawner");

	return new_scene;
}

} // mini_td

