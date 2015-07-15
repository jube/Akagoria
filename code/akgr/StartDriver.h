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
#ifndef AKGR_START_DRIVER_H
#define AKGR_START_DRIVER_H

#include "UI.h"

namespace akgr {

  enum class StartChoice {
    NONE,
    START_NEW_GAME,
    LOAD_SLOT,
    QUIT,
  };

  class StartDriver {
  public:
    StartDriver();

    int getSlotToLoad() const {
      return m_slotToLoad;
    }

    void onVerticalAction(VerticalAction action);
    StartChoice onUse();

    void update(float dt);
    void render(sf::RenderWindow& window);

  private:
    enum class Mode {
      MAIN,
      LOAD,
    };

    Mode m_mode;

    StartUI m_startUI;
    LoadUI m_loadUI;
    EntityUI *m_currentUI;

    int m_slotToLoad;

  };

}



#endif // AKGR_START_DRIVER_H
