#pragma once

#include <entt/fwd.hpp>
#include <entt/entity/storage.hpp>

#include <mm/engine_fwd.hpp>

#include "../components/enemy.hpp"
#include "../components/path_progress.hpp"
#include "../components/player_lives.hpp"
#include "../components/camera_trauma.hpp"

namespace mini_td::Systems {

// checks if enemies where successful, removes enemy layers from PlayerLives and destroys the enemy
void successful_enemies(
	entt::registry& scene,
	entt::view<entt::get_t<const Components::PathProgress, const Components::Enemy>> view,
	Components::PlayerLives& player_lives,
	Components::CameraTrauma& camera_trauma,
	MM::Engine& engine
);

} // mini_td::Systems

