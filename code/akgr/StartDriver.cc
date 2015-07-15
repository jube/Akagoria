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
#include "StartDriver.h"

#include <cassert>

#include "Singletons.h"

namespace akgr {

  StartDriver::StartDriver()
  : m_currentUI(&m_startUI)
  , m_slotToLoad(-1) // -1 means new game
  {

  }

  void StartDriver::onVerticalAction(VerticalAction action) {
    m_currentUI->onVerticalAction(action);
  }

  StartChoice StartDriver::onUse() {
    switch (m_mode) {
      case Mode::MAIN:
        switch (m_startUI.getCurrentChoice()) {
          case StartUI::CHOICE_NEW:
            assert(m_slotToLoad == -1);
            return StartChoice::START_NEW_GAME;

          case StartUI::CHOICE_LOAD:
            m_mode = Mode::LOAD;
            m_currentUI = &m_loadUI;
            break;

          case StartUI::CHOICE_QUIT:
            return StartChoice::QUIT;

          default:
            assert(false);
            break;
        }
      break;

      case Mode::LOAD: {
        int choice = m_loadUI.getCurrentChoice();
        switch (choice) {
          case LoadUI::CHOICE_BACK:
            m_mode = Mode::MAIN;
            m_currentUI = &m_startUI;
            break;

          default:
            assert(0 <= LoadUI::CHOICE_SLOT0 && choice <= LoadUI::CHOICE_SLOT2);
            if (gSavePointManager().hasSlot(choice)) {
              m_slotToLoad = choice;
              return StartChoice::LOAD_SLOT;
            }
            break;
        }
      }
      break;

    }

    return StartChoice::NONE;
  }

  void StartDriver::update(float dt) {
    m_currentUI->update(dt);
  }

  void StartDriver::render(sf::RenderWindow& window) {
    m_currentUI->render(window);
  }

}
