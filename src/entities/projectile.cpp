#include "./projectile.hpp"

#include <entt/entity/registry.hpp>

#include "../components/projectile.hpp"

#include <mm/components/position2d.hpp>
#include <mm/components/view_dir2d.hpp>
#include <mm/components/velocity2d_position.hpp>

#include <glm/trigonometric.hpp>

namespace mini_td::Entities {

void create_projectile(MM::Scene& scene, MM::Entity e, float radius, int64_t damage, glm::vec2 pos, float dir, float velocity) {
	scene.emplace<Components::Projectile>(e, radius, damage, velocity);

	scene.emplace<MM::Components::Position2D>(e, pos);
	scene.emplace<MM::Components::ViewDir2D>(e, dir);

	scene.emplace<MM::Components::Velocity2DPosition>(e);
}

void spawn_projectile(MM::Scene& scene, float radius, int64_t damage, glm::vec2 pos, float dir, float velocity) {
	create_projectile(scene, scene.create(), radius, damage, pos, dir, velocity);
}

} // mini_td::Entities

