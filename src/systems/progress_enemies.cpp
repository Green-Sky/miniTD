#include "./progress_enemies.hpp"

#include <entt/entity/registry.hpp>

namespace mini_td::Systems {

void progress_enemies(
	entt::view<entt::get_t<Components::PathProgress, const Components::Enemy>> view,
	const Components::PathUtil& path_util,
	const MM::Components::TimeDelta& td
) {
	view.each([path_length = path_util.total_length, delta = td.tickDelta](auto& path_progress, const auto& enemy) {
		float speed = 0.f; // m/s

		// TODO: extract
		// TODO: version is bad
		if (enemy.level >= 1 && enemy.level <= 3) {
			speed = 1.0f + 0.2f * (enemy.level - 1);
		} else if (enemy.level <= 6) {
			speed = 1.4f + 0.3f * (enemy.level - 4);
		} else {
			speed = 0.3f * enemy.level;
		}

		const float progress_speed = (speed / path_length) * delta;
		path_progress.progress += progress_speed;
	});
}

} // mini_td::Systems

