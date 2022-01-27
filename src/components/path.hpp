#pragma once

#include <glm/vec2.hpp>
#include <cstdint>
#include <vector>

namespace mini_td::Components {

struct Path {
	std::vector<glm::vec2> points {}; // point strip
	std::vector<float> point_progress {}; // at how much progress the point starts
	glm::vec2 extents {};

	float total_length {};

	void updateValues(void);
};

} // mini_td::Components

