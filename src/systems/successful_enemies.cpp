#include "./successful_enemies.hpp"

#include <entt/entity/registry.hpp>

#include <mm/logger.hpp>

namespace mini_td::Systems {

// checks if enemies where successful, removes enemy layers from PlayerLives and destroys the enemy
void successful_enemies(
	entt::registry& scene,
	entt::view<entt::get_t<const Components::PathProgress, const Components::Enemy>> view,
	Components::PlayerLives& player_lives
) {
	std::vector<entt::entity> to_remove {};

	view.each([&to_remove, &player_lives](const auto e, const auto& progress, const auto& enemy) {
		if (progress.progress >= 1.f) {
			to_remove.push_back(e);

			player_lives.lives -= enemy.level;

			SPDLOG_INFO("lost {} lives", enemy.level);
		}
	});

	scene.destroy(to_remove.cbegin(), to_remove.cend());
}

} // mini_td::Systems

