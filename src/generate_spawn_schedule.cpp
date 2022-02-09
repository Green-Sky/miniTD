#include "./components/spawn_group.hpp"
#include "./components/spawn_schedule.hpp"

#include <iostream>

using mini_td::Components::SpawnGroup;
using mini_td::Components::SpawnSchedule;

int main(int argc, char** argv) {
	// loosely based on bloons
	SpawnSchedule ss {};

	uint64_t curr_wave = 0;
	{ // easy (1-32)
		ss.schedule[++curr_wave] = {
			{
				1, // lvl
				32, // count
				1.5f, // interval
				0.f // inital_time_accu
			},
		};
		ss.schedule[++curr_wave] = {
			{
				1, // lvl
				32+16, // count
				1.3f, // interval
				0.f // inital_time_accu
			},
		};
		ss.schedule[++curr_wave] = {
			{
				1, // lvl
				32, // count
				1.3f, // interval
				0.f // inital_time_accu
			},
			{
				2, // lvl
				8, // count
				2.f, // interval
				-6.f // inital_time_accu
			},
		};
		ss.schedule[++curr_wave] = {
			{
				1, // lvl
				32, // count
				1.3f, // interval
				0.f // inital_time_accu
			},
			{
				1, // lvl
				16, // count
				0.7f, // interval
				0.f // inital_time_accu
			},
			{
				2, // lvl
				8, // count
				2.f, // interval
				-6.f // inital_time_accu
			},
			{
				2, // lvl
				8, // count
				2.f, // interval
				-32.f // inital_time_accu
			},
		};
		ss.schedule[++curr_wave] = { // 5
			{
				3, // lvl
				16, // count
				1.f, // interval
				0.f // inital_time_accu
			},
			{
				1, // lvl
				16, // count
				0.7f, // interval
				-12.f // inital_time_accu
			},
			{
				2, // lvl
				16, // count
				2.f, // interval
				-16.f // inital_time_accu
			},
		};
		ss.schedule[++curr_wave] = { // 6
			{
				3, // lvl
				48, // count
				0.5f, // interval
				0.f // inital_time_accu
			},
		};
		ss.schedule[++curr_wave] = { // 7
			{
				1, // lvl
				64, // count
				0.2f, // interval
				0.f // inital_time_accu
			},
			{
				2, // lvl
				16, // count
				1.f, // interval
				-14.f // inital_time_accu
			},
			{
				3, // lvl
				16, // count
				1.1f, // interval
				-16.f // inital_time_accu
			},
		};
	}

	{ // mid (33-64)
	}

	{ // hard (65-96)
	}

	// random after (lul)

	//SpawnGroup sg {};
	//sg.count = 32;

	std::cout << nlohmann::json(ss).dump(2);

	// extra analysis
	std::map<uint64_t, uint64_t> worths{};
	for (const auto& [w, sgs] : ss.schedule) {
		auto& worth = worths[w] = 0;
		// sum up worth (enemy lvl * count)
		for (const auto& sg : sgs) {
			worth += sg.count * sg.level;
		}

		if (worths.count(w-1)) {
			if (worths.at(w-1) > worths.at(w)) {
				std::cerr << "wave " << w << " has lower worth then wave before! (" << worths.at(w-1) << " > " << worths.at(w) << "\n";
			}
		}
	}

	return 0;
}

