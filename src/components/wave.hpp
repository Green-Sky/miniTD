#pragma once

#include <cstdint>

namespace mini_td::Components {

struct Wave {
	int64_t wave {0};

	bool start {false}; // set this to true to start this->wave wave
	bool active {false}; // indicator
	bool auto_proceed {false}; // if active is set to false, also set start to true

	float time_into_wave {0.f}; // reset to 0 on start

	int64_t money_per_completed_wave {0};
};

} // mini_td::Components

