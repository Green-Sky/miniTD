#pragma once

#include <entt/fwd.hpp>
#include <entt/entity/storage.hpp>

#include <mm/engine_fwd.hpp>

#include "../components/damage.hpp"

namespace mini_td::Systems {

void play_sound_on_damage(
	entt::view<entt::get_t<const Components::Damage>> view,
	MM::Engine& engine
);

} // mini_td::Systems

