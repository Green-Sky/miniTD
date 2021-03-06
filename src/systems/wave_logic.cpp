#include "./wave_logic.hpp"

#include <mm/engine.hpp>
#include <mm/services/screen_director.hpp>

#include <entt/entity/registry.hpp>

// for empty checks
#include "../components/spawn_group.hpp"
#include "../components/enemy.hpp"

#include <mm/logger.hpp>

namespace mini_td::Systems {

void wave_logic(
	entt::registry& scene,
	Components::Wave& wave,
	Components::Money& money,
	MM::Engine& engine,
	const Components::SpawnSchedule& ss,
	const MM::Components::TimeDelta& td
) {
	if (wave.active) {
		wave.time_into_wave += td.tickDelta;
	}

	if (wave.start) {
		SPDLOG_INFO("staring wave {}", wave.wave);
		wave.start = false;
		wave.time_into_wave = 0.f;
		ss.spawnWave(scene, wave.wave);
		wave.active = true;
		return;
	}

	// still spawning
	if (!scene.view<Components::SpawnGroup>().empty()) {
		wave.active = true;
		return;
	}

	// still enemies
	if (!scene.view<Components::Enemy>().empty()) {
		wave.active = true;
		return;
	}

	// if we got here, nothings alive
	if (wave.active) { // previous tick, there was still activity
		// we just finished the wave
		money.count += wave.money_per_completed_wave;

		wave.active = false;
		wave.start = wave.auto_proceed;
		wave.wave++;

		if (wave.final_wave >= 0 && wave.wave > wave.final_wave) {
			// you won
			// TODO: diff win and fail
			engine.getService<MM::Services::ScreenDirector>().queueChangeScreenTo("mini_td::Screens::end_screen");
		}
	} else {
		wave.start = wave.auto_proceed;
	}
}

} // mini_td::Systems

