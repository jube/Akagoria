cmake_minimum_required(VERSION 3.0)

project(AKAGORIA
  VERSION 0.5.0
  LANGUAGES C CXX
)

include(GNUInstallDirs)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(Threads)
find_package(Boost REQUIRED COMPONENTS filesystem locale serialization system)
find_package(Box2D REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SFML2 REQUIRED sfml-graphics>=2.1 sfml-audio>=2.1)
pkg_check_modules(LIBTMX0 REQUIRED libtmx0>=0.3.1)
pkg_check_modules(YAMLCPP yaml-cpp>=0.5)

add_definitions(-Wall -g -O2)
add_definitions(-std=c++11)

add_subdirectory(code)

install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/data/akagoria"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)

find_package(L10N)

if(L10N_FOUND)
  add_subdirectory(data/akagoria/l10n)
endif()
