#pragma once

#include <glm/vec2.hpp>
#include <cstdint>
#include <vector>

#include <nlohmann/json.hpp>
#include <mm/components/serialize/json_glm.hpp>

namespace mini_td::Components {

struct Path {
	std::vector<glm::vec2> points {}; // point strip
	glm::vec2 extents {};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Path, points, extents)

struct PathUtil {
	PathUtil(void) = default;
	explicit PathUtil(const Path& path);

	std::vector<float> point_progress {}; // at how much progress the point starts
	float total_length {};
};

} // mini_td::Components

