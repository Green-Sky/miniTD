#include "./game_sp.hpp"

#include <mm/services/opengl_renderer.hpp>
#include <mm/services/imgui_s.hpp>
#include <mm/services/organizer_scene.hpp>
#include "../services/game_hud.hpp"

#include <mm/opengl/render_tasks/imgui.hpp>

#include <entt/entity/registry.hpp>

#include "../components/path.hpp"

#include "../entities/tower.hpp"

#include "../opengl/render_tasks/map.hpp"
#include "../opengl/render_tasks/enemies.hpp"
#include "../opengl/render_tasks/projectiles.hpp"
#include "../opengl/render_tasks/towers.hpp"
#include <fx_draw/opengl/render_tasks/fx_renderer.hpp>

#include "../game_scene.hpp"
#include "nlohmann/json_fwd.hpp"

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

	{ // scene setup
		//Mission1 mission {};
		////mission.map_file_path = "/maps/inverse_saw.json";
		//mission.map_file_path = "/maps/triton_snake.json";
		//SPDLOG_INFO("mission j: {}", nlohmann::json(mission).dump(2));
		Mission1 mission = load_mission1(engine, "/missions/triton_snake_endless.json");
		std::unique_ptr<MM::Scene> new_scene = create_game_scene(engine, mission);
		auto& scene = *new_scene;

		{ // cam
			const auto& path = scene.ctx<Components::Path>();
			auto& cam = scene.set<MM::OpenGL::Camera3D>();
			cam.translation = glm::vec3(path.extents/2.f, 0.f);
			cam.horizontalViewPortSize = glm::max(path.extents.x, path.extents.y) * cam.screenRatio;
			cam.setOrthographic();
			cam.updateView();
		}

#if 0
		{ // test tower
			Entities::spawn_tower_type1(scene, {5.f, 5.5f});
			Entities::spawn_tower_type4(scene, {5.f, 4.5f});
		}
#endif

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

