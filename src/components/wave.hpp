#pragma once

#include <cstdint>

namespace mini_td::Components {

struct Wave {
	int64_t wave {0};

	float time_into_wave {0.f};

	// some kinda progress and spawn logic
};

} // mini_td::Components

