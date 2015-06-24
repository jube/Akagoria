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
#include "SpriteMap.h"

#include <tmx/LayerVisitor.h>
#include <tmx/ObjectLayer.h>
#include <tmx/TileLayer.h>

#include <game/Log.h>

#include "Singletons.h"

namespace akgr {

  template class GridMap<Sprite>;

  static constexpr unsigned SPRITE_MAP_UNIT = 1600; /* 25 * 64 */

  namespace {

    struct SpriteConstructor : public tmx::LayerVisitor {

      SpriteConstructor(SpriteMap *spriteMap, const std::string& kind)
      : m_spriteMap(spriteMap)
      , m_kind(kind)
      {

      }

      virtual void visitObjectLayer(const tmx::Map& map, const tmx::ObjectLayer& layer) override {
        if (!layer.hasProperty("kind")) {
          game::Log::warning(game::Log::GRAPHICS, "No kind for the layer: '%s'\n", layer.getName().c_str());
          return;
        }

        if (layer.getProperty("kind", "") != m_kind) {
          return;
        }

        int floor = std::stoi(layer.getProperty("floor", "0"));

        game::Log::info(game::Log::GRAPHICS, "\tLoading sprite layer: '%s' (floor: %i)\n", layer.getName().c_str(), floor);

        unsigned count = 0;

        for (auto obj : layer) {
          const std::string& name = obj->getName();

          if (!obj->isTile()) {
            game::Log::warning(game::Log::GRAPHICS, "Object is not a tile: '%s'\n", name.c_str());
            continue;
          }

          auto tile = static_cast<const tmx::TileObject *>(obj);

          unsigned gid = tile->getGID();
          assert(gid != 0);

          auto tileset = map.getTileSetFromGID(gid);
          auto image = tileset->getImage();
          sf::Texture *texture = gResourceManager().getTexture(image->getSource().string());
          assert(texture);
          texture->setSmooth(true);

          tmx::Size size;

          if (image->hasSize()) {
            size = image->getSize();
          } else {
            sf::Vector2u textureSize = texture->getSize();
            size.width = textureSize.x;
            size.height = textureSize.y;
          }

          gid = gid - tileset->getFirstGID();
          tmx::Rect rect = tileset->getCoords(gid, size);

          Sprite sprite;

          sprite.floor = floor;

          sprite.pos.x = tile->getX();
          sprite.pos.y = tile->getY() - tileset->getTileHeight();

          sprite.rect.left = rect.x;
          sprite.rect.top = rect.y;
          sprite.rect.width = rect.width;
          sprite.rect.height = rect.height;

          sprite.texture = texture;

          m_spriteMap->addSprite(sprite);
          count++;

          auto collisionData = gDataManager().getCollisionDataFor(name);

          if (!collisionData) {
            continue;
          }

          float x = sprite.pos.x + rect.width / 2;
          float y = sprite.pos.y + rect.height / 2;
          gPhysicsModel().addMapItem(x, y, floor, collisionData);
        }

        game::Log::info(game::Log::GRAPHICS, "\t\tSprites loaded: %u\n", count);
      }

    private:
      SpriteMap *m_spriteMap;
      const std::string& m_kind;
    };

  }

  SpriteMap::SpriteMap(int priority)
  : GridMap<Sprite>(priority) {

  }

  void SpriteMap::loadMap(const tmx::Map& map, const std::string& kind) {
    initialize(map.getWidth() * map.getTileWidth(), map.getHeight() * map.getTileHeight(), SPRITE_MAP_UNIT);

    SpriteConstructor visitor(this, kind);
    map.visitLayers(visitor);
  }

  void SpriteMap::addSprite(const Sprite& sprite) {
    addObject(sprite, sprite.pos);
  }

  void SpriteMap::update(float dt)  {
    if (!isDirty()) {
      return;
    }

    m_sprites.clear();

    processObjects([this](const Sprite& sprite) {
      if (sprite.floor == getFloor()) {
        m_sprites.push_back(&sprite);
      }
    });

    std::sort(m_sprites.begin(), m_sprites.end(), [](const Sprite *lhs, const Sprite *rhs) {
      return lhs->texture < rhs->texture;
    });

    setClean();
  }

  void SpriteMap::render(sf::RenderWindow& window)  {
    for (auto spriteData : m_sprites) {
      sf::Sprite sprite(*spriteData->texture, spriteData->rect);
      sprite.setPosition(spriteData->pos);
      window.draw(sprite);
    }
  }

}
