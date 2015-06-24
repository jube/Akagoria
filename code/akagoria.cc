/*
 * Akagoria, the revenge of Kalista
 * a single-player RPG in an open world with a top-down view.
 *
 * Copyright (c) 2013-2015, Julien Bernard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <cassert>
#include <cstdio>

#include <tmx/Map.h>

#include "game/Action.h"
#include "game/Camera.h"
#include "game/Clock.h"
#include "game/EntityManager.h"
#include "game/Log.h"
#include "game/ModelManager.h"
#include "game/ResourceManager.h"
#include "game/WindowSettings.h"

#include "akgr/GameEvents.h"
#include "akgr/Hero.h"
#include "akgr/Singletons.h"
#include "akgr/SpriteMap.h"
#include "akgr/TileMap.h"

#include "config.h"


int main(int argc, char *argv[]) {
  game::Log::setLevel(game::Log::INFO);

  // singletons
  game::SingletonStorage<game::ResourceManager> storageForResourceManager(akgr::gResourceManager);
  game::SingletonStorage<game::EventManager> storageForEventManager(akgr::gEventManager);
  game::SingletonStorage<game::EntityManager> storageForMainEntityManager(akgr::gMainEntityManager);
  game::SingletonStorage<game::EntityManager> storageForHeadsUpEntityManager(akgr::gHeadsUpEntityManager);

  game::SingletonStorage<akgr::DataManager> storageForDataManager(akgr::gDataManager);

  game::SingletonStorage<akgr::PhysicsModel> storageForPhysicsModel(akgr::gPhysicsModel);

  // initialize

  static constexpr unsigned INITIAL_WIDTH = 1024;
  static constexpr unsigned INITIAL_HEIGHT = 576;

  game::WindowSettings settings(INITIAL_WIDTH, INITIAL_HEIGHT, "Akagoria (version " GAME_VERSION ")");

  sf::RenderWindow window;
  settings.applyTo(window);
  window.setKeyRepeatEnabled(false);

  // load resources
  akgr::gResourceManager().addSearchDir(GAME_DATADIR);
  akgr::gDataManager().load(GAME_DATADIR);


  // add cameras
  game::CameraManager cameras;

  game::FlexibleCamera mainCamera(INITIAL_WIDTH);
  cameras.addCamera(mainCamera);
  akgr::gEventManager().registerHandler<akgr::HeroLocationEvent>([&mainCamera](game::EventType type, game::Event *event) {
    auto heroLocation = static_cast<akgr::HeroLocationEvent*>(event);
    mainCamera.setCenter(heroLocation->loc.pos);
    return game::EventStatus::KEEP;
  });

  game::HeadsUpCamera headsUpCamera(window);
  cameras.addCamera(headsUpCamera);

  // add actions
  game::ActionManager actions;

  game::Action closeWindowAction("Close window");
  closeWindowAction.addCloseControl();
  closeWindowAction.addKeyControl(sf::Keyboard::Escape);
  actions.addAction(closeWindowAction);

  game::Action fullscreenAction("Fullscreen");
  fullscreenAction.addKeyControl(sf::Keyboard::F);
  actions.addAction(fullscreenAction);

  game::Action leftAction("Left");
  leftAction.addKeyControl(sf::Keyboard::Left);
  leftAction.setContinuous();
  actions.addAction(leftAction);

  game::Action rightAction("Right");
  rightAction.addKeyControl(sf::Keyboard::Right);
  rightAction.setContinuous();
  actions.addAction(rightAction);

  game::Action upAction("Up");
  upAction.addKeyControl(sf::Keyboard::Up);
  upAction.setContinuous();
  actions.addAction(upAction);

  game::Action downAction("Down");
  downAction.addKeyControl(sf::Keyboard::Down);
  downAction.setContinuous();
  actions.addAction(downAction);


  // add entities
  game::ModelManager models;
  models.addModel(akgr::gPhysicsModel());

  akgr::TileMap groundMap(-30);
  akgr::gMainEntityManager().addEntity(groundMap);

  akgr::TileMap loTileMap(-20);
  akgr::gMainEntityManager().addEntity(loTileMap);

  akgr::SpriteMap loSpriteMap(-10);
  akgr::gMainEntityManager().addEntity(loSpriteMap);

  akgr::TileMap hiTileMap(10);
  akgr::gMainEntityManager().addEntity(hiTileMap);

  akgr::SpriteMap hiSpriteMap(20);
  akgr::gMainEntityManager().addEntity(hiSpriteMap);

  {
    auto path = akgr::gResourceManager().getAbsolutePath("maps/map.tmx");
    auto map = tmx::Map::parseFile(path);

    groundMap.loadMap(*map, "ground");
    loTileMap.loadMap(*map, "low_tile");
    hiTileMap.loadMap(*map, "high_tile");

    loSpriteMap.loadMap(*map, "low_sprite");
    hiSpriteMap.loadMap(*map, "high_sprite");
  }

  akgr::Hero hero(4000, 4000, 0);
  akgr::gMainEntityManager().addEntity(hero);
  hero.broadcastLocation();

  // main loop
  game::Clock clock;

  while (window.isOpen()) {
    // input
    sf::Event event;

    while (window.pollEvent(event)) {
      actions.update(event);
      cameras.update(event);
    }

    if (closeWindowAction.isActive()) {
      window.close();
    }

    if (fullscreenAction.isActive()) {
      settings.toggleFullscreen();
      settings.applyTo(window);
      auto sz = window.getSize();

      // fake resize event (not sent when going fullscreen before SFML 2.3.1)
      sf::Event event;
      event.type = sf::Event::Resized;
      event.size.width = sz.x;
      event.size.height = sz.y;
      cameras.update(event);
    }

    if (leftAction.isActive()) {
      hero.turnLeft();
    } else if (rightAction.isActive()) {
      hero.turnRight();
    } else {
      hero.stopTurning();
    }

    if (upAction.isActive()) {
      hero.walkForward();
    } else if (downAction.isActive()) {
      hero.walkBackward();
    } else {
      hero.stopWalking();
    }

    // update
    auto dt = clock.restart().asSeconds();

    models.update(dt);

    akgr::gMainEntityManager().update(dt);
    akgr::gHeadsUpEntityManager().update(dt);

    // render
    window.clear(sf::Color::White);

    mainCamera.configure(window);
    akgr::gMainEntityManager().render(window);

    headsUpCamera.configure(window);
    akgr::gHeadsUpEntityManager().render(window);

    window.display();

    actions.reset();
  }

  return 0;
}
