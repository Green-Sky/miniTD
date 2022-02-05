#include "./successful_enemies.hpp"

#include <entt/entity/registry.hpp>

#include <mm/services/sound_service.hpp>
#include <mm/resource_manager.hpp>
#include <soloud_sfxr.h>

#include <mm/logger.hpp>

namespace mini_td::Systems {

// checks if enemies where successful, removes enemy layers from PlayerLives and destroys the enemy
void successful_enemies(
	entt::registry& scene,
	entt::view<entt::get_t<const Components::PathProgress, const Components::Enemy>> view,
	Components::PlayerLives& player_lives,
	MM::Engine& engine
) {
	std::vector<entt::entity> to_remove {};

	const auto lives_before = player_lives.lives;

	view.each([&to_remove, &player_lives](const auto e, const auto& progress, const auto& enemy) {
		if (progress.progress >= 1.f) {
			to_remove.push_back(e);

			player_lives.lives -= enemy.level;

			SPDLOG_INFO("lost {} lives, {}/{}", enemy.level, player_lives.lives, player_lives.max);
		}
	});

	scene.destroy(to_remove.cbegin(), to_remove.cend());

	const auto lives_lost = lives_before - player_lives.lives;
	if (lives_lost > 0) {
		// sound
		using namespace entt::literals;
		auto& rm = MM::ResourceManager<SoLoud::Sfxr>::ref();
		if (rm.contains("hurt"_hs)) {
			auto s = rm.get("hurt"_hs);

			auto& s_e = engine.getService<MM::Services::SoundService>().engine;

			s_e.play(*s, 1.0f);
		}

		// TODO: screen shake
		//const float lives_lost_relative = float(lives_lost) / player_lives.max;
	}
}

} // mini_td::Systems

