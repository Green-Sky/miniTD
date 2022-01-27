cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(miniTD CXX C)

add_executable(miniTD1
	./setup_services.hpp
	./setup_services.cpp

	./screens/main_menu.hpp
	./screens/main_menu.cpp
	./screens/game_sp.hpp
	./screens/game_sp.cpp

	./services/main_menu.hpp
	./services/main_menu.cpp

	./components/enemy.hpp
	./components/path_progress.hpp
	./components/player_lives.hpp
	./components/path.hpp
	./components/path.cpp

	./systems/successful_enemies.hpp
	./systems/successful_enemies.cpp
	./systems/progress_enemies.hpp
	./systems/progress_enemies.cpp
	./systems/progress_to_position.hpp
	./systems/progress_to_position.cpp

	./entities/enemy.hpp
	./entities/enemy.cpp

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

	mm_logo_screen
)

