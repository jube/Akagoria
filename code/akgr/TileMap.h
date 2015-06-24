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
#ifndef AKGR_TILE_MAP_H
#define AKGR_TILE_MAP_H

#include <tmx/Map.h>

#include "GridMap.h"

namespace akgr {

  struct Tile {
    int floor;
    std::array<sf::Vector2f, 4> position;
    std::array<sf::Vector2f, 4> texCoords;
  };

  extern template class GridMap<Tile>;

  class TileMap : public GridMap<Tile> {
  public:
    TileMap(int priority);

    void loadMap(tmx::Map& map, const std::string& kind);

    void setTexture(sf::Texture *texture);
    void addTile(const Tile& tile);

    virtual void update(float dt) override;
    virtual void render(sf::RenderWindow& window) override;

  private:
    sf::Texture *m_texture;
    sf::VertexArray m_vertices;
  };


}

#endif // AKGR_TILE_MAP_H
