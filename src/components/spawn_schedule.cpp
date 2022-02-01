#include "./spawn_schedule.hpp"

#include "../entities/spawn_group.hpp"

#include <mm/random/srng.hpp>

#include <glm/common.hpp>

#include <mm/logger.hpp>

namespace mini_td::Components {

void SpawnSchedule::spawnWave(MM::Scene& scene, uint64_t wave) const {
	if (schedule.count(wave)) {
		auto& groups = schedule.at(wave);
		for (const auto& sg : groups) {
			// TODO: why use the function
			Entities::spawn_spawn_group(scene, sg.level, sg.count, sg.interval, sg.time_accu);
		}
	} else { // random
		uint64_t tokens = 32*wave; // rough amount of worth

		// HACK
		// ... hardcoding the seed and algo makes it predictable procedual
		MM::Random::SRNG rng {uint32_t(11*1337*wave)};

		bool prev_no_spawn = false; // endless loop fail save
		const uint64_t min_token_spend = 16;
		while (tokens >= min_token_spend) {
			auto tokens_this_group = rng.minMax(min_token_spend, glm::min<uint64_t>(tokens, 1024));
			auto level = rng.minMax(1, glm::min<uint64_t>(tokens, wave >= 64 ? 32 : 16)); // max level 32??
			uint64_t count = tokens_this_group/level;
			tokens_this_group = level * count; // acutal token count
			tokens -= tokens_this_group;

			if (count != 0) {
				// TODO: timing
				SPDLOG_INFO("spawn_group: l:{} c:{}, tokens_left:{}", level, count, tokens);
				Entities::spawn_spawn_group(scene, level, count, 1.f, rng.zeroToOne() * wave);
				prev_no_spawn = false;
			} else { // warn
				if (prev_no_spawn) {
					SPDLOG_ERROR("wasted tokens {}", tokens);
					// error, wasted tokens
					break;
				}
				prev_no_spawn = true;
			}
		}
	}
}

} // mini_td::Components

