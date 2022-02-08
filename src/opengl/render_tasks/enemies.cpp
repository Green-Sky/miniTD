#include "./enemies.hpp"

#include <mm/services/scene_service_interface.hpp>
#include <entt/entity/registry.hpp>

#include <mm/opengl/camera_3d.hpp>

#include "../../components/game_constants.hpp"
#include "../../components/enemy.hpp"
#include <mm/components/position2d.hpp>

namespace mini_td::OpenGL::RenderTasks {

void Enemies::render(MM::Services::OpenGLRenderer& rs, MM::Engine& engine) {
	auto& scene = engine.getService<MM::Services::SceneServiceInterface>().getScene();
	const auto& gc = scene.ctx<Components::GameConstants>();

	_fx_draw.setCamera(scene.ctx<MM::OpenGL::Camera3D>());

	rs.targets[target_fbo]->bind(MM::OpenGL::FrameBufferObject::W);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	scene.view<const Components::Enemy, const MM::Components::Position2D>()
	.each([this, &gc](const auto& enemy_comp, const auto& pos_comp) {
		draw_enemy(pos_comp.pos, enemy_comp.level, gc.enemy_radius);
	});

	_fx_draw.flushTris();
	_fx_draw.flushLines();
}

void Enemies::draw_enemy(glm::vec2 pos, int64_t level, float radius) {
	// TODO: refine this
	// TODO: extract this
	glm::vec3 color_list[] {
		{0.f, 0.9f, 0.f},
		{0.1f, 0.3f, 0.8f},
		{0.9f, 0.0f, 0.f},
	};
	glm::vec3 color = color_list[((level - 1) / 3)%3];

	if ((level-1) % 3 == 0) {
		_fx_draw.drawSolidCircle(pos, radius, {color, 0.2f}, 3.f);
		_fx_draw.drawCircle(pos, radius, {color, 1.f}, 3.f);
	} else if ((level-1) % 3 == 1) {
		_fx_draw.drawSolidCircle(pos, radius, {color, 0.2f}, 4.f);
		_fx_draw.drawCircle(pos, radius, {color, 1.f}, 4.f);
	} else {
		_fx_draw.drawSolidCircle(pos, radius, {color, 0.2f});
		_fx_draw.drawCircle(pos, radius, {color, 1.f});
	}
}

} // gh4nr::OpenGL::RenderTasks

