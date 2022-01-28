#include "./spawn_group_update.hpp"

#include "../entities/enemy.hpp"

#include <entt/entity/registry.hpp>

namespace mini_td::Systems {

void spawn_group_update(
	MM::Scene& scene,
	entt::view<entt::get_t<Components::SpawnGroup>> view,
	const MM::Components::TimeDelta& td
) {
	std::vector<MM::Entity> to_delete {};
	view.each([delta = td.tickDelta, &to_delete, &scene](const auto e, Components::SpawnGroup& sg) {
		sg.time_accu += delta;

		while (sg.time_accu >= sg.interval) {
			sg.time_accu -= sg.interval;

			sg.spawned_count++;
			if (sg.spawned_count >= sg.count) {
				to_delete.push_back(e);
			}

			Entities::spawn_enemy(scene, sg.level, 0.f);
		}
	});

	scene.destroy(to_delete.cbegin(), to_delete.cend());
}

} // mini_td::Systems

