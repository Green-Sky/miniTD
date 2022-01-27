#pragma once

#include <mm/engine_fwd.hpp>

namespace mini_td::Entities {

void create_enemy(MM::Scene& scene, MM::Entity e, int64_t level, float progress = 0.f);

void spawn_enemy(MM::Scene& scene, int64_t level, float progress = 0.f);

} // mini_td::Entities

