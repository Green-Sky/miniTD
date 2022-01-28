#pragma once

#include <mm/engine_fwd.hpp>

#include "../components/spawn_group.hpp"
#include <mm/components/time_delta.hpp>

namespace mini_td::Systems {

void spawn_group_update(
	MM::Scene& scene,
	entt::view<entt::get_t<Components::SpawnGroup>> view,
	const MM::Components::TimeDelta& td
);

} // mini_td::Systems

