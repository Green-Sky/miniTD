#include "./enemies.hpp"

#include <mm/services/scene_service_interface.hpp>
#include <entt/entity/registry.hpp>

#include <mm/opengl/camera_3d.hpp>

//#include "../../components/path.hpp"
#include "../../components/enemy.hpp"
#include <mm/components/position2d.hpp>

namespace mini_td::OpenGL::RenderTasks {

void Enemies::render(MM::Services::OpenGLRenderer& rs, MM::Engine& engine) {
	auto& scene = engine.getService<MM::Services::SceneServiceInterface>().getScene();
	//const auto& path = scene.ctx<Components::Path>();

	_fx_draw.setCamera(scene.ctx<MM::OpenGL::Camera3D>());

	rs.targets[target_fbo]->bind(MM::OpenGL::FrameBufferObject::W);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	//glm::vec4 path_line_color {0.5f, 0.5f, 0.5f, 0.3f}; // light gray
	//_fx_draw.drawPolyLine(path.points.data(), path.points.size(), path_line_color);
	scene.view<const Components::Enemy, const MM::Components::Position2D>()
	.each([this](const auto& enemy_comp, const auto& pos_comp) {
		_fx_draw.drawSolidCircle(pos_comp.pos, 0.1f, {0.9f, 0.f, 0.f, 0.2f});
		_fx_draw.drawCircle(pos_comp.pos, 0.1f, {0.9f, 0.f, 0.f, 1.f});
	});

	_fx_draw.flushTris();
	_fx_draw.flushLines();
}

} // gh4nr::OpenGL::RenderTasks

