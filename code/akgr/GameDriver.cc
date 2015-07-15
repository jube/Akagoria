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
#include "GameDriver.h"

#include "GameEvents.h"
#include "Singletons.h"

namespace akgr {

  GameDriver::GameDriver(game::Action& upAction, game::Action& downAction)
  : m_mode(Mode::WALK)
  , m_upAction(upAction)
  , m_downAction(downAction)
  , m_currentUI(&m_heroUI)
  {
    gEventManager().registerHandler<DialogEndEvent>(&GameDriver::onDialogEnd, this);
  }

  void GameDriver::onHorizontalAction(HorizontalAction action) {
    m_currentUI->onHorizontalAction(action);
  }

  void GameDriver::onVerticalAction(VerticalAction action) {
    m_currentUI->onVerticalAction(action);
  }

  void GameDriver::onUse() {
    switch (m_mode) {
      case Mode::WALK:
        if (gDialogManager().hasNextLine()) {
          gDialogManager().showNextLine();
        } else {
          UseEvent event;
          event.loc = gHero().getLocation();
          event.kind = UseEvent::NONE;
          gEventManager().triggerEvent(&event);

          if (event.kind == UseEvent::TALK) {
            m_mode = Mode::TALK;
          }
        }
        break;

      case Mode::TALK:
        if (gDialogManager().hasNextLine()) {
          gDialogManager().showNextLine();
        }
        break;

      case Mode::SAVE:

        break;
    }
  }

  void GameDriver::update(float dt) {
    if (m_mode != Mode::WALK) {
      gHero().stopWalking();
    }

    m_currentUI->update(dt);
  }

  void GameDriver::render(sf::RenderWindow& window) {
    m_currentUI->render(window);
  }

  void GameDriver::setArrowActionsContinuous() {
    m_upAction.setContinuous();
    m_downAction.setContinuous();
  }

  void GameDriver::setArrowActionsInstantaneous() {
    m_upAction.setInstantaneous();
    m_downAction.setInstantaneous();
  }

  game::EventStatus GameDriver::onDialogEnd(game::EventType type, game::Event *event) {
    if (m_mode == Mode::TALK) {
      m_mode = Mode::WALK;
    }

    return game::EventStatus::KEEP;
  }

}
