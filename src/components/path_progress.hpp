#pragma once

#include <cstdint>

#include <nlohmann/json.hpp>

namespace mini_td::Components {

struct PathProgress {
	float progress {0.f}; // [0,1]
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PathProgress, progress)

} // mini_td::Components

