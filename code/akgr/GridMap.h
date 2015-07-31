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
#ifndef AKGR_GRID_MAP_H
#define AKGR_GRID_MAP_H

#include <cassert>
#include <vector>

#include <boost/range/irange.hpp>

#include <game/Entity.h>
#include <game/Event.h>

#include "Location.h"

namespace akgr {

  class BaseMap : public game::Entity {
  public:
    BaseMap(int priority);

  protected:
    void initialize(unsigned grid_width, unsigned grid_height, unsigned grid_unit);

    unsigned getGridWidth() const {
      return m_grid_width;
    }

    unsigned getGridHeight() const {
      return m_grid_height;
    }

    unsigned getGridUnit() const {
      return m_grid_unit;
    }

    typedef boost::iterator_range<boost::range_detail::integer_iterator<unsigned>> Range;

    Range getXRange() const;

    Range getYRange() const;

    int getFloor() const {
      return m_floor;
    }

    void setDirty() {
      m_dirty = true;
    }

    void setClean() {
      m_dirty = false;
    }

    bool isDirty() const {
      return m_dirty;
    }

    static unsigned computeGridSize(unsigned map_size, unsigned grid_unit);

  private:
    game::EventStatus onHeroLocation(game::EventType type, game::Event *event);

  private:
    unsigned m_grid_width;
    unsigned m_grid_height;
    unsigned m_grid_unit;
    unsigned m_focus_x;
    unsigned m_focus_y;
    int m_floor;
    bool m_dirty;
  };

  template<typename T>
  class GridMap : public BaseMap {
  public:

    GridMap(int priority)
    : BaseMap(priority) {
    }

    void addObject(const T& obj, const sf::Vector2f& pos) {
      assert(pos.x >= 0.0f);
      assert(pos.y >= 0.0f);

      unsigned x = pos.x / getGridUnit();
      unsigned y = pos.y / getGridUnit();

      std::size_t index = y * getGridWidth() + x;
      m_content[index].push_back(obj);
    }

  protected:
    void initialize(unsigned map_width, unsigned map_height, unsigned grid_unit) {
      unsigned grid_width = BaseMap::computeGridSize(map_width, grid_unit);
      unsigned grid_height = BaseMap::computeGridSize(map_height, grid_unit);

      BaseMap::initialize(grid_width, grid_height, grid_unit);

      assert(m_content.empty());
      m_content.resize(grid_width * grid_height);
    }

    template<typename Func>
    void processObjects(Func func) {
      for (auto y : getYRange()) {
        unsigned start = y * getGridWidth();

        for (auto x : getXRange()) {
          auto& vec = m_content.at(start + x);

          for (auto& obj : vec) {
            func(obj);
          }
        }
      }
    }

  private:
    std::vector<std::vector<T>> m_content;
  };


}

#endif // AKGR_GRID_MAP_H
