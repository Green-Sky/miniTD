#pragma once

#include <mm/engine_fwd.hpp>
#include <glm/fwd.hpp>

namespace mini_td::Entities {

void create_projectile(MM::Scene& scene, MM::Entity e, float radius, int64_t damage, glm::vec2 pos, float dir, float velocity);

void spawn_projectile(MM::Scene& scene, float radius, int64_t damage, glm::vec2 pos, float dir, float velocity);

} // mini_td::Entities

