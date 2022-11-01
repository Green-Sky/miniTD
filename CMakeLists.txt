cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(miniTD CXX C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# add this to your projects cmake to enable ipo
if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
	include(CheckIPOSupported)
	check_ipo_supported(RESULT HAVE_LTO)
	if(HAVE_LTO)
		set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
		message(STATUS "Enabled LTO")
	else()
		message(STATUS "LTO not supported")
	endif()
endif()


option(BUILD_TESTING "Enable testing with ctest." OFF)

include("${CMAKE_CURRENT_LIST_DIR}/external/MushMachine/mm_options_and_defines.cmake")
add_subdirectory("external/MushMachine" EXCLUDE_FROM_ALL)

add_subdirectory("external/fx_draw" EXCLUDE_FROM_ALL)

# Bump up warning levels appropriately for clang, gcc & msvc
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

	#link_libraries(-fsanitize=address)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
endif()

add_subdirectory("src")

