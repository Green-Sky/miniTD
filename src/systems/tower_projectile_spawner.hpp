#pragma once

#include <entt/fwd.hpp>

#include "../components/tower_projectile_spawner.hpp"
#include "../components/tower_cooldown.hpp"
#include "../components/target.hpp"
#include <mm/components/position2d.hpp>

namespace mini_td::Systems {

void tower_projectile_spawner(
	entt::registry& scene,
	entt::view<entt::get_t<const Components::TowerProjectileSpawner, Components::TowerCooldown, const Components::Target, const MM::Components::Position2D>> view
);

} // mini_td::Systems

