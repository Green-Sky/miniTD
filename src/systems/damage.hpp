#pragma once

#include <entt/fwd.hpp>
#include <entt/entity/storage.hpp>

#include "../components/enemy.hpp"
#include "../components/damage.hpp"
#include "../components/money.hpp"

namespace mini_td::Systems {

// applies the damage and destroys enemies
void damage(
	entt::registry& scene,
	entt::view<entt::get_t<const Components::Damage, Components::Enemy>> view,
	Components::Money& money
);

} // mini_td::Systems

