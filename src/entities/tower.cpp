#include "./tower.hpp"

#include <mm/components/position2d.hpp>
#include "../components/target.hpp"
#include "../components/tower_cooldown.hpp"
#include "../components/tower_projectile_spawner.hpp"
#include "../components/tower_art.hpp"

#include <entt/entity/registry.hpp>

namespace mini_td::Entities {

//void create_tower(MM::Scene& scene, MM::Entity e, float range, glm::vec2 pos, float heat_decay, );

//void spawn_tower(MM::Scene& scene, int64_t level, float progress = 0.f);

// "Particle Cannon"
// - range: long
// - projectile:
//   - small
//   - fast
//   - high dmg (?)
// - heat decay: low
// - art type: 3
void spawn_tower_type1(MM::Scene& scene, glm::vec2 pos) {
	auto e = scene.create();
	scene.emplace<Components::Target>(e, 5.f);
	scene.emplace<Components::TargettingTag_FirstBest>(e);
	scene.emplace<MM::Components::Position2D>(e, pos);
	scene.emplace<Components::TowerCooldown>(e, 2.f, 0.f);
	auto& tpjs = scene.emplace<Components::TowerProjectileSpawner>(e);
	tpjs.pj.radius = 0.05f;
	tpjs.pj.velocity = 3.f;
	tpjs.pj.damage = 2;
	scene.emplace<Components::TowerArtType3>(e);
}

// "Neutrino Bomber"
// - range: short-ish
// - projectile:
//   - large
//   - slow
//   - high dmg
// - heat decay: low
// - art type: 2
void spawn_tower_type2(MM::Scene& scene, glm::vec2 pos) {
	auto e = scene.create();
	scene.emplace<Components::Target>(e, 2.f);
	scene.emplace<Components::TargettingTag_FirstBest>(e);
	scene.emplace<MM::Components::Position2D>(e, pos);
	scene.emplace<Components::TowerCooldown>(e, 0.3f, 0.f);
	auto& tpjs = scene.emplace<Components::TowerProjectileSpawner>(e);
	tpjs.pj.radius = 0.2f;
	tpjs.pj.velocity = 0.9f;
	tpjs.pj.damage = 7;
	scene.emplace<Components::TowerArtType2>(e);
}

// "Burst Laser"
// - range: short-ish
// - ray:
//   - low dmg
// - heat decay: high
// - art type: 1
void spawn_tower_type3(MM::Scene& scene, glm::vec2 pos) {
}

// "X Laser"
// - range: long (very)
// - ray:
//   - high dmg
// - heat decay: low
// - art type: 4
void spawn_tower_type4(MM::Scene& scene, glm::vec2 pos) {
}

} // mini_td::Entities

