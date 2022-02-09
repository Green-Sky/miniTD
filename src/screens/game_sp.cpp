#include "./game_sp.hpp"

#include <mm/services/opengl_renderer.hpp>
#include <mm/services/imgui_s.hpp>
#include <mm/services/organizer_scene.hpp>
#include "../services/game_hud.hpp"
#include <mm/services/sound_service.hpp>

#include <mm/resource_manager.hpp>

#include <mm/opengl/render_tasks/imgui.hpp>

#include <entt/entity/registry.hpp>

#include "../components/path.hpp"

#include "../entities/tower.hpp"

#include "../opengl/render_tasks/map.hpp"
#include "../opengl/render_tasks/enemies.hpp"
#include "../opengl/render_tasks/projectiles.hpp"
#include "../opengl/render_tasks/towers.hpp"
#include <fx_draw/opengl/render_tasks/fx_renderer.hpp>

#include <soloud_sfxr.h>
#include <mm/soloud_json.hpp>

#include "../game_scene.hpp"

#include <mm/opengl/camera_3d.hpp>

#include <mm/logger.hpp>

#include <memory>
#include <utility>

namespace mini_td::Screens {

struct SoundLoaderSfxrPreset {
	std::shared_ptr<::SoLoud::Sfxr> load(const ::SoLoud::Sfxr::SFXR_PRESETS preset, const int seed) const {
		auto sfxr = std::make_shared<::SoLoud::Sfxr>();
		sfxr->loadPreset(preset, seed);
		return sfxr;
	}
};

struct SoundLoaderSfxrJson {
	std::shared_ptr<::SoLoud::Sfxr> load(const nlohmann::json& j) const {
		auto sfxr = std::make_shared<::SoLoud::Sfxr>();
		*sfxr = j;
		return sfxr;
	}
};

struct SoundLoaderSfxrFile {
	std::shared_ptr<::SoLoud::Sfxr> load(const std::string& path, MM::Engine& engine) const {
		return {};
	}
};

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

	{ // load sound
		using namespace entt::literals;
		auto& rm = MM::ResourceManager<SoLoud::Sfxr>::ref();
		{ // damage (an enemy)
			//rm.load<SoundLoaderSfxrJson>("damage", nlohmann::json());
			//rm.load<SoundLoaderSfxrPreset>("damage", SoLoud::Sfxr::SFXR_PRESETS::BLIP, 0);
			rm.load<SoundLoaderSfxrPreset>("damage", SoLoud::Sfxr::SFXR_PRESETS::BLIP, 1);
		}
		{ // hurt (player loses lives)
			rm.load<SoundLoaderSfxrPreset>("hurt", SoLoud::Sfxr::SFXR_PRESETS::HURT, 3);
			rm.get("hurt"_hs)->mParams.master_vol = 0.3f;
		}
	}
}

void create_game_sp(MM::Engine& engine, MM::Services::ScreenDirector::Screen& screen) {
	// start enable
	screen.start_enable.push_back(engine.type<MM::Services::SoundService>()); // shold be active
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

