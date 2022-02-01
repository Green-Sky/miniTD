#pragma once

#include <cstdint>

#include <nlohmann/json.hpp>

namespace mini_td::Components {

struct SpawnGroup {
	int64_t level {1};
	int64_t count {1};
	float interval {1.f}; // per sec

	int64_t spawned_count {0};

	float time_accu {0.f}; // accumulated time
};

// spawned_count is intenal?
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SpawnGroup, level, count, interval, time_accu)

} // mini_td::Components

