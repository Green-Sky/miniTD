#include "./projectile_velocity.hpp"

#include <entt/entity/registry.hpp>

#include <glm/trigonometric.hpp>

namespace mini_td::Systems {

// viewdir and velocity -> mm Velocity2DPosition
void projectile_velocity(
	entt::view<entt::get_t<const Components::Projectile, const MM::Components::ViewDir2D, MM::Components::Velocity2DPosition>> view
) {
	view.each([](const auto& pj, const auto& view_dir, auto& vel) {
		vel.pos_vel = glm::vec2{glm::cos(view_dir.dir), glm::sin(view_dir.dir)} * pj.velocity;
	});
}

} // mini_td::Systems

