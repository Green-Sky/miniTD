#include "./path.hpp"

#include <glm/geometric.hpp>

#include <cassert>

namespace mini_td::Components {

PathUtil::PathUtil(const Path& path) {
	assert(path.points.size() >= 2);

	// sum of segments
	total_length = 0.f;
	glm::vec2 prev = path.points.front();
	for (size_t i = 1; i < path.points.size(); i++) {
		total_length += glm::distance(prev, path.points[i]);
		prev = path.points[i];
	}

	point_progress.clear();
	point_progress.push_back(0.f);
	float current_length = 0.f;
	prev = path.points.front();
	for (size_t i = 1; i < path.points.size(); i++) {
		current_length += glm::distance(prev, path.points[i]);
		point_progress.push_back(current_length/total_length);
		prev = path.points[i];
	}
}

} // mini_td::Components

