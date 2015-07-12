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
#include <fstream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

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
#include "akgr/Story.h"
#include "akgr/TileMap.h"
#include "akgr/UI.h"

#include "config.h"

static constexpr unsigned INITIAL_WIDTH = 1280;
static constexpr unsigned INITIAL_HEIGHT = 720;

enum class StartMode {
  MAIN,
  LOAD,
};

int main(int argc, char *argv[]) {
  game::Log::setLevel(game::Log::INFO);

  // singletons
  game::SingletonStorage<game::Random> storageForRandom(akgr::gRandom);
  game::SingletonStorage<game::ResourceManager> storageForResourceManager(akgr::gResourceManager);
  akgr::gResourceManager().addSearchDir(GAME_DATADIR);

  game::SingletonStorage<game::EventManager> storageForEventManager(akgr::gEventManager);
  game::SingletonStorage<game::EntityManager> storageForMainEntityManager(akgr::gMainEntityManager);
  game::SingletonStorage<game::EntityManager> storageForHeadsUpEntityManager(akgr::gHeadsUpEntityManager);

  game::SingletonStorage<akgr::DataManager> storageForDataManager(akgr::gDataManager);

  game::SingletonStorage<akgr::PhysicsModel> storageForPhysicsModel(akgr::gPhysicsModel);

  game::SingletonStorage<akgr::CharacterManager> storageForCharacterManager(akgr::gCharacterManager);
  game::SingletonStorage<akgr::DialogManager> storageForDialogManager(akgr::gDialogManager);
  game::SingletonStorage<akgr::RequirementManager> storageForRequirementManager(akgr::gRequirementManager);
  game::SingletonStorage<akgr::SavePointManager> storageForSavePointManager(akgr::gSavePointManager);
  game::SingletonStorage<akgr::ShrineParticles> storageForShrineParticles(akgr::gShrineParticles);

  game::SingletonStorage<game::WindowGeometry> storageForWindowGeometry(akgr::gWindowGeometry, INITIAL_WIDTH, INITIAL_HEIGHT);



  // initialize window
  game::WindowSettings settings(INITIAL_WIDTH, INITIAL_HEIGHT, "Akagoria (version " GAME_VERSION ")");

  sf::RenderWindow window;
  settings.applyTo(window);
  window.setKeyRepeatEnabled(false);

  // splash screen
  akgr::SplashUI splashUI;

  window.clear(sf::Color::Black);
  splashUI.displaySplashMessage(window, true);
  window.display();

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
  actions.addAction(upAction);

  game::Action downAction("Down");
  downAction.addKeyControl(sf::Keyboard::Down);
  actions.addAction(downAction);

  game::Action useAction("Use");
  useAction.addKeyControl(sf::Keyboard::X);
  actions.addAction(useAction);


  // UI for start screen
  akgr::StartUI startUI;
  akgr::LoadUI loadUI;
  int slotToLoad = -1; // -1 means new game

  StartMode startMode = StartMode::MAIN;
  akgr::EntityUI *currentUI = &startUI;

  // start screen
  game::Clock clock;

  while (window.isOpen()) {
    // input
    sf::Event event;

    while (window.pollEvent(event)) {
      actions.update(event);
      cameras.update(event);

      akgr::gWindowGeometry().update(event);
    }

    if (closeWindowAction.isActive()) {
      window.close();
      return EXIT_SUCCESS;
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
      akgr::gWindowGeometry().update(event);
    }

    if (upAction.isActive()) {
      currentUI->onVerticalAction(akgr::VerticalAction::UP);
    } else if (downAction.isActive()) {
      currentUI->onVerticalAction(akgr::VerticalAction::DOWN);
    } else {
      currentUI->onVerticalAction(akgr::VerticalAction::NONE);
    }

    if (useAction.isActive()) {
      if (startMode == StartMode::MAIN) {
        int choice = startUI.getCurrentChoice();

        if (choice == akgr::StartUI::START_NEW_GAME) {
          break;
        }

        if (choice == akgr::StartUI::LOAD_GAME) {
          startMode = StartMode::LOAD;
          currentUI = &loadUI;
        }

        if (choice == akgr::StartUI::QUIT) {
          window.close();
          return EXIT_SUCCESS;
        }
      } else {
        assert(startMode == StartMode::LOAD);
        int choice = loadUI.getCurrentChoice();

        if (choice == akgr::LoadUI::BACK) {
          startMode = StartMode::MAIN;
          currentUI = &startUI;
        } else {
          if (akgr::gSavePointManager().hasSlot(choice)) {
            slotToLoad = choice;
            break;
          }
        }
      }
    }

    // update
    auto dt = clock.restart().asSeconds();
    splashUI.update(dt);
    startUI.update(dt);

    // render
    window.clear(sf::Color::Black);

    headsUpCamera.configure(window);
    splashUI.render(window);

    if (startMode == StartMode::MAIN) {
      startUI.render(window);
    } else {
      loadUI.render(window);
    }

    window.display();

    actions.reset();
  }

  window.clear(sf::Color::Black);
  splashUI.displaySplashMessage(window, true);
  window.display();

  // load data
  akgr::gDataManager().load(GAME_DATADIR);

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

    akgr::gPhysicsModel().loadMap(*map);
    akgr::gDataManager().loadMap(*map);
  }

  upAction.setContinuous();
  downAction.setContinuous();

  // hero
  auto startLocation = akgr::gDataManager().getPointOfInterestDataFor("Start");
  assert(startLocation);
  game::SingletonStorage<akgr::Hero> storageForHero(akgr::gHero, startLocation->loc);
  akgr::gMainEntityManager().addEntity(akgr::gHero());


  if (slotToLoad != -1) {
    akgr::gSavePointManager().loadFromSlot(slotToLoad);
  } else {
    akgr::gRequirementManager().addRequirement("IntroDialogReq"_id);

    // another character
    auto shagirLocation = akgr::gDataManager().getPointOfInterestDataFor("Shagir");
    assert(shagirLocation);
    auto shagirCharacter = akgr::gCharacterManager().addCharacter("Shagir", shagirLocation->loc, 0.5f);
    shagirCharacter->attachDialog("ShagirConversation0");
  }

  akgr::gHero().broadcastLocation();
  akgr::gCharacterManager().updateCharacterSearch();

  akgr::gMainEntityManager().addEntity(akgr::gCharacterManager());
  akgr::gMainEntityManager().addEntity(akgr::gShrineParticles());

  akgr::gHeadsUpEntityManager().addEntity(akgr::gDialogManager());

  akgr::Story story;

  akgr::HeroUI heroUI;
  currentUI = &heroUI;

  // main loop
  while (window.isOpen()) {
    // input
    sf::Event event;

    while (window.pollEvent(event)) {
      actions.update(event);
      cameras.update(event);

      akgr::gWindowGeometry().update(event);
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
      akgr::gWindowGeometry().update(event);
    }

    if (leftAction.isActive()) {
      currentUI->onHorizontalAction(akgr::HorizontalAction::LEFT);
    } else if (rightAction.isActive()) {
      currentUI->onHorizontalAction(akgr::HorizontalAction::RIGHT);
    } else {
      currentUI->onHorizontalAction(akgr::HorizontalAction::NONE);
    }

    if (upAction.isActive()) {
      currentUI->onVerticalAction(akgr::VerticalAction::UP);
    } else if (downAction.isActive()) {
      currentUI->onVerticalAction(akgr::VerticalAction::DOWN);
    } else {
      currentUI->onVerticalAction(akgr::VerticalAction::NONE);
    }

    if (useAction.isActive()) {
      if (akgr::gDialogManager().hasNextLine()) {
        akgr::gDialogManager().showNextLine();
      } else {
        akgr::gHero().tryToTalk();
      }
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
