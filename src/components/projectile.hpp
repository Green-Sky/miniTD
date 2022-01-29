#pragma once

#include <cstdint>

namespace mini_td::Components {

struct Projectile {
	float radius {0.1f};
	int64_t damage {1};
	float velocity {1.f}; // TODO: move
};

} // mini_td::Components

