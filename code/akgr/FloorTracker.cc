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
#include "FloorTracker.h"

#include "GameEvents.h"
#include "MapEvents.h"
#include "Singletons.h"

namespace akgr {

  FloorTracker::FloorTracker()
  : m_floor(0)
  {
    gEventManager().registerHandler<HeroLocationEvent>(&FloorTracker::onHeroLocation, this);
    gEventManager().registerHandler<ViewUpEvent>(&FloorTracker::onView, this);
    gEventManager().registerHandler<ViewDownEvent>(&FloorTracker::onView, this);
    gEventManager().registerHandler<ViewInsideEvent>(&FloorTracker::onView, this);
    gEventManager().registerHandler<ViewOutsideEvent>(&FloorTracker::onView, this);
  }

  game::EventStatus FloorTracker::onHeroLocation(game::EventType type, game::Event *event) {
    assert(type == HeroLocationEvent::type);
    m_floor = static_cast<HeroLocationEvent *>(event)->loc.floor;
    return game::EventStatus::DIE; // we only need it once for initialization
  }

  game::EventStatus FloorTracker::onView(game::EventType type, game::Event *event) {
    switch (type) {
      case ViewUpEvent::type:
        m_floor += 2;
        break;
      case ViewDownEvent::type:
        m_floor -= 2;
        break;
      case ViewInsideEvent::type:
        m_floor += 1;
        break;
      case ViewOutsideEvent::type:
        m_floor -= 1;
        break;
      default:
        assert(false);
        break;
    }

    return game::EventStatus::KEEP;
  }

}
