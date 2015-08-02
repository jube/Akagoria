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
#include "UI.h"

#include "Singletons.h"

namespace akgr {

  static constexpr unsigned STANDARD_SIZE = 20;
  static constexpr float STANDARD_POINTER_RADIUS = 5.0f;

  void EntityUI::onHorizontalAction(HorizontalAction action) {
    switch (action) {
      case HorizontalAction::LEFT:
        onLeft();
        break;
      case HorizontalAction::RIGHT:
        onRight();
        break;
      case HorizontalAction::NONE:
        // nothing
        break;
    }
  }

  void EntityUI::onLeft() {
    // nothing by default
  }

  void EntityUI::onRight() {
    // nothing by default
  }

  void EntityUI::onVerticalAction(VerticalAction action) {
    switch (action) {
      case VerticalAction::UP:
        onUp();
        break;
      case VerticalAction::DOWN:
        onDown();
        break;
      case VerticalAction::NONE:
        // nothing;
        break;
    }
  }

  void EntityUI::onUp() {
    // nothing by default
  }

  void EntityUI::onDown() {
    // nothing by default
  }


  static void drawBox(sf::RenderWindow& window, float x, float y, float width, float height) {
    static const sf::Color fillColor(0x04, 0x08, 0x84, 0xC0);

    sf::RectangleShape boxShape({ width, height });
    boxShape.setPosition(x, y);
    boxShape.setFillColor(fillColor);
    boxShape.setOutlineColor(sf::Color::White);
    boxShape.setOutlineThickness(1);
    window.draw(boxShape);
  }

  static void drawText(sf::RenderWindow& window, sf::Font& font, float x, float y, unsigned size, const std::string& str) {
    sf::Text text(str, font, size);
    auto rect = text.getLocalBounds();
    text.setColor(sf::Color::White);
    text.setOrigin(rect.left, rect.top);
    text.setPosition(x, y);
    window.draw(text);
  }

  static void drawPointer(sf::RenderWindow& window, float x, float y) {
    sf::CircleShape pointer(STANDARD_POINTER_RADIUS, 3);
    pointer.setOrigin(STANDARD_POINTER_RADIUS, STANDARD_POINTER_RADIUS);
    pointer.setPosition(x, y);
    pointer.rotate(90);
    pointer.setFillColor(sf::Color::White);
    window.draw(pointer);
  }

  static constexpr unsigned SPEAKER_SIZE = 16;
  static constexpr float SPEAKER_WIDTH = 150.0f;
  static constexpr float SPEAKER_HEIGHT = 25.0f;
  static constexpr float SPEAKER_PADDING = 5.0f;

  static constexpr unsigned WORDS_SIZE = STANDARD_SIZE;
  static constexpr float WORDS_WIDTH = 600.0f;
  static constexpr float WORDS_HEIGHT = 90.0f;
  static constexpr float WORDS_BOTTOM = 40.0f;
  static constexpr float WORDS_PADDING = 10.0f;

  DialogUI::DialogUI()
  : m_currentLine(nullptr)
  {
    m_font = gResourceManager().getFont("fonts/DejaVuSans.ttf");
    assert(m_font);
  }

  void DialogUI::setDialogLine(const DialogData::Line& line) {
    m_currentLine = &line;
  }

  void DialogUI::render(sf::RenderWindow& window) {
    assert(m_currentLine);

    float x = gWindowGeometry().getXCentered(WORDS_WIDTH);
    float y = gWindowGeometry().getYFromBottom(WORDS_HEIGHT + WORDS_BOTTOM);

    // draw speaker box and text
    drawBox(window, x + WORDS_PADDING, y - SPEAKER_HEIGHT, SPEAKER_WIDTH, SPEAKER_HEIGHT);
    drawText(window, *m_font, x + WORDS_PADDING + SPEAKER_PADDING, y - SPEAKER_HEIGHT + SPEAKER_PADDING, SPEAKER_SIZE, m_currentLine->speaker);

    // draw words box and text
    drawBox(window, x, y, WORDS_WIDTH, WORDS_HEIGHT);
    drawText(window, *m_font, x + WORDS_PADDING, y + WORDS_PADDING, WORDS_SIZE, m_currentLine->words);
  }


  static constexpr unsigned MESSAGE_SIZE = STANDARD_SIZE;

  static constexpr float MESSAGE_WIDTH = 600.0f;
  static constexpr float MESSAGE_HEIGHT = 90.0f;
  static constexpr float MESSAGE_PADDING = 10.0f;
  static constexpr float MESSAGE_TOP = 40.0f;

  MessageUI::MessageUI()
  : m_currentMessage(nullptr)
  {
    m_font = gResourceManager().getFont("fonts/DejaVuSans.ttf");
    assert(m_font);
  }

  void MessageUI::setMessage(const MessageData& message) {
    m_currentMessage = &message;
  }

  void MessageUI::render(sf::RenderWindow& window) {
    assert(m_currentMessage);

    float x = gWindowGeometry().getXCentered(MESSAGE_WIDTH);
    drawBox(window, x, MESSAGE_TOP, MESSAGE_WIDTH, MESSAGE_HEIGHT);
    drawText(window, *m_font, x + MESSAGE_PADDING, MESSAGE_TOP + MESSAGE_PADDING, MESSAGE_SIZE, m_currentMessage->message);
  }


  void HeroUI::onHorizontalAction(HorizontalAction action) {
    switch (action) {
      case HorizontalAction::LEFT:
        akgr::gHero().turnLeft();
        break;
      case HorizontalAction::RIGHT:
        akgr::gHero().turnRight();
        break;
      case HorizontalAction::NONE:
        akgr::gHero().stopTurning();
        break;
    }
  }

  void HeroUI::onVerticalAction(VerticalAction action) {
    switch (action) {
      case VerticalAction::UP:
        akgr::gHero().walkForward();
        break;
      case VerticalAction::DOWN:
        akgr::gHero().walkBackward();
        break;
      case VerticalAction::NONE:
        akgr::gHero().stopWalking();
        break;
    }
  }


  SplashUI::SplashUI() {
    m_font = gResourceManager().getFont("fonts/DejaVuSans.ttf");
    assert(m_font);
  }

  void SplashUI::render(sf::RenderWindow& window) {
    displaySplashMessage(window, false);
  }

  static constexpr float LOADING_PADDING = 60.0f;

  void SplashUI::displaySplashMessage(sf::RenderWindow& window, bool loading) {
    // define a splash message
    sf::Text text;
    text.setFont(*m_font);
    text.setCharacterSize(32);
    text.setColor(sf::Color(0xFF, 0x80, 0x00));
    text.setString("Akagoria, the revenge of Kalista");

    // put the splash screen in the center
    sf::FloatRect rect = text.getLocalBounds();
    text.setOrigin(rect.left, rect.top);

    float x = gWindowGeometry().getXCentered(rect.width);
    float y = gWindowGeometry().getYCentered(rect.height);

    text.setPosition(x, y);
    window.draw(text);

    if (loading) {
      sf::Text loadingText;
      loadingText.setFont(*m_font);
      loadingText.setCharacterSize(20);
      loadingText.setColor(sf::Color(0xFF, 0x80, 0x00));
      loadingText.setString("Loading...");
      loadingText.setStyle(sf::Text::Italic);

      // put the splash screen in the center
      sf::FloatRect loadingRect = loadingText.getLocalBounds();
      loadingText.setOrigin(loadingRect.left, loadingRect.top);

      float loadingX = gWindowGeometry().getXCentered(loadingRect.width);
      float loadingY = y + LOADING_PADDING;

      loadingText.setPosition(loadingX, loadingY);
      window.draw(loadingText);
    }
  }

  static constexpr float MENU_POS = 2.0f;
  static constexpr float MENU_LEFT = 25.0f;
  static constexpr float MENU_POINTER = MENU_LEFT / 2;

  void MenuUI::onUp() {
    m_currentChoice = (m_currentChoice + m_choiceCount - 1) % m_choiceCount;
  }

  void MenuUI::onDown() {
    m_currentChoice = (m_currentChoice + 1) % m_choiceCount;
  }

  static constexpr int SELECT_CHOICE_COUNT = 3;

  static constexpr float SELECT_SLOT_HEIGHT = 65.0f;
  static constexpr float SELECT_SLOT_MARGIN = 5.0f;
  static constexpr float SELECT_SLOT_PADDING = 10.0f;
  static constexpr unsigned SELECT_SLOT_SIZE = 16;

  static constexpr float SELECT_WIDTH = 300.0f;
  static constexpr float SELECT_HEIGHT = SELECT_CHOICE_COUNT * SELECT_SLOT_HEIGHT + 50.0f;

  static constexpr float SELECT_SLOT_WIDTH = SELECT_WIDTH - MENU_POS - MENU_LEFT - SELECT_SLOT_MARGIN;

  SelectSlotUI::SelectSlotUI()
  : MenuUI(SELECT_CHOICE_COUNT + 1) // 3 slots + back to main
  {
    m_font = gResourceManager().getFont("fonts/DejaVuSans.ttf");
    assert(m_font);
  }

  void SelectSlotUI::render(sf::RenderWindow& window) {
    drawBox(window, MENU_POS, MENU_POS, SELECT_WIDTH, SELECT_HEIGHT);

    float x = MENU_POS + MENU_LEFT;
    float y = MENU_POS + SELECT_SLOT_MARGIN;

    int choice = getCurrentChoice();
    float pointerX = MENU_POS + MENU_POINTER;

    drawBox(window, x, y, SELECT_SLOT_WIDTH,  SELECT_SLOT_HEIGHT);
    drawText(window, *m_font, x + SELECT_SLOT_PADDING, y + SELECT_SLOT_MARGIN, SELECT_SLOT_SIZE, gSavePointManager().getSlotInfo(0));

    if (choice == 0) {
      float pointerY = y + SELECT_SLOT_HEIGHT / 2;
      drawPointer(window, pointerX, pointerY);
    }

    y += SELECT_SLOT_HEIGHT + SELECT_SLOT_MARGIN;
    drawBox(window, x, y, SELECT_SLOT_WIDTH,  SELECT_SLOT_HEIGHT);
    drawText(window, *m_font, x + SELECT_SLOT_PADDING, y + SELECT_SLOT_MARGIN, SELECT_SLOT_SIZE, gSavePointManager().getSlotInfo(1));

    if (choice == 1) {
      float pointerY = y + SELECT_SLOT_HEIGHT / 2;
      drawPointer(window, pointerX, pointerY);
    }

    y += SELECT_SLOT_HEIGHT + SELECT_SLOT_MARGIN;
    drawBox(window, x, y, SELECT_SLOT_WIDTH,  SELECT_SLOT_HEIGHT);
    drawText(window, *m_font, x + SELECT_SLOT_PADDING, y + SELECT_SLOT_MARGIN, SELECT_SLOT_SIZE, gSavePointManager().getSlotInfo(2));

    if (choice == 2) {
      float pointerY = y + SELECT_SLOT_HEIGHT / 2;
      drawPointer(window, pointerX, pointerY);
    }

    y += SELECT_SLOT_HEIGHT + 2 * SELECT_SLOT_MARGIN;
    drawText(window, *m_font, x, y, STANDARD_SIZE, "Back");

    if (choice == 3) {
      float pointerY = y + 0.4 * STANDARD_SIZE;
      drawPointer(window, pointerX, pointerY);
    }

  }


  static constexpr int START_CHOICE_COUNT = 3;

  static constexpr float START_WIDTH = 300.0f;
  static constexpr float START_HEIGHT = 100.0f;
  static constexpr float START_PADDING = 10.0f;
  static constexpr unsigned START_SIZE = STANDARD_SIZE;

  StartUI::StartUI()
  : MenuUI(START_CHOICE_COUNT)
  {
    m_font = gResourceManager().getFont("fonts/DejaVuSans.ttf");
    assert(m_font);
  }

  void StartUI::render(sf::RenderWindow& window) {
    drawBox(window, MENU_POS, MENU_POS, START_WIDTH, START_HEIGHT);

    float x = MENU_POS + MENU_LEFT;
    float y = MENU_POS + START_PADDING;

    drawText(window, *m_font, x, y, START_SIZE, "Start new adventure");
    y += START_SIZE + START_PADDING;
    drawText(window, *m_font, x, y, START_SIZE, "Load adventure");
    y += START_SIZE + START_PADDING;
    drawText(window, *m_font, x, y, START_SIZE, "Quit");

    float pointerX = MENU_POS + MENU_POINTER;
    float pointerY = MENU_POS + START_PADDING + 0.4f * START_SIZE + getCurrentChoice() * (START_SIZE + START_PADDING);
    drawPointer(window, pointerX, pointerY);
  }


}
