cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(miniTD CXX C)

add_executable(miniTD1
	./setup_services.hpp
	./setup_services.cpp

	./screens/main_menu.hpp
	./screens/main_menu.cpp
	./screens/game_sp.hpp
	./screens/game_sp.cpp
	./screens/debug.hpp
	./screens/debug.cpp

	./services/main_menu.hpp
	./services/main_menu.cpp
	./services/game_hud.hpp
	./services/game_hud.cpp

	./components/enemy.hpp
	./components/path_progress.hpp
	./components/player_lives.hpp
	./components/path.hpp
	./components/path.cpp
	./components/wave.hpp
	./components/spawn_group.hpp
	./components/projectile.hpp
	./components/damage.hpp
	./components/target.hpp
	./components/tower_cooldown.hpp
	./components/tower_projectile_spawner.hpp
	./components/tower_ray.hpp
	./components/tower_art.hpp

	./systems/successful_enemies.hpp
	./systems/successful_enemies.cpp
	./systems/progress_enemies.hpp
	./systems/progress_enemies.cpp
	./systems/progress_to_position.hpp
	./systems/progress_to_position.cpp
	./systems/spawn_group_update.hpp
	./systems/spawn_group_update.cpp
	./systems/projectile_velocity.hpp
	./systems/projectile_velocity.cpp
	./systems/projectile_collision.hpp
	./systems/projectile_collision.cpp
	./systems/projectile_bounds_check.hpp
	./systems/projectile_bounds_check.cpp
	./systems/damage.hpp
	./systems/damage.cpp
	./systems/target_first_best.hpp
	./systems/target_first_best.cpp
	./systems/target_first.hpp
	./systems/target_first.cpp
	./systems/tower_cooldown.hpp
	./systems/tower_cooldown.cpp
	./systems/tower_projectile_spawner.hpp
	./systems/tower_projectile_spawner.cpp
	./systems/tower_ray_shooter.hpp
	./systems/tower_ray_shooter.cpp

	./entities/enemy.hpp
	./entities/enemy.cpp
	./entities/spawn_group.hpp
	./entities/spawn_group.cpp
	./entities/projectile.hpp
	./entities/projectile.cpp
	./entities/tower.hpp
	./entities/tower.cpp

	./opengl/render_tasks/map.hpp
	./opengl/render_tasks/map.cpp
	./opengl/render_tasks/enemies.hpp
	./opengl/render_tasks/enemies.cpp
	./opengl/render_tasks/projectiles.hpp
	./opengl/render_tasks/projectiles.cpp
	./opengl/render_tasks/towers.hpp
	./opengl/render_tasks/towers.cpp

	./main1.cpp
)

target_link_libraries(miniTD1 PUBLIC
	imgui_service
	screen_director

	opengl_renderer_s
	imgui_render_task
	#simple_rect_render_task

	imgui_tools
	imgui_opengl_renderer_tools

	random

	transform_system

	simple_velocity_system

	fx_draw

	mm_logo_screen
)

