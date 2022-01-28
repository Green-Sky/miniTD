#include "./spawn_group.hpp"

#include <entt/entity/registry.hpp>

#include "../components/spawn_group.hpp"

namespace mini_td::Entities {

void create_spawn_group(MM::Scene& scene, MM::Entity e, int64_t level, int64_t count, float interval, float inital_time_accu) {
	auto& sg = scene.emplace<Components::SpawnGroup>(e);
	sg.level = level;
	sg.count = count;
	sg.interval = interval;
	sg.time_accu = inital_time_accu;
	sg.spawned_count = 0;
}

void spawn_spawn_group(MM::Scene& scene, int64_t level, int64_t count, float interval, float inital_time_accu) {
	create_spawn_group(scene, scene.create(), level, count, interval, inital_time_accu);
}

} // mini_td::Entities

