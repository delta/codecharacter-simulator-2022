cmake_minimum_required(VERSION 3.15.0)
project(simulator)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fsanitize=address -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# linter modules go here
set(CMAKE_SCRIPTS_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Conan setup for dependecies
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)
conan_basic_setup()

# Always enable colors, since Ninja strips them
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options(-fdiagnostics-color=always)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_compile_options(-fcolor-diagnostics)
endif()

# enable cache system
include(${CMAKE_SCRIPTS_PATH}/cache.cmake)

# Project building options
set(BUILD_PROJECT
    "all"
    CACHE STRING "Set the name of the project to build")

if(NOT BUILD_PROJECT STREQUAL "no_tests")
  include(${CMAKE_SCRIPTS_PATH}/clang-format.cmake)
  include(${CMAKE_SCRIPTS_PATH}/cmake-format.cmake)
endif()

if(BUILD_PROJECT STREQUAL "simulator")

elseif(BUILD_PROJECT STREQUAL "no_tests")
  add_subdirectory(src)
else()
  add_subdirectory(src)
  add_subdirectory(test)
endif()
