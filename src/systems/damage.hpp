#pragma once

#include <entt/fwd.hpp>

#include "../components/enemy.hpp"
#include "../components/damage.hpp"

namespace mini_td::Systems {

// applies the damage and destroys enemies
void damage(
	entt::registry& scene,
	entt::view<entt::get_t<const Components::Damage, Components::Enemy>> view
);

} // mini_td::Systems

