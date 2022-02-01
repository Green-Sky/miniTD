#pragma once

#include <entt/fwd.hpp>

#include "../components/wave.hpp"
#include "../components/spawn_schedule.hpp"

#include <mm/components/time_delta.hpp>

namespace mini_td::Systems {

void wave_logic(
	entt::registry& scene,
	Components::Wave& wave,
	const Components::SpawnSchedule& ss,
	const MM::Components::TimeDelta& td
);

} // mini_td::Systems

