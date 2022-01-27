#pragma once

#include <entt/fwd.hpp>

#include "../components/path_progress.hpp"
#include "../components/enemy.hpp"
#include "../components/path.hpp"
#include <mm/components/time_delta.hpp>

namespace mini_td::Systems {

void progress_enemies(
	entt::view<entt::get_t<Components::PathProgress, const Components::Enemy>> view,
	const Components::Path& path,
	const MM::Components::TimeDelta& td
);

} // mini_td::Systems

