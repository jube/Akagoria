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
#include "DataManager.h"

#include <yaml-cpp/yaml.h>

#include <game/Log.h>

namespace akgr {

  static void loadCollisionData(std::map<std::string, CollisionData>& collisions, const std::string& path) {
    try {
      YAML::Node node = YAML::LoadFile(path);

      assert(node.IsMap());

      for (const auto& entry : node) {
        std::string name = entry.first.as<std::string>();
        auto properties = entry.second;

        assert(properties.IsMap());

        auto typeNode = properties["type"];

        if (!typeNode) {
          game::Log::warning(game::Log::RESOURCES, "Missing type for entry: '%s'\n", name.c_str());
          continue;
        }

        std::string type = typeNode.as<std::string>();

        CollisionData data;

        if (type == "circle") {
          data.shape = CollisionShape::CIRCLE;

          auto radiusNode = properties["radius"];
          assert(radiusNode);

          data.circle.radius = radiusNode.as<float>();

          collisions.emplace(std::move(name), data);

        } else {
          game::Log::warning(game::Log::RESOURCES, "Unknown type for entry: '%s'\n", name.c_str());
        }
      }
    } catch (std::exception& ex) {
      game::Log::error(game::Log::RESOURCES, "Error when loading collision database: %s\n", ex.what());
      return;
    }
  }

  static void loadSpriteData(std::map<std::string, SpriteData>& sprites, const std::string& path) {
    try {
      YAML::Node node = YAML::LoadFile(path);

      assert(node.IsMap());

      for (const auto& entry : node) {
        std::string name = entry.first.as<std::string>();
        auto properties = entry.second;

        assert(properties.IsMap());

        auto imageNode = properties["image"];

        if (!imageNode) {
          game::Log::warning(game::Log::RESOURCES, "Missing image for entry: '%s'\n", name.c_str());
          continue;
        }

        std::string image = imageNode.as<std::string>();

        auto dimensionNode = properties["dimension"];

        if (!dimensionNode) {
          game::Log::warning(game::Log::RESOURCES, "Missing dimension for entry: '%s'\n", name.c_str());
          continue;
        }

        assert(dimensionNode.IsMap());

        int width = dimensionNode["width"].as<int>();
        assert(width > 0);
        int height = dimensionNode["height"].as<int>();
        assert(height > 0);

        auto spriteNode = properties["sprite"];

        if (!spriteNode) {
          game::Log::warning(game::Log::RESOURCES, "Missing sprite for entry: '%s'\n", name.c_str());
          continue;
        }

        assert(spriteNode.IsMap());

        int spriteWidth = spriteNode["width"].as<int>();
        assert(spriteWidth > 0);
        int spriteHeight = spriteNode["height"].as<int>();
        assert(spriteHeight > 0);

        assert(width % spriteWidth == 0);
        assert(height % spriteHeight == 0);

        int count = width / spriteWidth;
        assert(count > 0);

        auto ids_node = properties["ids"];

        unsigned k = 0;

        for (auto id : ids_node) {
          std::string name = id.as<std::string>();

          int left = (k % count) * spriteWidth;
          int top = (k / count) * spriteHeight;

          SpriteData data;
          data.image = image;
          data.rectangle = { left, top, spriteWidth, spriteHeight };

          sprites.emplace(std::move(name), std::move(data));

        }
      }
    } catch (std::exception& ex) {
      game::Log::error(game::Log::RESOURCES, "Error when loading sprite database: %s\n", ex.what());
      return;
    }
  }

  static void loadItemData(std::map<std::string, ItemData>& items, const std::string& path) {
    try {
      YAML::Node node = YAML::LoadFile(path);

      assert(node.IsMap());

      for (const auto& entry : node) {
        std::string name = entry.first.as<std::string>();
        auto properties = entry.second;

        ItemData data;

        assert(properties.IsMap());

        auto spriteNode = properties["sprite"];

        if (!spriteNode) {
          game::Log::warning(game::Log::RESOURCES, "Missing sprite for entry: '%s'\n", name.c_str());
          continue;
        }

        data.sprite = spriteNode.as<std::string>();

        auto collisionNode = properties["collision"];

        if (!collisionNode) {
          game::Log::warning(game::Log::RESOURCES, "Missing collision for entry: '%s'\n", name.c_str());
          continue;
        }

        data.collision = collisionNode.as<std::string>();

        items.emplace(std::move(name), std::move(data));
      }
    } catch (std::exception& ex) {
      game::Log::error(game::Log::RESOURCES, "Error when loading item database: %s\n", ex.what());
      return;
    }
  }


  void DataManager::load(const boost::filesystem::path& basedir) {
    game::Log::info(game::Log::RESOURCES, "Loading data\n");

    boost::filesystem::path collisions_path = basedir / "data/collisions.yml";
    loadCollisionData(m_collisions, collisions_path.string());
    game::Log::info(game::Log::RESOURCES, "\tCollision data: %zu\n", m_collisions.size());

    boost::filesystem::path sprites_path = basedir / "data/sprites.yml";
    loadSpriteData(m_sprites, sprites_path.string());
    game::Log::info(game::Log::RESOURCES, "\tSprite data: %zu\n", m_sprites.size());

    boost::filesystem::path items_path = basedir / "data/items.yml";
    loadItemData(m_items, items_path.string());
    game::Log::info(game::Log::RESOURCES, "\tItem data: %zu\n", m_items.size());
  }

  const CollisionData *DataManager::getCollisionDataFor(const std::string& name) const {
    auto it = m_collisions.find(name);

    if (it == m_collisions.end()) {
      game::Log::warning(game::Log::RESOURCES, "Could not find collision data for '%s'\n", name.c_str());
      return nullptr;
    }

    return &it->second;
  }

  const SpriteData *DataManager::getSpriteDataFor(const std::string& name) const {
    auto it = m_sprites.find(name);

    if (it == m_sprites.end()) {
      game::Log::warning(game::Log::RESOURCES, "Could not find sprite data for '%s'\n", name.c_str());
      return nullptr;
    }

    return &it->second;
  }

  std::tuple<const CollisionData *, const SpriteData *> DataManager::getItemDataFor(const std::string& name) const {
    auto it = m_items.find(name);

    if (it == m_items.end()) {
      game::Log::warning(game::Log::RESOURCES, "Could not find item data for '%s'\n", name.c_str());
      return std::make_tuple(nullptr, nullptr);
    }

    ItemData data = it->second;
    return std::make_tuple(getCollisionDataFor(data.collision), getSpriteDataFor(data.sprite));
  }

}
