#pragma once

#include <entt/entity/entity.hpp>

namespace mini_td::Components {

struct TargettingTag_First {};
//struct TargettingTag_Last {};
struct TargettingTag_Strong {};
//struct TargettingTag_Weak {};
struct TargettingTag_FirstBest {}; // best for perf lul

struct Target {
	float range {1.f};
	entt::entity e {entt::null};
};

} // mini_td::Components

