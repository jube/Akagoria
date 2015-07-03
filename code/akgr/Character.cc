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

#include <game/Log.h>

#include "Data.h"
#include "Singletons.h"

namespace akgr {

  static constexpr float CHARACTER_WIDTH = 60;
  static constexpr float CHARACTER_HEIGHT = 45;

  Character::Character(std::string name, float x, float y, float angle, int floor)
  : m_name(std::move(name))
  {
    CollisionData data;
    data.shape = CollisionShape::RECTANGLE;
    data.rectangle.width = CHARACTER_WIDTH;
    data.rectangle.height = CHARACTER_HEIGHT;
    m_body = gPhysicsModel().createCharacterBody(x, y, floor, &data);
    m_body.setAngleAndVelocity(angle, 0.0f);
  }

  void Character::update(float dt) {

  }

  static constexpr float PI_2 = 1.57079632679489661923f;

  void Character::render(sf::RenderWindow& window) {
    auto pos = m_body.getPosition();
    auto angle = m_body.getAngle() / PI_2 * 90.0f;

    sf::RectangleShape shape({ CHARACTER_WIDTH, CHARACTER_HEIGHT });
    shape.setOrigin(CHARACTER_WIDTH / 2, CHARACTER_HEIGHT / 2);
    shape.setFillColor(sf::Color(0xFF, 0x80, 0x00));
    shape.setPosition(pos);
    shape.setRotation(angle);
    window.draw(shape);
  }


  void CharacterManager::addCharacter(std::string name, float x, float y, float angle, int floor) {
    auto it = m_nameToCharacters.find(name);

    if (it != m_nameToCharacters.end()) {
      game::Log::warning(game::Log::GENERAL, "A character already exists with this name: '%s'\n", name.c_str());
      return;
    }

    auto index = m_characters.size();
    m_nameToCharacters.insert(std::make_pair(name, index));
    m_characters.emplace_back(std::move(name), x, y, angle, floor);
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
    for (auto& c : m_characters) {
      c.update(dt);
    }
  }

  void CharacterManager::render(sf::RenderWindow& window) {
    for (auto& c : m_characters) {
      c.render(window);
    }
  }

}
