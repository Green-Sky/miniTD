#pragma once

#include <mm/services/screen_director.hpp>

namespace mini_td::Screens {

void create_game_mp_host(MM::Engine& engine, MM::Services::ScreenDirector::Screen& screen);
void create_game_mp_client(MM::Engine& engine, MM::Services::ScreenDirector::Screen& screen);

} // mini_td::Screens

