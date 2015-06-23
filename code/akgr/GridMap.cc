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
#include "GridMap.h"

#include "GameEvents.h"
#include "MapEvents.h"
#include "Singletons.h"

namespace akgr {

  BaseMap::BaseMap(int priority)
    : game::Entity(priority)
    , m_grid_width(0), m_grid_height(0), m_grid_unit(0), m_focus_x(0), m_focus_y(0), m_floor(0), m_dirty(true) {
    gEventManager().registerHandler<HeroLocationEvent>(&BaseMap::onHeroLocation, this);
    gEventManager().registerHandler<ViewDownEvent>(&BaseMap::onViewDown, this);
    gEventManager().registerHandler<ViewUpEvent>(&BaseMap::onViewUp, this);
  }

  void BaseMap::initialize(unsigned grid_width, unsigned grid_height, unsigned grid_unit) {
    m_grid_width = grid_width;
    m_grid_height = grid_height;
    m_grid_unit = grid_unit;
  }

  BaseMap::Range BaseMap::getXRange() const {
    unsigned xmin = (m_focus_x > 0) ? m_focus_x - 1 : m_focus_x;
    unsigned xmax = (m_focus_x < m_grid_width - 1) ? m_focus_x + 1 : m_focus_x;
    return boost::irange(xmin, xmax);
  }

  BaseMap::Range BaseMap::getYRange() const {
    unsigned ymin = (m_focus_y > 0) ? m_focus_y - 1 : m_focus_y;
    unsigned ymax = (m_focus_y < m_grid_height - 1) ? m_focus_y + 1 : m_focus_y;
    return boost::irange(ymin, ymax);
  }

  game::EventStatus BaseMap::onHeroLocation(game::EventType type, game::Event *event) {
    assert(type == HeroLocationEvent::type);

    sf::Vector2f pos = static_cast<HeroLocationEvent *>(event)->loc.pos;

    assert(pos.x >= 0.0f);
    assert(pos.y >= 0.0f);

    unsigned x = pos.x / m_grid_unit;
    unsigned y = pos.y / m_grid_unit;

    if (x != m_focus_x || y != m_focus_y) {
      m_focus_x = x;
      m_focus_y = y;
      setDirty();
    }

    return game::EventStatus::KEEP;
  }

  game::EventStatus BaseMap::onViewDown(game::EventType type, game::Event *event) {
    assert(type == ViewDownEvent::type);
    m_floor -= 2;
    setDirty();
    return game::EventStatus::KEEP;
  }

  game::EventStatus BaseMap::onViewUp(game::EventType type, game::Event *event) {
    assert(type == ViewDownEvent::type);
    m_floor += 2;
    setDirty();
    return game::EventStatus::KEEP;
  }


  unsigned BaseMap::computeGridSize(unsigned map_size, unsigned grid_unit) {
    if (map_size % grid_unit == 0) {
      return map_size / grid_unit;
    }

    return map_size / grid_unit + 1;
  }


//   template class GridMap<Tile>;

}
