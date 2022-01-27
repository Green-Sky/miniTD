#include "./progress_enemies.hpp"

#include <entt/entity/registry.hpp>

#include <mm/logger.hpp>

namespace mini_td::Systems {

void progress_enemies(
	entt::view<entt::get_t<Components::PathProgress, const Components::Enemy>> view,
	const Components::Path& path,
	const MM::Components::TimeDelta& td
) {
	view.each([path_length = path.total_length, delta = td.tickDelta](auto& path_progress, const auto& enemy) {
		float speed = 0.f; // m/s

		// TODO: extract
		// TODO: version is bad
		if (enemy.level >= 1 && enemy.level <= 3) {
			speed = 0.5f + 0.1f * (enemy.level - 1);
		} else if (enemy.level <= 6) {
			speed = 1.0f + 0.5f * (enemy.level - 4);
		} else {
			speed = 1.f * enemy.level;
		}

		const float progress_speed = (speed / path_length) * delta;
		path_progress.progress += progress_speed;

		SPDLOG_INFO("new prog {}", path_progress.progress);
	});
}

} // mini_td::Systems

