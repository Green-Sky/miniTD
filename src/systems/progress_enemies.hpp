#pragma once

#include <entt/fwd.hpp>
#include <entt/entity/storage.hpp>

#include "../components/path_progress.hpp"
#include "../components/enemy.hpp"
#include "../components/path.hpp"
#include <mm/components/time_delta.hpp>

namespace mini_td::Systems {

void progress_enemies(
	entt::view<entt::get_t<Components::PathProgress, const Components::Enemy>> view,
	const Components::PathUtil& path_util,
	const MM::Components::TimeDelta& td
);

} // mini_td::Systems

