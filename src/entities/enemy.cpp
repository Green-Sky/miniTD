#include "./enemy.hpp"

#include <entt/entity/registry.hpp>

#include "../components/enemy.hpp"
#include "../components/path_progress.hpp"
#include <mm/components/position2d.hpp>

namespace mini_td::Entities {

void create_enemy(MM::Scene& scene, MM::Entity e, int64_t level, float progress) {
	scene.emplace_or_replace<Components::Enemy>(e, level);
	scene.emplace_or_replace<Components::PathProgress>(e, progress);
	scene.emplace_or_replace<MM::Components::Position2D>(e);
}

void spawn_enemy(MM::Scene& scene, int64_t level, float progress) {
	create_enemy(scene, scene.create(), level, progress);
}

} // mini_td::Entities

