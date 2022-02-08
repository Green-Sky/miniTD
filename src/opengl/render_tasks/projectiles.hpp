#pragma once

#include <mm/opengl/render_task.hpp>
#include <mm/services/opengl_renderer.hpp>

#include <fx_draw/fx_draw.hpp>

namespace mini_td::OpenGL::RenderTasks {

class Projectiles : public MM::OpenGL::RenderTask {
	private:
		fx_draw::FXDraw _fx_draw; // not default initialized

	public:
		explicit Projectiles(MM::Engine& engine) : _fx_draw(engine) {}

		const char* name(void) override { return "mini_td::Projectiles"; }

		void render(MM::Services::OpenGLRenderer& rs, MM::Engine& engine) override;

	public:
		std::string target_fbo = "display";
};

} // gh4nr::OpenGL::RenderTasks

