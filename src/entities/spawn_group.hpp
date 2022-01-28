#pragma once

#include <mm/engine_fwd.hpp>

namespace mini_td::Entities {

void create_spawn_group(MM::Scene& scene, MM::Entity e, int64_t level, int64_t count, float interval, float inital_time_accu = 0.f);

void spawn_spawn_group(MM::Scene& scene, int64_t level, int64_t count, float interval, float inital_time_accu = 0.f);

} // mini_td::Entities

