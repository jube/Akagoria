include_directories(${Boost_INCLUDE_DIRS})
include_directories(${BOX2D_INCLUDE_DIRS})
include_directories(${SFML2_INCLUDE_DIRS})
include_directories(${LIBTMX0_INCLUDE_DIRS})
include_directories(${YAMLCPP_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(akagoria
  akagoria.cc

  # gameskel base
  game/AssetManager.cc
  game/Clock.cc
  game/EventManager.cc
  game/Log.cc
  game/Random.cc
  # gameskel graphics
  game/Action.cc
  game/Animation.cc
  game/Camera.cc
  game/Control.cc
  game/Entity.cc
  game/EntityManager.cc
  game/ResourceManager.cc
  game/WindowSettings.cc
  game/WindowGeometry.cc
  # gameskel model
  game/Model.cc
  game/ModelManager.cc

  # akagoria
  akgr/Body.cc
  akgr/Character.cc
  akgr/DataManager.cc
  akgr/DialogManager.cc
  akgr/FloorTracker.cc
  akgr/GameDriver.cc
  akgr/GridMap.cc
  akgr/Hero.cc
  akgr/HeroAttributes.cc
  akgr/MessageManager.cc
  akgr/PhysicsModel.cc
  akgr/RequirementManager.cc
  akgr/SavePointManager.cc
  akgr/ShrineManager.cc
  akgr/Singletons.cc
  akgr/SpriteMap.cc
  akgr/StartDriver.cc
  akgr/Story.cc
  akgr/TileMap.cc
  akgr/UI.cc
)

target_link_libraries(akagoria
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES}
  ${BOX2D_LIBRARIES}
  ${SFML2_LIBRARIES}
  ${LIBTMX0_LIBRARIES}
  ${YAMLCPP_LIBRARIES}
)

install(
  TARGETS akagoria
  RUNTIME DESTINATION games
)

add_executable(akagoria_dialog_validator
  akagoria_dialog_validator.cc
)

target_link_libraries(akagoria_dialog_validator
  ${Boost_LIBRARIES}
  ${SFML2_LIBRARIES}
  ${YAMLCPP_LIBRARIES}
)
