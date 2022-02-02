#pragma once

#include "./mission.hpp"

#include <memory>

namespace mini_td {

std::unique_ptr<MM::Scene> create_game_scene(MM::Engine& engine, const Mission1& mission);

} // mini_td

