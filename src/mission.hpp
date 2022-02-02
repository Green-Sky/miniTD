#pragma once

#include <mm/engine_fwd.hpp>

#include <nlohmann/json.hpp>

namespace mini_td {

struct Mission1 {
	std::string title {"UN_NAMED"};
	std::string map_file_path {}; // path to path

	int64_t starting_lives {100};
	int64_t starting_money {100};

	// limit tower selection?

	// wave
	int64_t money_per_wave {50};
	int64_t starting_wave {1};
	int64_t final_wave {-1}; // negative for inf
	std::string spawn_schedule_file_path {};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Mission1,
	title,
	map_file_path,

	starting_lives,
	starting_money,

	money_per_wave,
	starting_wave,
	final_wave,
	spawn_schedule_file_path
)

Mission1 load_mission1(MM::Engine& engine, const std::string& path);

} // mini_td

