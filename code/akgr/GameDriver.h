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
#ifndef AKGR_GAME_DRIVER_H
#define AKGR_GAME_DRIVER_H

#include <game/Action.h>
#include <game/Event.h>

#include "UI.h"

namespace akgr {

  class GameDriver {
  public:
    GameDriver(game::Action& upAction, game::Action& downAction);

    void onHorizontalAction(HorizontalAction action);
    void onVerticalAction(VerticalAction action);

    void onUse();

    void update(float dt);
    void render(sf::RenderWindow& window);

  private:
    void setArrowActionsContinuous();
    void setArrowActionsInstantaneous();

  private:
    enum class Mode {
      WALK,
      TALK,
      SAVE,
    };

    Mode m_mode;

    game::Action& m_upAction;
    game::Action& m_downAction;

    HeroUI m_heroUI;
    EntityUI *m_currentUI;

  private:
    game::EventStatus onDialogEnd(game::EventType type, game::Event *event);
  };


}


#endif // AKGR_GAME_DRIVER_H
