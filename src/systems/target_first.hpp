#pragma once

#include <entt/fwd.hpp>

#include "../components/target.hpp"
#include <mm/components/position2d.hpp>

namespace mini_td::Systems {

void target_first(
	entt::registry& scene,
	entt::view<entt::get_t<const Components::TargettingTag_First, Components::Target, const MM::Components::Position2D>> view
);

} // mini_td::Systems

