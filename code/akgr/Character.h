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
#ifndef AKGR_CHARACTER_H
#define AKGR_CHARACTER_H

#include <game/Entity.h>
#include <game/Event.h>

#include "Body.h"

namespace akgr {

  class Character {
  public:
    Character(std::string name, float x, float y, float angle, int floor);

    void attachDialog(std::string dialogName);
    void detachDialog();

    bool hasDialog() const {
      return !m_dialog.empty();
    }

    const std::string& getDialogName() const {
      return m_dialog;
    }

    Location getLocation() {
      return m_body.getLocation();
    }

    void update(float dt);
    void render(sf::RenderWindow& window);

  private:
    std::string m_name;
    Body m_body;
    std::string m_dialog;
  };


  class CharacterManager : public game::Entity {
  public:
    CharacterManager();

    Character *addCharacter(std::string name, float x, float y, float angle, int floor);

    Character *getCharacter(const std::string& name);

    virtual void update(float dt) override;
    virtual void render(sf::RenderWindow& window) override;

  private:
    std::vector<Character> m_characters;
    std::map<std::string, std::size_t> m_nameToCharacters;

  private:
    game::EventStatus onTalk(game::EventType type, game::Event *event);
  };

}

#endif // AKGR_CHARACTER_H

