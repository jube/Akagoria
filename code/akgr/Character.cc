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
#include "Character.h"

#include <cmath>

#include <game/Log.h>

#include "Data.h"
#include "GameEvents.h"
#include "Maths.h"
#include "Singletons.h"

namespace akgr {

  static constexpr float DIALOG_RADIUS = 30.0f;
  static constexpr float DIALOG_THICKNESS = 5.0f;

  static constexpr float CHARACTER_WIDTH = 60;
  static constexpr float CHARACTER_HEIGHT = 45;

  Character::Character(std::string name, const Location& loc, float angle)
  : m_name(std::move(name))
  , m_dialogKind(Dialog::NONE)
  {
    CollisionData data;
    data.shape = CollisionShape::RECTANGLE;
    data.rectangle.width = CHARACTER_WIDTH;
    data.rectangle.height = CHARACTER_HEIGHT;
    m_body = gPhysicsModel().createCharacterBody(loc, &data);
    m_body.setAngleAndVelocity(angle, 0.0f);
  }

  void Character::attachDialog(std::string dialogName) {
    m_dialogKind = Dialog::SIMPLE;
    m_dialogName = std::move(dialogName);
  }

  void Character::attachQuestDialog(std::string dialogName) {
    m_dialogKind = Dialog::QUEST;
    m_dialogName = std::move(dialogName);
  }

  void Character::detachDialog() {
    m_dialogKind = Dialog::NONE;
  }

  void Character::update(float dt) {

  }

  void Character::render(sf::RenderWindow& window) {
    auto pos = m_body.getPosition();
    auto angle = m_body.getAngle() / PI_2 * 90.0f;

    if (hasDialog()) {

      sf::Color color;

      switch (m_dialogKind) {
        case Dialog::SIMPLE:
          color = sf::Color(0xFF, 0x00, 0x00, 0x80);
          break;
        case Dialog::QUEST:
          color = sf::Color(0xFF, 0xFF, 0x00, 0x80);
          break;
        default:
          assert(false);
          break;
      }

      sf::CircleShape circleShape(DIALOG_RADIUS);
      circleShape.setFillColor(sf::Color::Transparent);
      circleShape.setOutlineThickness(DIALOG_THICKNESS);
      circleShape.setOutlineColor(color);
      circleShape.setOrigin(DIALOG_RADIUS, DIALOG_RADIUS);
      circleShape.setPosition(pos);
      window.draw(circleShape);
    }

    sf::RectangleShape shape({ CHARACTER_WIDTH, CHARACTER_HEIGHT });
    shape.setOrigin(CHARACTER_WIDTH / 2, CHARACTER_HEIGHT / 2);
    shape.setFillColor(sf::Color(0xFF, 0x80, 0x00));
    shape.setPosition(pos);
    shape.setRotation(angle);
    window.draw(shape);
  }


  CharacterManager::CharacterManager() {
    gEventManager().registerHandler<UseEvent>(&CharacterManager::onUse, this);
  }

  Character *CharacterManager::addCharacter(std::string name, const Location& loc, float angle) {
    auto it = m_nameToCharacters.find(name);

    if (it != m_nameToCharacters.end()) {
      game::Log::warning(game::Log::GENERAL, "A character already exists with this name: '%s'\n", name.c_str());
      return &m_characters[it->second];
    }

    auto index = m_characters.size();
    m_nameToCharacters.insert(std::make_pair(name, index));
    m_characters.emplace_back(std::move(name), loc, angle);
    return &m_characters[index];
  }

  void CharacterManager::updateCharacterSearch() {
    std::size_t index = 0;

    for (const auto& c : m_characters) {
      m_nameToCharacters.insert(std::make_pair(c.getName(), index));
    }
  }

  Character *CharacterManager::getCharacter(const std::string& name) {
    auto it = m_nameToCharacters.find(name);

    if (it == m_nameToCharacters.end()) {
      game::Log::warning(game::Log::GENERAL, "Could not find the character named: '%s'\n", name.c_str());
      return nullptr;
    }

    return &m_characters[it->second];
  }

  void CharacterManager::update(float dt) {
    int floor = m_tracker.getFloor();

    for (auto& c : m_characters) {
      if (c.getLocation().floor != floor) {
        continue;
      }

      c.update(dt);
    }
  }

  void CharacterManager::render(sf::RenderWindow& window) {
    int floor = m_tracker.getFloor();

    for (auto& c : m_characters) {
      if (c.getLocation().floor != floor) {
        continue;
      }

      c.render(window);
    }
  }

  static constexpr float DIALOG_DISTANCE = 100.0f;

  game::EventStatus CharacterManager::onUse(game::EventType type, game::Event *event) {
    auto useEvent = static_cast<UseEvent *>(event);

    for (auto& c : m_characters) {
      if (!c.hasDialog()) {
        continue;
      }

      Location loc = c.getLocation();

      if (loc.floor == useEvent->loc.floor) {
        float d2 = squareDistance(loc.pos, useEvent->loc.pos);
//         game::Log::info(game::Log::GENERAL, "Distance: %f\n", std::sqrt(d2));

        if (d2 < DIALOG_DISTANCE * DIALOG_DISTANCE) {
          useEvent->kind = UseEvent::TALK;
          gDialogManager().start(c.getDialogName());
        }
      }
    }

    return game::EventStatus::KEEP;
  }

}
