# CMake References: https://github.com/Lectem/cpp-boilerplate/blob/master/CMakeLists.txt

cmake_minimum_required(VERSION 3.17)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

project(aoc2022
    VERSION 0.1.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
if(APPLE)
add_compile_options(-mmacosx-version-min=10.15)
endif()
add_compile_options(-Wall -Wextra -pedantic -Werror=int-in-bool-context)


if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

# Standard CMake modules
include(CTest)
include(CMakeDependentOption)
include(GNUInstallDirs)
include(InstallRequiredSystemLibraries)
include(CMakePackageConfigHelpers)

# Custom modules and scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake") # Make our cmake scripts available
include(BuildHelpers)  # Helpers for writing cmake files

option(ENABLE_TESTING "Should unit tests be compiled." ON)

set(${PROJECT_NAME}_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    CACHE STRING "Path to install ${PROJECT_NAME} Config*.cmake files to.")

####################
##  Dependencies  ##
####################

if(ENABLE_TESTING)
    find_package(GTest REQUIRED)
    include(GoogleTest)
    enable_testing()
endif()

###############
##  Targets  ##
###############

add_subdirectory(day1)
add_subdirectory(day2)
add_subdirectory(day3)
add_subdirectory(day4)
add_subdirectory(day5)
add_subdirectory(day6)
add_subdirectory(day7)
add_subdirectory(day8)
add_subdirectory(day9)
add_subdirectory(day10)
add_subdirectory(day11)
add_subdirectory(day12)
add_subdirectory(day13)
add_subdirectory(day14)
add_subdirectory(day15)
add_subdirectory(day16)
add_subdirectory(day17)
add_subdirectory(day18)
add_subdirectory(day19)
add_subdirectory(day20)
