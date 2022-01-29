#include "./target_first_best.hpp"

#include <entt/entity/registry.hpp>

#include "../components/enemy.hpp"
#include <mm/components/position2d.hpp>

#include <glm/geometric.hpp>

//#include <mm/logger.hpp>

namespace mini_td::Systems {

void target_first_best(
	const entt::registry& scene,
	entt::view<entt::get_t<const Components::TargettingTag_FirstBest, Components::Target, const MM::Components::Position2D>> view
) {
	view.each([&scene](Components::Target& t_target, const MM::Components::Position2D& t_pos_comp) {
		// TODO: reuse last target?
		t_target.e = entt::null;

		size_t i = 0;
		auto enemy_view = scene.view<const Components::Enemy, const MM::Components::Position2D>();
		for (const auto& [e_e, e_enemy, e_pos_comp] : enemy_view.each()) {
			i++;
			const float dist = glm::distance(t_pos_comp.pos, e_pos_comp.pos);
			if (dist <= t_target.range) {
				// we got ourselfs a target
				t_target.e = e_e;
				//SPDLOG_INFO("new target {} dist:{}, it was the {}th ent", e_e, dist, i);
				break;
			}
		}
	});
}

} // mini_td::Systems

