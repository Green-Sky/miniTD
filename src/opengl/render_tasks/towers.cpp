#include "./towers.hpp"

#include <mm/services/scene_service_interface.hpp>
#include <entt/entity/registry.hpp>

#include <mm/opengl/camera_3d.hpp>

#include "../../components/game_constants.hpp"
#include "../../components/tower_art.hpp"
#include <mm/components/position2d.hpp>

namespace mini_td::OpenGL::RenderTasks {

void Towers::render(MM::Services::OpenGLRenderer& rs, MM::Engine& engine) {
	auto* ssi = engine.tryService<MM::Services::SceneServiceInterface>();
	if (ssi == nullptr) {
		return; // nothing to draw
	}

	auto& scene = ssi->getScene();

	if (!scene.ctx().contains<MM::OpenGL::Camera3D>()) {
		return; // nothing to draw
	}

	_fx_draw.setCamera(scene.ctx().get<MM::OpenGL::Camera3D>());

	const auto& gc = scene.ctx().get<Components::GameConstants>();

	rs.targets[target_fbo]->bind(MM::OpenGL::FrameBufferObject::W);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	scene.view<const Components::TowerArtType1, const MM::Components::Position2D>()
	.each([this, &gc](const auto& pos_comp) {
		draw_tower_type1(pos_comp.pos, gc.tower_radius);
	});
	scene.view<const Components::TowerArtType2, const MM::Components::Position2D>()
	.each([this, &gc](const auto& pos_comp) {
		draw_tower_type2(pos_comp.pos, gc.tower_radius);
	});
	scene.view<const Components::TowerArtType3, const MM::Components::Position2D>()
	.each([this, &gc](const auto& pos_comp) {
		draw_tower_type3(pos_comp.pos, gc.tower_radius);
	});
	scene.view<const Components::TowerArtType4, const MM::Components::Position2D>()
	.each([this, &gc](const auto& pos_comp) {
		draw_tower_type4(pos_comp.pos, gc.tower_radius);
	});

	_fx_draw.flushTris();
	_fx_draw.flushLines();
}

// circle base
// line inner
// done
void Towers::draw_tower_type1(glm::vec2 pos, float tower_radius) {
	glm::vec3 color {
		1.2f, 1.2f, 1.2f
	};

	_fx_draw.drawSolidCircle(pos, tower_radius, {color, 0.2f});
	_fx_draw.drawCircle(pos, tower_radius, {color, 1.f});

	// draw cross
	_fx_draw.drawLine(pos + glm::vec2{0.f, -tower_radius}, pos + glm::vec2{0.f, tower_radius}, {color, 0.8f});
	_fx_draw.drawLine(pos + glm::vec2{-tower_radius, 0.f}, pos + glm::vec2{tower_radius, 0.f}, {color, 0.8f});
}

// circle base
// circle inner
// done
void Towers::draw_tower_type2(glm::vec2 pos, float tower_radius) {
	glm::vec3 color {
		1.2f, 1.2f, 1.2f
	};

	_fx_draw.drawSolidCircle(pos, tower_radius, {color, 0.2f});
	_fx_draw.drawCircle(pos, tower_radius, {color, 1.f});
	_fx_draw.drawCircle(pos, tower_radius*0.333f, {color, 0.8f});
}

// rect base
// circle inner
// done
void Towers::draw_tower_type3(glm::vec2 pos, float tower_radius) {
	glm::vec3 color {
		1.2f, 1.2f, 1.2f
	};

	_fx_draw.drawSolidCircle(pos, tower_radius, {color, 0.2f}, 4.f);
	_fx_draw.drawCircle(pos, tower_radius, {color, 1.f}, 4.f);
	_fx_draw.drawCircle(pos, tower_radius*0.333f, {color, 0.8f});
}

// rect base
// line inner
void Towers::draw_tower_type4(glm::vec2 pos, float tower_radius) {
	glm::vec3 color {
		1.2f, 1.2f, 1.2f
	};

	_fx_draw.drawSolidCircle(pos, tower_radius, {color, 0.2f}, 4.f);
	_fx_draw.drawCircle(pos, tower_radius, {color, 1.f}, 4.f);

	// TODO: this needs to be diff
	// draw cross
	_fx_draw.drawLine(pos + glm::vec2{0.f, -tower_radius}, pos + glm::vec2{0.f, tower_radius}, {color, 0.8f});
	_fx_draw.drawLine(pos + glm::vec2{-tower_radius, 0.f}, pos + glm::vec2{tower_radius, 0.f}, {color, 0.8f});
}

} // gh4nr::OpenGL::RenderTasks

