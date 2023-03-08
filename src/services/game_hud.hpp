#pragma once

#include <mm/engine_fwd.hpp>
#include <mm/services/service.hpp>

#include <entt/entity/entity.hpp>

#include <chrono>

// fwd
struct ImVec2;

namespace mini_td::Services {

class GameHUD : public MM::Services::Service {
	private: // service interface
		const char* name(void) override { return "mini_td::GameHUD"; }

		bool enable(MM::Engine& engine, std::vector<MM::UpdateStrategies::TaskInfo>& task_array) override;
		void disable(MM::Engine& engine) override;

	public:
		bool debug {false};

	private:
		void update(MM::Engine& engine);

		void updateTimer(void);

		void updateCamera(MM::Scene& scene, float fractional_offset);

		void renderToolbarBuild(MM::Engine& engine);
		void renderToolbarStats(MM::Engine& engine);
		void renderToolbarDebug(MM::Engine& engine);

		// true = rect
		void drawTower(bool outer, bool inner, const ImVec2& size, const ImVec2& pos) const;
		bool towerButton(const char* title, bool outer, bool inner, const ImVec2& size) const ;

		void updateTowerPreview(MM::Engine& engine);

		bool _toolbar {true};

		entt::entity _tower_placement_preview {entt::null};

		using clock = std::chrono::high_resolution_clock;
		std::chrono::time_point<clock> _last_time;

		float _delta {0.f};

		float _screen_shake_time {0.f};
};

} // mini_td::Services

