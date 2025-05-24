#pragma once

#include <entt/fwd.hpp>
#include <entt/entity/storage.hpp>

#include "../components/tower_cooldown.hpp"
#include <mm/components/time_delta.hpp>

namespace mini_td::Systems {

void tower_cooldown(
	entt::view<entt::get_t<Components::TowerCooldown>> view,
	const MM::Components::TimeDelta& td
);

} // mini_td::Systems

