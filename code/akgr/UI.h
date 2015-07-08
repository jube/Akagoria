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
#ifndef AKGR_UI_H
#define AKGR_UI_H

#include <game/Entity.h>

#include "Data.h"

namespace akgr {

  class DialogUI : public game::Entity {
  public:
    DialogUI();

    void setDialogLine(const DialogData::Line& line);

    virtual void render(sf::RenderWindow& window) override;

  private:
    sf::Font *m_font;
    const DialogData::Line *m_currentLine;
  };

  class SplashUI : public game::Entity {
  public:
    SplashUI();

    virtual void render(sf::RenderWindow& window) override;

    void displaySplashMessage(sf::RenderWindow& window, bool loading = false);

  private:
    sf::Font *m_font;
  };

  class MenuUI : public game::Entity {
  public:
    MenuUI(int choiceCount)
    : m_currentChoice(0)
    , m_choiceCount(choiceCount)
    {

    }

    int getCurrentChoice() const {
      return m_currentChoice;
    }

    void moveDown();
    void moveUp();

  private:
    int m_currentChoice;
    int m_choiceCount;

  };

  class LoadUI : public MenuUI {
  public:
    LoadUI();

    virtual void render(sf::RenderWindow& window) override;

  private:
    sf::Font *m_font;
  };

  class StartUI : public MenuUI {
  public:
    static constexpr int START_NEW_GAME = 0;
    static constexpr int LOAD_GAME = 1;
    static constexpr int QUIT = 2;

    StartUI();

    virtual void render(sf::RenderWindow& window) override;

  private:
    sf::Font *m_font;
  };

}

#endif // AKGR_UI_H
