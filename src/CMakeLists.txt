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
	./screens/end_screen.hpp
	./screens/end_screen.cpp

	./services/main_menu.hpp
	./services/main_menu.cpp
	./services/game_hud.hpp
	./services/game_hud.cpp

	./mission.hpp
	./mission.cpp

	./game_scene.hpp
	./game_scene.cpp

	./components/game_constants.hpp
	./components/enemy.hpp
	./components/path_progress.hpp
	./components/player_lives.hpp
	./components/path.hpp
	./components/path.cpp
	./components/wave.hpp
	./components/spawn_group.hpp
	./components/spawn_schedule.hpp
	./components/spawn_schedule.cpp
	./components/projectile.hpp
	./components/damage.hpp
	./components/target.hpp
	./components/tower_cooldown.hpp
	./components/tower_projectile_spawner.hpp
	./components/tower_ray.hpp
	./components/tower_art.hpp
	./components/tower_preview.hpp
	./components/camera_trauma.hpp

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
	./systems/wave_logic.hpp
	./systems/wave_logic.cpp
	./systems/play_sound_on_damage.hpp
	./systems/play_sound_on_damage.cpp
	./systems/camera_trauma_update.hpp
	./systems/camera_trauma_update.cpp
	./systems/player_death.hpp
	./systems/player_death.cpp

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
	clear_render_task
	bloom
	composition_render_task

	sound_service
	filesystem_soloud_file

	imgui_tools
	imgui_sound
	imgui_opengl_renderer_tools

	random

	transform_system

	simple_velocity_system

	fx_draw

	mm_logo_screen
)

if(EMSCRIPTEN)
	target_link_libraries(miniTD1
		PUBLIC
			-sMIN_WEBGL_VERSION=2
			-sALLOW_MEMORY_GROWTH=1
			"--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/../res@/"
	)
endif()

########################################

add_executable(generate_spawn_schedule
	./components/spawn_group.hpp

	./generate_spawn_schedule.cpp
)

target_link_libraries(generate_spawn_schedule PUBLIC
	common_components_serialize_json
	engine # engine_fwd.hpp
)

########################################

add_executable(miniTDTox
	./setup_services.hpp
	./setup_services_tox.cpp

	./screens/main_menu.hpp
	./screens/main_menu.cpp
	./screens/game_sp.hpp
	./screens/game_sp.cpp
	./screens/game_mp.hpp
	./screens/game_mp.cpp
	./screens/debug.hpp
	./screens/debug.cpp
	./screens/end_screen.hpp
	./screens/end_screen.cpp

	./services/main_menu.hpp
	./services/main_menu.cpp
	./services/game_hud.hpp
	./services/game_hud.cpp

	./services/lobby_host.hpp
	./services/lobby_host.cpp
	./services/lobby_client.hpp
	./services/lobby_client.cpp

	./mission.hpp
	./mission.cpp

	./game_scene.hpp
	./game_scene.cpp

	./components/game_constants.hpp
	./components/enemy.hpp
	./components/path_progress.hpp
	./components/player_lives.hpp
	./components/path.hpp
	./components/path.cpp
	./components/wave.hpp
	./components/spawn_group.hpp
	./components/spawn_schedule.hpp
	./components/spawn_schedule.cpp
	./components/projectile.hpp
	./components/damage.hpp
	./components/target.hpp
	./components/tower_cooldown.hpp
	./components/tower_projectile_spawner.hpp
	./components/tower_ray.hpp
	./components/tower_art.hpp
	./components/tower_preview.hpp
	./components/camera_trauma.hpp

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
	./systems/wave_logic.hpp
	./systems/wave_logic.cpp
	./systems/play_sound_on_damage.hpp
	./systems/play_sound_on_damage.cpp
	./systems/camera_trauma_update.hpp
	./systems/camera_trauma_update.cpp
	./systems/player_death.hpp
	./systems/player_death.cpp

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

	./setup_meta.hpp
	./setup_meta.cpp
	./comp_meta.hpp
	./comp_meta.cpp

	./main_tox.cpp
)

target_link_libraries(miniTDTox PUBLIC
	imgui_service
	screen_director

	opengl_renderer_s
	imgui_render_task
	#simple_rect_render_task
	clear_render_task
	bloom
	composition_render_task

	sound_service
	filesystem_soloud_file

	imgui_tools
	imgui_sound
	imgui_opengl_renderer_tools

	random

	transform_system

	simple_velocity_system

	fx_draw

	mm_logo_screen

	mm_tox
	mm_tox_imgui
)

########################################

