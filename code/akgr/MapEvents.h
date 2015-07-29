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
#ifndef AKGR_MAP_EVENTS_H
#define AKGR_MAP_EVENTS_H

#include <game/Event.h>

namespace akgr {

  struct ViewUpEvent : public game::Event {
    static const game::EventType type = "ViewUpEvent"_type;
  };

  struct ViewDownEvent : public game::Event {
    static const game::EventType type = "ViewDownEvent"_type;
  };

  struct ViewInsideEvent : public game::Event {
    static const game::EventType type = "ViewInsideEvent"_type;
  };

  struct ViewOutsideEvent : public game::Event {
    static const game::EventType type = "ViewOutsideEvent"_type;
  };

  struct MoveUpEvent : public game::Event {
    static const game::EventType type = "MoveUpEvent"_type;
  };

  struct MoveDownEvent : public game::Event {
    static const game::EventType type = "MoveDownEvent"_type;
  };

  struct MoveInsideEvent : public game::Event {
    static const game::EventType type = "MoveInsideEvent"_type;
  };

  struct MoveOutsideEvent : public game::Event {
    static const game::EventType type = "MoveOutsideEvent"_type;
  };

}


#endif // AKGR_MAP_EVENTS_H
