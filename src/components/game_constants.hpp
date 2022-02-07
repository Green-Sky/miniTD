#pragma once

namespace mini_td::Components {

struct GameConstants {
	float camera_trauma_decay = 1.f; // per sec

	float camera_shake_max_angle = 0.3f; // rad
	float camera_shake_max_offset = 0.07f; // horizontals
};

} // mini_td::Components

