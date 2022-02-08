#pragma once

#include <mm/opengl/render_task.hpp>
#include <mm/services/opengl_renderer.hpp>

#include <fx_draw/fx_draw.hpp>

namespace mini_td::OpenGL::RenderTasks {

class Towers : public MM::OpenGL::RenderTask {
	private:
		fx_draw::FXDraw _fx_draw; // not default initialized

	public:
		explicit Towers(MM::Engine& engine) : _fx_draw(engine) {}

		const char* name(void) override { return "mini_td::Towers"; }

		void render(MM::Services::OpenGLRenderer& rs, MM::Engine& engine) override;

	private:
		void draw_tower_type1(glm::vec2 pos, float tower_radius);
		void draw_tower_type2(glm::vec2 pos, float tower_radius);
		void draw_tower_type3(glm::vec2 pos, float tower_radius);
		void draw_tower_type4(glm::vec2 pos, float tower_radius);

	public:
		std::string target_fbo = "display";
};

} // gh4nr::OpenGL::RenderTasks

