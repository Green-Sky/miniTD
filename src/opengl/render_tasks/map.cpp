#include "./map.hpp"

#include <mm/services/scene_service_interface.hpp>
#include <entt/entity/registry.hpp>

#include <mm/opengl/camera_3d.hpp>

#include "../../components/path.hpp"
#include <mm/components/position2d.hpp>

namespace mini_td::OpenGL::RenderTasks {

void Map::render(MM::Services::OpenGLRenderer& rs, MM::Engine& engine) {
	auto* ssi = engine.tryService<MM::Services::SceneServiceInterface>();
	if (ssi == nullptr) {
		return; // nothing to draw
	}

	auto& scene = ssi->getScene();

	if (!scene.ctx().contains<MM::OpenGL::Camera3D>()) {
		return; // nothing to draw
	}

	_fx_draw.setCamera(scene.ctx().at<MM::OpenGL::Camera3D>());

	const auto& path = scene.ctx().at<Components::Path>();

	rs.targets[target_fbo]->bind(MM::OpenGL::FrameBufferObject::W);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glm::vec4 path_line_color {0.5f, 0.5f, 0.5f, 0.3f}; // light gray
	_fx_draw.drawPolyLine(path.points.data(), path.points.size(), path_line_color);

	//_fx_draw.flushTris();
	_fx_draw.flushLines();
}

} // gh4nr::OpenGL::RenderTasks

