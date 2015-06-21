cmake_minimum_required(VERSION 3.0)

project(AKAGORIA
  VERSION 0.5.0
  LANGUAGES C CXX
)

include(GNUInstallDirs)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(Threads)
find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(Box2D REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SFML2 REQUIRED sfml-graphics>=2.1 sfml-audio>=2.1)

add_definitions(-Wall -g -O2)
add_definitions(-std=c++11)

add_subdirectory(code)
