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

#include <mm/opengl/render_tasks/clear.hpp>
#include <mm/opengl/bloom.hpp>
#include <mm/opengl/render_tasks/composition.hpp>
#include "../opengl/render_tasks/map.hpp"
#include "../opengl/render_tasks/enemies.hpp"
#include "../opengl/render_tasks/projectiles.hpp"
#include "../opengl/render_tasks/towers.hpp"
#include <fx_draw/opengl/render_tasks/fx_renderer.hpp>

#include <mm/opengl/fbo_builder.hpp>
#include <mm/opengl/texture_loader.hpp>

#include <soloud_sfxr.h>
#include <mm/soloud_json.hpp>

#include "../game_scene.hpp"
#include "mm/services/scene_service_interface.hpp"

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

		{ // texture / fbo setup
			using namespace entt::literals;

			const float render_scale = 1.f;

			auto& rm_t = MM::ResourceManager<MM::OpenGL::Texture>::ref();
			auto [w, h] = engine.getService<MM::Services::SDLService>().getWindowSize();


			// hdr color gles3 / webgl2
			rm_t.reload<MM::OpenGL::TextureLoaderEmpty>(
				"hdr_color",
				GL_RGBA16F,
				w * render_scale, h * render_scale,
				GL_RGBA,
				GL_HALF_FLOAT
			);
			{ // filter
				rm_t.get("hdr_color"_hs)->bind(0);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			rs.targets["game_view"] = MM::OpenGL::FBOBuilder::start()
				.attachTexture(rm_t.get("hdr_color"_hs), GL_COLOR_ATTACHMENT0)
				// we need not depth?
				//.attachTexture(rm_t.get("depth"_hs), GL_DEPTH_ATTACHMENT)
				.setResizeFactors(render_scale, render_scale)
				.setResize(true)
				.finish();
			assert(rs.targets["game_view"]);

		}

		rs.render_tasks.clear();

		// clear
		auto& clear_opaque = rs.addRenderTask<MM::OpenGL::RenderTasks::Clear>(engine);
		clear_opaque.target_fbo = "game_view";
		// clears all color attachments
		clear_opaque.r = 0.1f;
		clear_opaque.g = 0.1f;
		clear_opaque.b = 0.1f;
		clear_opaque.a = 1.f;
		clear_opaque.mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

		rs.addRenderTask<mini_td::OpenGL::RenderTasks::Map>(engine).target_fbo = "game_view";
		rs.addRenderTask<mini_td::OpenGL::RenderTasks::Enemies>(engine).target_fbo = "game_view";
		rs.addRenderTask<mini_td::OpenGL::RenderTasks::Towers>(engine).target_fbo = "game_view";
		rs.addRenderTask<mini_td::OpenGL::RenderTasks::Projectiles>(engine).target_fbo = "game_view";
		rs.addRenderTask<fx_draw::OpenGL::RenderTasks::FXDrawRenderTask>(engine).target_fbo = "game_view";

		// rn does rt too
		MM::OpenGL::setup_bloom(engine, "hdr_color", 6, 1.f);

		// not part of setup_bloom
		auto& comp = rs.addRenderTask<MM::OpenGL::RenderTasks::Composition>(engine);
		comp.color_tex = "hdr_color";
		comp.bloom_tex = "blur_tmp1";
		comp.target_fbo = "display";

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

	//engine.getService<MM::Services::SceneServiceInterface>().getScene().ctx().emplace
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

