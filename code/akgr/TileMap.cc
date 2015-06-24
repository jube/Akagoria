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
#include "TileMap.h"

#include <tmx/LayerVisitor.h>
#include <tmx/ObjectLayer.h>
#include <tmx/TileLayer.h>

#include <game/Log.h>

#include "Singletons.h"

namespace akgr {

  template class GridMap<Tile>;

static constexpr unsigned TILE_MAP_UNIT = 1600; /* 25 * 64 */

  namespace {

    struct TileConstructor : public tmx::LayerVisitor {
      TileConstructor(TileMap& tileMap, const std::string& kind)
      : m_tileMap(tileMap)
      , m_kind(kind)
      {

      }

      virtual void visitTileLayer(const tmx::Map& map, const tmx::TileLayer& layer) override {
        if (!layer.hasProperty("kind")) {
          game::Log::warning(game::Log::GRAPHICS, "No kind for the layer: '%s'\n", layer.getName().c_str());
          return;
        }

        if (layer.getProperty("kind", "") != m_kind) {
          return;
        }

        int floor = std::stoi(layer.getProperty("floor", "0"));

        game::Log::info(game::Log::GRAPHICS, "\tLoading tile layer: '%s' (floor: %i)\n", layer.getName().c_str(), floor);

        unsigned tileWidth = map.getTileWidth();
        assert(tileWidth);

        unsigned tileHeight = map.getTileHeight();
        assert(tileHeight);

        unsigned width = map.getWidth();
        assert(width);

        unsigned height = map.getHeight();
        assert(height);

        sf::Texture *texture = nullptr;
        tmx::Size size;

        unsigned k = 0;
        unsigned count = 0;

        for (auto cell : layer) {
          unsigned i = k % width;
          unsigned j = k / width;
          assert(j < height);
          k++;

          unsigned gid = cell.getGID();

          if (gid == 0) {
            continue;
          }

          auto tileset = map.getTileSetFromGID(gid);

          if (texture == nullptr) {
            assert(tileset->getTileWidth() == tileWidth);
            assert(tileset->getTileHeight() == tileHeight);

            auto image = tileset->getImage();
            texture = gResourceManager().getTexture(image->getSource().string());
            assert(texture);
            texture->setSmooth(true);

            if (image->hasSize()) {
              size = image->getSize();
            } else {
              sf::Vector2u texture_size = texture->getSize();
              size.width = texture_size.x;
              size.height = texture_size.y;
            }

            m_tileMap.setTexture(texture);
          } else {
            assert(texture == gResourceManager().getTexture(tileset->getImage()->getSource().string()));
          }

          gid = gid - tileset->getFirstGID();
          tmx::Rect rect = tileset->getCoords(gid, size);

          Tile tile;

          tile.floor = floor;

          // define its 4 corners
          tile.position[0] = sf::Vector2f(i * tileWidth, j * tileHeight);
          tile.position[1] = sf::Vector2f((i + 1) * tileWidth, j * tileHeight);
          tile.position[2] = sf::Vector2f((i + 1) * tileWidth, (j + 1) * tileHeight);
          tile.position[3] = sf::Vector2f(i * tileWidth, (j + 1) * tileHeight);

          // define its 4 texture coordinates
          tile.texCoords[0] = sf::Vector2f(rect.x, rect.y);
          tile.texCoords[1] = sf::Vector2f(rect.x + rect.width, rect.y);
          tile.texCoords[2] = sf::Vector2f(rect.x + rect.width, rect.y + rect.height);
          tile.texCoords[3] = sf::Vector2f(rect.x, rect.y + rect.height);

          m_tileMap.addTile(tile);
          count++;
        }

        game::Log::info(game::Log::GRAPHICS, "\t\tTiles loaded: %u\n", count);
      }


    private:
      TileMap& m_tileMap;
      const std::string& m_kind;
    };

  }

  TileMap::TileMap(int priority)
  : GridMap<Tile>(priority), m_texture(nullptr) {

  }

  void TileMap::setTexture(sf::Texture *texture) {
    assert(m_texture == nullptr || texture == m_texture);
    m_texture = texture;
  }


  void TileMap::loadMap(tmx::Map& map, const std::string& kind) {
    initialize(map.getWidth() * map.getTileWidth(), map.getHeight() * map.getTileHeight(), TILE_MAP_UNIT);

    TileConstructor visitor(*this, kind);
    map.visitLayers(visitor);
  }

  void TileMap::addTile(const Tile& tile) {
    addObject(tile, (tile.position[0] + tile.position[2]) / 2.0f);
  }

  void TileMap::update(float dt)  {
    if (!isDirty()) {
      return;
    }

    m_vertices.clear();
    m_vertices.setPrimitiveType(sf::Quads);

    processObjects([this](const Tile& tile) {
      if (tile.floor == getFloor()) {
        for (std::size_t i = 0; i < 4; ++i) {
          m_vertices.append(sf::Vertex(tile.position[i], tile.texCoords[i]));
        }
      }
    });

    setClean();
  }

  void TileMap::render(sf::RenderWindow& window)  {
    if (!m_texture) {
      return;
    }

    window.draw(m_vertices, m_texture);
  }

}
