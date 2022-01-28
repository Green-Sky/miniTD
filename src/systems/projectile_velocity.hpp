#pragma once

#include <entt/fwd.hpp>

#include "../components/projectile.hpp"
#include <mm/components/view_dir2d.hpp>
#include <mm/components/velocity2d_position.hpp>

namespace mini_td::Systems {

// viewdir and velocity -> mm Velocity2DPosition
void projectile_velocity(
	entt::view<entt::get_t<const Components::Projectile, const MM::Components::ViewDir2D, MM::Components::Velocity2DPosition>> view
);

} // mini_td::Systems

