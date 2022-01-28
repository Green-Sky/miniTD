#pragma once

#include <cstdint>

namespace mini_td::Components {

struct SpawnGroup {
	int64_t level {1};
	int64_t count {1};
	float interval {1.f}; // per sec

	int64_t spawned_count {0};

	float time_accu {0.f}; // accumulated time
};

} // mini_td::Components

