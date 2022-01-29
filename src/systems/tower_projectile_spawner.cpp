#include "./tower_projectile_spawner.hpp"

#include <entt/entity/registry.hpp>

#include "../entities/projectile.hpp"
#include <mm/components/velocity2d_position.hpp>

#include <glm/trigonometric.hpp>
#include <glm/geometric.hpp>

#include <mm/logger.hpp>

namespace mini_td::Systems {

void tower_projectile_spawner(
	entt::registry& scene,
	entt::view<entt::get_t<const Components::TowerProjectileSpawner, Components::TowerCooldown, const Components::Target, const MM::Components::Position2D>> view
) {
	view.each([&scene](const Components::TowerProjectileSpawner& tpjs, Components::TowerCooldown& cooldown, const Components::Target& t_target, const MM::Components::Position2D& t_pos_comp) {
		if (cooldown.heat <= 0.f && scene.valid(t_target.e)) {
			cooldown.heat = 1.f;

			// TODO: predict?
			// TODO: we need vel and angle

			// get dir vec
			const auto& target_pos = scene.get<MM::Components::Position2D>(t_target.e).pos;
			const glm::vec2 target_vec = target_pos - t_pos_comp.pos;

			// we assume lenth is not 0

			const float dir = glm::atan(target_vec.y, target_vec.x);

			Entities::spawn_projectile(scene, tpjs.pj.radius, tpjs.pj.damage, t_pos_comp.pos, dir, tpjs.pj.velocity);
		}
	});
}

} // mini_td::Systems

