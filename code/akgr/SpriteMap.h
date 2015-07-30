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
#ifndef AKGR_SPRITE_MAP_H
#define AKGR_SPRITE_MAP_H

#include <tmx/Map.h>

#include "GridMap.h"

namespace akgr {

  struct Sprite {
    int floor;
    float angle;
    sf::Vector2f pos;
    sf::IntRect rect;
    sf::Texture *texture;
  };

  extern template class GridMap<Sprite>;

  class SpriteMap : public GridMap<Sprite> {
  public:
    SpriteMap(int priority);

    void loadMap(const tmx::Map& map, const std::string& kind);

    void addSprite(const Sprite& sprite);

    virtual void update(float dt) override;
    virtual void render(sf::RenderWindow& window) override;

  private:
    std::vector<const Sprite*> m_sprites;
  };

}

#endif // AKGR_SPRITE_MAP_H
