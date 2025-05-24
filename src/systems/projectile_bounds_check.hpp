#pragma once

#include <entt/fwd.hpp>
#include <entt/entity/storage.hpp>

#include "../components/projectile.hpp"
#include "../components/path.hpp"
#include <mm/components/position2d.hpp>

namespace mini_td::Systems {

void projectile_bounds_check(
	entt::registry& scene,
	entt::view<entt::get_t<const Components::Projectile, const MM::Components::Position2D>> view,
	const Components::Path& path
);

} // mini_td::Systems

