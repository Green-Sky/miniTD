#include "./projectile_bounds_check.hpp"

#include <entt/entity/registry.hpp>

namespace mini_td::Systems {

void projectile_bounds_check(
	entt::registry& scene,
	entt::view<entt::get_t<const Components::Projectile, const MM::Components::Position2D>> view,
	const Components::Path& path
) {
	std::vector<entt::entity> to_delete {};
	view.each([&to_delete, &path](const auto e, const auto&, const auto& pos_comp) {
		if (
			pos_comp.pos.x < 0.f || pos_comp.pos.y < 0.f ||
			pos_comp.pos.x > path.extents.x || pos_comp.pos.y > path.extents.y
		) {
			to_delete.push_back(e);
		}
	});

	scene.destroy(to_delete.cbegin(), to_delete.cend());
}

} // mini_td::Systems

