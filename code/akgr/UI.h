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

  enum class HorizontalAction {
    LEFT,
    NONE,
    RIGHT,
  };

  enum class VerticalAction {
    UP,
    NONE,
    DOWN,
  };

  class EntityUI : public game::Entity {
  public:

    virtual void onHorizontalAction(HorizontalAction action);
    virtual void onLeft();
    virtual void onRight();


    virtual void onVerticalAction(VerticalAction action);
    virtual void onUp();
    virtual void onDown();
  };

  class DialogUI : public EntityUI {
  public:
    DialogUI();

    void setDialogLine(const DialogData::Line& line);

    virtual void render(sf::RenderWindow& window) override;

  private:
    sf::Font *m_font;
    const DialogData::Line *m_currentLine;
  };

  class MessageUI : public EntityUI {
  public:
    MessageUI();

    void setMessage(const MessageData& message);

    virtual void render(sf::RenderWindow& window) override;

  private:
    sf::Font *m_font;
    const MessageData *m_currentMessage;
  };

  class HeroUI : public EntityUI {
  public:
    virtual void onHorizontalAction(HorizontalAction action) override;
    virtual void onVerticalAction(VerticalAction action) override;
  };

  class SplashUI : public game::Entity {
  public:
    SplashUI();

    virtual void render(sf::RenderWindow& window) override;

    void displaySplashMessage(sf::RenderWindow& window, bool loading = false);

  private:
    sf::String m_titleString;
    sf::String m_loadingString;
    sf::Font *m_font;
  };

  class MenuUI : public EntityUI {
  public:
    MenuUI(int choiceCount)
    : m_currentChoice(0)
    , m_choiceCount(choiceCount)
    {

    }

    int getCurrentChoice() const {
      return m_currentChoice;
    }

    virtual void onUp() override;
    virtual void onDown() override;

  private:
    int m_currentChoice;
    int m_choiceCount;

  };

  class SelectSlotUI : public MenuUI {
  public:
    static constexpr int CHOICE_SLOT0 = 0;
    static constexpr int CHOICE_SLOT1 = 1;
    static constexpr int CHOICE_SLOT2 = 2;
    static constexpr int CHOICE_BACK = 3;

    SelectSlotUI();

    virtual void render(sf::RenderWindow& window) override;

  private:
    sf::String m_backString;
    sf::Font *m_font;
  };

  class StartUI : public MenuUI {
  public:
    static constexpr int CHOICE_NEW = 0;
    static constexpr int CHOICE_LOAD = 1;
    static constexpr int CHOICE_QUIT = 2;

    StartUI();

    virtual void render(sf::RenderWindow& window) override;

  private:
    sf::String m_newString;
    sf::String m_loadString;
    sf::String m_quitString;
    sf::Font *m_font;
  };

}

#endif // AKGR_UI_H
