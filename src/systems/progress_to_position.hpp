#pragma once

#include <entt/fwd.hpp>
#include <entt/entity/storage.hpp>

#include <mm/components/position2d.hpp>
#include "../components/path_progress.hpp"
#include "../components/path.hpp"

namespace mini_td::Systems {

void progress_to_position(
	entt::view<entt::get_t<MM::Components::Position2D, const Components::PathProgress>> view,
	const Components::Path& path,
	const Components::PathUtil& path_util
);

} // mini_td::Systems

