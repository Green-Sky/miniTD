#pragma once

namespace mini_td::Components {

struct GameConstants {
	float camera_trauma_decay = 1.f; // per sec

	float camera_shake_max_angle = 0.3f; // rad
	float camera_shake_max_offset = 0.07f; // horizontals

	// tw placement
	// path distance
	float tower_radius = 0.5f;
	float enemy_radius = 0.2f;
};

} // mini_td::Components

