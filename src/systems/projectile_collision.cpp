#include "./projectile_collision.hpp"

#include <mm/engine_fwd.hpp>

#include <entt/entity/registry.hpp>

#include "../components/enemy.hpp"
#include "../components/damage.hpp"

#include <glm/geometric.hpp>

namespace mini_td::Systems {

// TODO: inefficent
// O(n * m) lel
void projectile_collision(
	entt::registry& scene,
	entt::view<entt::get_t<Components::Projectile, const MM::Components::Position2D>> view,
	const Components::GameConstants& gc
) {
	std::vector<MM::Entity> to_delete {}; // pj
	view.each([&scene, &to_delete, &gc](const MM::Entity e_pj, Components::Projectile& pj, const MM::Components::Position2D& pj_pos_comp) {
		for (const auto& [e_e, enemy, e_pos_comp] : scene.view<const Components::Enemy, const MM::Components::Position2D>().each()) {
			const float dist = glm::distance(pj_pos_comp.pos, e_pos_comp.pos);
			const float radius_sum = pj.radius + gc.enemy_radius;
			if (dist <= radius_sum) {
				// hit something
				auto& enemy_dmg = scene.get_or_emplace<Components::Damage>(e_e).damage;
				const auto max_applicalbe_dmg = enemy.level - enemy_dmg;
				const auto actual_dmg = glm::min(max_applicalbe_dmg, pj.damage);
				enemy_dmg += actual_dmg;
				pj.damage -= actual_dmg;

				if (pj.damage <= 0) {
					to_delete.push_back(e_pj);
					break;
				}
			}
		}
	});

	scene.destroy(to_delete.cbegin(), to_delete.cend());
}

} // mini_td::Systems

