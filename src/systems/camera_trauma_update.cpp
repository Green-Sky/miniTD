#include "./camera_trauma_update.hpp"

#include <glm/common.hpp>

namespace mini_td::Systems {

void camera_trauma_update(
	Components::CameraTrauma& camera_trauma,
	const Components::GameConstants& gc,
	const MM::Components::TimeDelta& td
) {
	camera_trauma.trauma = glm::clamp(
		camera_trauma.trauma - gc.camera_trauma_decay * td.tickDelta,
		0.f, 1.f
	);
}

} // mini_td::Systems

