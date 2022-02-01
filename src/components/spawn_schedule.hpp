#pragma once

#include <mm/engine_fwd.hpp>

#include "./spawn_group.hpp"

#include <nlohmann/json.hpp>

namespace mini_td::Components {

struct SpawnSchedule {
	// if wave (index) is not present, do random based on wave
	std::map<uint64_t, std::vector<SpawnGroup>> schedule {};

	void spawnWave(MM::Scene& scene, uint64_t wave) const;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SpawnSchedule, schedule)

} // mini_td::Components

