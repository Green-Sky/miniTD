#pragma once

#include <entt/fwd.hpp>

#include "../components/camera_trauma.hpp"
#include "../components/game_constants.hpp"

#include <mm/components/time_delta.hpp>

namespace mini_td::Systems {

void camera_trauma_update(
	Components::CameraTrauma& camera_trauma,
	const Components::GameConstants& gc,
	const MM::Components::TimeDelta& td
);

} // mini_td::Systems

