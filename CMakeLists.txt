cmake_minimum_required(VERSION 3.0)

project(AKAGORIA
  VERSION 0.5.0
  LANGUAGES C CXX
)

include(GNUInstallDirs)

find_package(Threads)
find_package(Boost REQUIRED COMPONENTS filesystem system)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SFML2 REQUIRED sfml-graphics>=2.1 sfml-audio>=2.1)

add_definitions(-Wall -g -O2)
add_definitions(-std=c++11)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SFML2_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(akagoria
  akagoria.cc

  # base
  game/AssetManager.cc
  game/Clock.cc
  game/EventManager.cc
  game/Log.cc
  game/Random.cc
  # graphics
  game/Action.cc
  game/Animation.cc
  game/Camera.cc
  game/Control.cc
  game/Entity.cc
  game/EntityManager.cc
  game/ResourceManager.cc
  game/WindowSettings.cc
  # model
  game/Model.cc
  game/ModelManager.cc

  # akagoria
  akgr/PhysicsModel.cc
  akgr/Singletons.cc
)

target_link_libraries(akagoria
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES}
  ${SFML2_LIBRARIES}
)

install(
  TARGETS akagoria
  RUNTIME DESTINATION games
)

install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/data/akagoria"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)
