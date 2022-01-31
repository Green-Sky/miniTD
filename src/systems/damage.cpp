#include "./damage.hpp"

#include <mm/engine_fwd.hpp>

#include <entt/entity/registry.hpp>

#include <glm/common.hpp>

namespace mini_td::Systems {

void damage(
	entt::registry& scene,
	entt::view<entt::get_t<const Components::Damage, Components::Enemy>> view,
	Components::Money& money
) {
	std::vector<MM::Entity> to_delete {};
	view.each([&to_delete, &money](const auto e, const auto& dmg, auto& enemy) {
		money.count += glm::min<int64_t>(dmg.damage, enemy.level);
		enemy.level -= dmg.damage;
		if (enemy.level <= 0) {
			to_delete.push_back(e);
		}
	});

	scene.remove<Components::Damage>(view.begin(), view.end());

	scene.destroy(to_delete.cbegin(), to_delete.cend());
}

} // mini_td::Systems

