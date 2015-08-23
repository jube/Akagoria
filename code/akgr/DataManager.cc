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

#include <boost/locale.hpp>

#include <tmx/ObjectLayer.h>
#include <yaml-cpp/yaml.h>

#include <game/Log.h>

namespace akgr {

  static sf::String convertString(const std::string& str) {
    std::basic_string<sf::Uint32> u32String;
    u32String.reserve(str.size());
    sf::Utf8::toUtf32(str.begin(), str.end(), std::back_inserter(u32String));
    return sf::String(u32String);
  }

  static sf::String convertLocalString(const std::string& str) {
    std::string translatedString = boost::locale::gettext(str.c_str());
    return convertString(translatedString);
  }

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

  static void loadDialogData(std::map<std::string, DialogData>& dialogues, const std::string& path) {
    try {
      YAML::Node node = YAML::LoadFile(path);

      assert(node.IsMap());

      for (const auto& entry : node) {
        std::string name = entry.first.as<std::string>();
        auto properties = entry.second;

        DialogData data;

        assert(properties.IsMap());

        auto contentNode = properties["content"];

        if (!contentNode) {
          game::Log::warning(game::Log::RESOURCES, "Missing content for entry: '%s'\n", name.c_str());
          continue;
        }

        assert(contentNode.IsSequence());

        for (const auto& lineNode : contentNode) {
          auto speakerNode = lineNode["speaker"];
          assert(speakerNode);
          auto speaker = convertString(speakerNode.as<std::string>());

          auto wordsNode = lineNode["words"];
          assert(wordsNode);
          auto words = convertLocalString(wordsNode.as<std::string>());

          data.content.push_back({ std::move(speaker), std::move(words) });
        }

        dialogues.emplace(std::move(name), std::move(data));
      }
    } catch (std::exception& ex) {
      game::Log::error(game::Log::RESOURCES, "Error when loading item database: %s\n", ex.what());
      return;
    }
  }

  static void loadMessageData(std::map<std::string, MessageData>& messages, const std::string& path) {
    try {
      YAML::Node node = YAML::LoadFile(path);

      assert(node.IsMap());

      for (const auto& entry : node) {
        std::string name = entry.first.as<std::string>();
        auto properties = entry.second;

        MessageData data;

        assert(properties.IsMap());

        auto messageNode = properties["message"];

        if (!messageNode) {
          game::Log::warning(game::Log::RESOURCES, "Missing message for entry: '%s'\n", name.c_str());
          continue;
        }

        data.message = convertLocalString(messageNode.as<std::string>());

        messages.emplace(std::move(name), std::move(data));
      }
    } catch (std::exception& ex) {
      game::Log::error(game::Log::RESOURCES, "Error when loading item database: %s\n", ex.what());
      return;
    }
  }

  void DataManager::load(const boost::filesystem::path& basedir) {
    game::Log::info(game::Log::RESOURCES, "Loading data\n");

    boost::filesystem::path collisionsPath = basedir / "data/collisions.yml";
    loadCollisionData(m_collisions, collisionsPath.string());
    game::Log::info(game::Log::RESOURCES, "\tCollision data: %zu\n", m_collisions.size());

    boost::filesystem::path spritesPath = basedir / "data/sprites.yml";
    loadSpriteData(m_sprites, spritesPath.string());
    game::Log::info(game::Log::RESOURCES, "\tSprite data: %zu\n", m_sprites.size());

    boost::filesystem::path itemsPath = basedir / "data/items.yml";
    loadItemData(m_items, itemsPath.string());
    game::Log::info(game::Log::RESOURCES, "\tItem data: %zu\n", m_items.size());

    boost::filesystem::path dialoguesPath = basedir / "data/dialogues.yml";
    loadDialogData(m_dialogues, dialoguesPath.string());
    game::Log::info(game::Log::RESOURCES, "\tDialog data: %zu\n", m_dialogues.size());

    boost::filesystem::path messagesPath = basedir / "data/messages.yml";
    loadMessageData(m_messages, messagesPath.string());
    game::Log::info(game::Log::RESOURCES, "\tMessage data: %zu\n", m_messages.size());
  }

  namespace {

    struct DataConstructor : public tmx::LayerVisitor {
      DataConstructor(DataManager& manager)
      : m_manager(manager)
      {

      }

      virtual void visitObjectLayer(const tmx::Map& map, const tmx::ObjectLayer& layer) override {
        if (!layer.hasProperty("kind")) {
          game::Log::warning(game::Log::GRAPHICS, "No kind for the layer: '%s'\n", layer.getName().c_str());
          return;
        }

        if (layer.getProperty("kind", "") != "poi") {
          return;
        }

        int floor = std::stoi(layer.getProperty("floor", "0"));

        game::Log::info(game::Log::RESOURCES, "Loading POI layer: '%s' (floor: %i)\n", layer.getName().c_str(), floor);

        unsigned count = 0;

        for (auto obj : layer) {
          const std::string& name = obj->getName();

          if (!obj->isEllipse()) {
            game::Log::warning(game::Log::RESOURCES, "Object is not an ellipse: '%s'\n", name.c_str());
            continue;
          }

          sf::Vector2f pos(obj->getX(), obj->getY());
          m_manager.addPointOfInterestData(name, { pos, floor });

          count++;
        }

        game::Log::info(game::Log::RESOURCES, "\tPOI loaded: %u\n", count);
      }

    private:
      DataManager& m_manager;
    };


  }

  void DataManager::loadMap(const tmx::Map& map) {
    DataConstructor visitor(*this);
    map.visitLayers(visitor);
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

  void DataManager::addPointOfInterestData(std::string name, const Location& loc) {
    m_pois.insert(std::make_pair(std::move(name), PointOfInterestData{ loc }));
  }

  const PointOfInterestData *DataManager::getPointOfInterestDataFor(const std::string& name) const {
    auto it = m_pois.find(name);

    if (it == m_pois.end()) {
      game::Log::warning(game::Log::RESOURCES, "Could not find point of interest data for '%s'\n", name.c_str());
      return nullptr;
    }

    return &it->second;
  }

  const DialogData *DataManager::getDialogDataFor(const std::string& name) const {
    auto it = m_dialogues.find(name);

    if (it == m_dialogues.end()) {
      game::Log::warning(game::Log::RESOURCES, "Could not find dialogue data for '%s'\n", name.c_str());
      return nullptr;
    }

    return &it->second;
  }

  const MessageData *DataManager::getMessageDataFor(const std::string& name) const {
    auto it = m_messages.find(name);

    if (it == m_messages.end()) {
      game::Log::warning(game::Log::RESOURCES, "Could not find message data for '%s'\n", name.c_str());
      return nullptr;
    }

    return &it->second;
  }

}
