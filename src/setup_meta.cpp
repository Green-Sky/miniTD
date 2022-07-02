#include "./setup_meta.hpp"

#include <mm/components/position2d.hpp>
#include <mm/components/serialize/json_position2d.hpp>
#include "./components/enemy.hpp"
#include "./components/path_progress.hpp"
#include "./components/tower_art.hpp"

#include <nlohmann/json.hpp>

#include <entt/entity/registry.hpp>
#include <entt/meta/factory.hpp>

namespace mini_td {

// components
template<typename T>
bool component_emplace_json(entt::registry& scene, const nlohmann::json& j, entt::entity e) {
	if constexpr (std::is_empty_v<T>) {
		scene.emplace_or_replace<T>(e); // assert empty json?
	} else {
		scene.emplace_or_replace<T>(e, static_cast<T>(j)); // bc events
	}
	return true;
}

template<typename T>
bool component_get_json(const entt::registry& scene, nlohmann::json& j, entt::entity e) {
	if (scene.all_of<T>(e)) {
		if constexpr (!std::is_empty_v<T>) {
			j = scene.get<T>(e);
		}
		return true;
	}

	return false;
}


void setup_meta_comps(void) {
	using namespace entt::literals;

#define SETUP_COMP_TYPE(x) \
	entt::meta<x>() \
		.type(entt::hashed_string::value(#x)) \
		.func<&component_emplace_json<x>>("component_emplace_json"_hs) \
		.func<&component_get_json<x>>("component_get_json"_hs);

	SETUP_COMP_TYPE(MM::Components::Position2D); // shoule we skip for enemies?
	SETUP_COMP_TYPE(Components::Enemy);
	SETUP_COMP_TYPE(Components::PathProgress);
	SETUP_COMP_TYPE(Components::TowerArtType1);
	SETUP_COMP_TYPE(Components::TowerArtType2);
	SETUP_COMP_TYPE(Components::TowerArtType3);
	SETUP_COMP_TYPE(Components::TowerArtType4);
}

} // mini_td

