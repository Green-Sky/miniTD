#include "./damage.hpp"

#include <mm/engine_fwd.hpp>

#include <entt/entity/registry.hpp>

namespace mini_td::Systems {

void damage(
	entt::registry& scene,
	entt::view<entt::get_t<const Components::Damage, Components::Enemy>> view
) {
	std::vector<MM::Entity> to_delete {};
	view.each([&to_delete](const auto e, const auto& dmg, auto& enemy) {
		enemy.level -= dmg.damage;
		if (enemy.level <= 0) {
			to_delete.push_back(e);
		}
	});

	scene.remove<Components::Damage>(view.begin(), view.end());

	scene.destroy(to_delete.cbegin(), to_delete.cend());
}

} // mini_td::Systems

