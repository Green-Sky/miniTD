#include "./tower_cooldown.hpp"

#include <entt/entity/registry.hpp>

#include <glm/common.hpp>

namespace mini_td::Systems {

void tower_cooldown(
	entt::view<entt::get_t<Components::TowerCooldown>> view,
	const MM::Components::TimeDelta& td
) {
	view.each([delta = td.tickDelta](auto& tower_cooldown) {
		tower_cooldown.heat = glm::max(tower_cooldown.heat - tower_cooldown.decay*delta, 0.f);
	});
}

} // mini_td::Systems
