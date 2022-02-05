#include "./play_sound_on_damage.hpp"

#include <entt/entity/registry.hpp>

#include <mm/services/sound_service.hpp>

#include <mm/resource_manager.hpp>

#include <soloud_sfxr.h>

namespace mini_td::Systems {

void play_sound_on_damage(
	entt::view<entt::get_t<const Components::Damage>> view,
	MM::Engine& engine
) {
	using namespace entt::literals;
	// only play one per tick, thats too much already anyway
	if (!view.empty()) {
		auto& rm = MM::ResourceManager<SoLoud::Sfxr>::ref();
		if (rm.contains("damage"_hs)) {
			auto s = rm.get("damage"_hs);

			auto& s_e = engine.getService<MM::Services::SoundService>().engine;

			s_e.play(*s, 1.0f);
		}
	}
}

} // mini_td::Systems

