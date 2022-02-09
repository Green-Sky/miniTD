#pragma once

#include <mm/engine_fwd.hpp>

#include "../components/player_lives.hpp"

namespace mini_td::Systems {

void player_death(
	Components::PlayerLives& player_lives,
	MM::Engine& engine
);

} // mini_td::Systems

