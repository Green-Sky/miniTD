#include "./projectiles.hpp"

#include <mm/services/scene_service_interface.hpp>
#include <entt/entity/registry.hpp>

#include <mm/opengl/camera_3d.hpp>

#include "../../components/projectile.hpp"
#include <mm/components/position2d.hpp>

namespace mini_td::OpenGL::RenderTasks {

void Projectiles::render(MM::Services::OpenGLRenderer& rs, MM::Engine& engine) {
	auto& scene = engine.getService<MM::Services::SceneServiceInterface>().getScene();

	_fx_draw.setCamera(scene.ctx<MM::OpenGL::Camera3D>());

	rs.targets[target_fbo]->bind(MM::OpenGL::FrameBufferObject::W);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	scene.view<const Components::Projectile, const MM::Components::Position2D>()
	.each([this](const auto& pj_comp, const auto& pos_comp) {
		// TODO: extract?
		_fx_draw.drawSolidCircle(pos_comp.pos, pj_comp.radius, {1.f, 0.f, 1.f, 0.2f});
		_fx_draw.drawCircle(pos_comp.pos, pj_comp.radius, {1.f, 0.f, 1.f, 1.f});
	});

	_fx_draw.flushTris();
	_fx_draw.flushLines();
}

} // gh4nr::OpenGL::RenderTasks

