#include "./player_death.hpp"

#include <mm/engine.hpp>

#include <mm/services/screen_director.hpp>

namespace mini_td::Systems {

void player_death(
	Components::PlayerLives& player_lives,
	MM::Engine& engine
) {
	if (player_lives.lives <= 0) {
		// you loose
		// TODO: diff win and fail
		engine.getService<MM::Services::ScreenDirector>().queueChangeScreenTo("mini_td::Screens::end_screen");
	}
}

} // mini_td::Systems

