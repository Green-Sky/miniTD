#include "./comp_meta.hpp"

#include <entt/meta/resolve.hpp>
#include <entt/entity/registry.hpp>

#include <nlohmann/json.hpp>

#include <mm/logger.hpp>

namespace mini_td {

void meta_component_emplace(
	entt::registry& scene,
	const entt::id_type comp_type,
	const entt::entity e,
	const nlohmann::json& comp_j
) {
	using namespace entt::literals;

	auto type = entt::resolve(comp_type);
	if (!type) {
		SPDLOG_ERROR("unregistred component type");
		return;
	}

	auto fn = type.func("component_emplace_json"_hs);
	if (!fn) {
		SPDLOG_ERROR("component type with missing 'component_emplace_json' function");
		return;
	}

	auto ret = fn.invoke({}, entt::forward_as_meta(scene), entt::forward_as_meta(comp_j), e);
	if (!ret) {
		SPDLOG_ERROR("'component_emplace_json' function invoke failed");
		return;
	}
}

} // mini_td

