#include "./path.hpp"

#include <glm/geometric.hpp>

#include <cassert>

namespace mini_td::Components {

void Path::updateValues(void) {
	assert(points.size() >= 2);

	// start and end should be outside the bounds by 1m

	//// establish bounds
	//glm::vec2 min = points[0];
	//glm::vec2 max = points[0];
	//for (size_t i = 1; i < points.size(); i++) {
		//auto& point = points[i];
		//if (min.x > point.x) {
			//min.x = point.x;
		//}
		//if (max.x < point.x) {
			//max.x = point.x;
		//}
		//if (min.y > point.y) {
			//min.y = point.y;
		//}
		//if (max.y < point.y) {
			//max.y = point.y;
		//}
	//}

	//// assert first and last on bounds
	//assert(points.front().x == min.x || points.front().x == max.x || points.front().y == min.y || points.front().y == max.y);
	//assert(points.back().x == min.x || points.back().x == max.x || points.back().y == min.y || points.back().y == max.y);

	//// offset all points by min
	//for (auto& point : points) {
		//point -= min;
	//}

	//extents = max - min;

	// sum of segments
	total_length = 0.f;
	glm::vec2 prev = points.front();
	for (size_t i = 1; i < points.size(); i++) {
		total_length += glm::distance(prev, points[i]);
		prev = points[i];
	}

	point_progress.clear();
	point_progress.push_back(0.f);
	float current_length = 0.f;
	prev = points.front();
	for (size_t i = 1; i < points.size(); i++) {
		current_length += glm::distance(prev, points[i]);
		point_progress.push_back(current_length/total_length);
		prev = points[i];
	}
}

} // mini_td::Components

