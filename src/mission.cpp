#include "./mission.hpp"

#include <mm/engine.hpp>
#include <mm/services/filesystem.hpp>

#include <mm/logger.hpp>

#include <cassert>

namespace mini_td {

Mission1 load_mission1(MM::Engine& engine, const std::string& path) {
	auto& fs = engine.getService<MM::Services::FilesystemService>();
	if (!fs.isFile(path.c_str())) {
		SPDLOG_ERROR("mission file does not exist {}", path);
		// TODO: exception
		assert(false); // hehe
	}

	return fs.readJson(path.c_str());
}

} // mini_td

