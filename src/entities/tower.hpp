#pragma once

#include <mm/engine_fwd.hpp>
#include <glm/fwd.hpp>

namespace mini_td::Entities {

//void create_tower(MM::Scene& scene, MM::Entity e, float range, glm::vec2 pos, float heat_decay, );

//void spawn_tower(MM::Scene& scene, int64_t level, float progress = 0.f);

// HACK:

// "Particle Cannon"
// - range: long
// - projectile:
//   - small
//   - fast
//   - high dmg
// - heat decay: low
// - art type: 3
void spawn_tower_type1(MM::Scene& scene, glm::vec2 pos);

// "Neutrino Bomber"
// - range: short-ish
// - projectile:
//   - large
//   - slow
//   - high dmg
// - heat decay: low
// - art type: 2
void spawn_tower_type2(MM::Scene& scene, glm::vec2 pos);

// "Burst Laser"
// - range: short-ish
// - ray:
//   - low dmg
// - heat decay: high
// - art type: 1
void spawn_tower_type3(MM::Scene& scene, glm::vec2 pos);

// "X Laser"
// - range: long (very)
// - ray:
//   - high dmg
// - heat decay: low
// - art type: 4
void spawn_tower_type4(MM::Scene& scene, glm::vec2 pos);

} // mini_td::Entities

