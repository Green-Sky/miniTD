#pragma once

#include <mm/engine_fwd.hpp>
#include <mm/services/service.hpp>

#include <entt/entity/entity.hpp>

// fwd
struct ImVec2;

namespace mini_td::Services {

class GameHUD : public MM::Services::Service {
	private: // service interface
		const char* name(void) override { return "mini_td::GameHUD"; }

		bool enable(MM::Engine& engine, std::vector<MM::UpdateStrategies::TaskInfo>& task_array) override;
		void disable(MM::Engine& engine) override;

	private: // tasks
		void update(MM::Engine& engine);

		void updateCamera(MM::Scene& scene, float fractional_offset);

		// true = rect
		void drawTower(bool outer, bool inner, const ImVec2& size, const ImVec2& pos) const;
		bool towerButton(const char* title, bool outer, bool inner, const ImVec2& size) const ;

		void updateTowerPreview(MM::Engine& engine);

		bool _toolbar {true};

		entt::entity _tower_placement_preview {entt::null};
};

} // mini_td::Services

