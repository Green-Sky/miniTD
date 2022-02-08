#pragma once

#include <entt/fwd.hpp>

#include "../components/projectile.hpp"
#include <mm/components/position2d.hpp>
#include "../components/game_constants.hpp"

namespace mini_td::Systems {

void projectile_collision(
	entt::registry& scene,
	entt::view<entt::get_t<Components::Projectile, const MM::Components::Position2D>> view,
	const Components::GameConstants& gc
);

} // mini_td::Systems

