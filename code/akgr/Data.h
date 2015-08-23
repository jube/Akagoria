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
#ifndef AKGR_DATA_H
#define AKGR_DATA_H

#include <boost/filesystem.hpp>
#include <SFML/Graphics.hpp>

#include "Location.h"

namespace akgr {
  enum class CollisionShape {
    CIRCLE,
    RECTANGLE,
  };

  struct CollisionData {
    CollisionShape shape;
    union {
      struct {
        float radius;
      } circle;
      struct {
        float width;
        float height;
      } rectangle;
    };
  };

  struct SpriteData {
    boost::filesystem::path image;
    sf::IntRect rectangle;
  };

  struct ItemData {
    std::string sprite;
    std::string collision;
  };

  struct PointOfInterestData {
    Location loc;
  };

  struct DialogData {
    struct Line {
      sf::String speaker;
      sf::String words;
    };

    std::vector<Line> content;
  };

  struct MessageData {
    sf::String message;
  };

}

#endif // AKGR_DATA_H
