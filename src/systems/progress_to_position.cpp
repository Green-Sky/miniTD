#include "./progress_to_position.hpp"

#include <entt/entity/registry.hpp>

#include <glm/common.hpp>

namespace mini_td::Systems {

void progress_to_position(
	entt::view<entt::get_t<MM::Components::Position2D, const Components::PathProgress>> view,
	const Components::Path& path,
	const Components::PathUtil& path_util
) {
	view.each([&path, &path_util](auto& pos_comp, const auto& prog) {
		// get the two points

		// TODO: naive aproach
		// TODO: cache last point and start from there
		// (perf only relevant for > 10 points prob)
		size_t p1_i = 0;
		for (size_t i = 1; i < path_util.point_progress.size(); i++) {
			if (prog.progress < path_util.point_progress[i]) {
				p1_i = i-1;
				break;
			}
		}

		// remap to progress on segment
		const float p1_prog = path_util.point_progress[p1_i];
		const float p2_prog = path_util.point_progress[p1_i+1];
		const float segment_progress = (prog.progress - p1_prog) / (p2_prog - p1_prog);
		pos_comp.pos = glm::mix(path.points[p1_i], path.points[p1_i+1], segment_progress);
	});
}

} // mini_td::Systems

