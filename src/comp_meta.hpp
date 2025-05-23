#pragma once

#include <nlohmann/json_fwd.hpp>
#include <entt/entity/fwd.hpp>

namespace mini_td {

void meta_component_emplace(
	entt::registry& scene,
	const entt::id_type comp_type,
	const entt::entity e,
	const nlohmann::json& comp_j
);

} // mini_td

