#pragma once

#include <cstdint>

#include <nlohmann/json.hpp>

namespace mini_td::Components {

struct Enemy {
	int64_t level {1};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Enemy, level)

} // mini_td::Components

