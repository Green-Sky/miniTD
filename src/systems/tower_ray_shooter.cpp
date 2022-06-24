#include "./tower_ray_shooter.hpp"

#include <entt/entity/registry.hpp>

#include "../components/damage.hpp"

#include <fx_draw/components/fx_timer.hpp>
#include <fx_draw/components/simple_line.hpp>

namespace mini_td::Systems {

void tower_ray_shooter(
	entt::registry& scene,
	entt::view<entt::get_t<const Components::TowerRay, Components::TowerCooldown, const Components::Target, const MM::Components::Position2D>> view
) {
	view.each([&scene](const Components::TowerRay& tr, Components::TowerCooldown& cooldown, const Components::Target& t_target, const MM::Components::Position2D& pos_comp) {
		if (cooldown.heat <= 0.f && scene.valid(t_target.e)) {
			cooldown.heat = 1.f;

			auto& dmg = scene.get_or_emplace<Components::Damage>(t_target.e).damage;
			dmg += tr.damage;


			// spawn a ray
			const auto& ray_start = pos_comp.pos;
			const auto& ray_end = scene.get<MM::Components::Position2D>(t_target.e).pos;
			auto ray_e = scene.create();
			scene.emplace<fx_draw::Components::FXTimer>(ray_e, 0.1f, 0.1f);
			scene.emplace<fx_draw::Components::SimpleLine>(ray_e, ray_start, ray_end, glm::vec4{1.8f, 1.8f, 1.8f, 1.f});
		}
	});
}

} // mini_td::Systems

