#pragma once

#include <entt/fwd.hpp>
#include <entt/entity/storage.hpp>

#include "../components/tower_ray.hpp"
#include "../components/tower_cooldown.hpp"
#include "../components/target.hpp"
#include <mm/components/position2d.hpp>

namespace mini_td::Systems {

void tower_ray_shooter(
	entt::registry& scene,
	entt::view<entt::get_t<const Components::TowerRay, Components::TowerCooldown, const Components::Target, const MM::Components::Position2D>> view
);

} // mini_td::Systems

