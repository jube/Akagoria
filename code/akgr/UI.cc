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

  static constexpr unsigned SPEAKER_SIZE = 16;
  static constexpr float SPEAKER_WIDTH = 150.0f;
  static constexpr float SPEAKER_HEIGHT = 25.0f;
  static constexpr float SPEAKER_PADDING = 5.0f;

  static constexpr unsigned WORDS_SIZE = 20;
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


  static constexpr int CHOICE_COUNT = 3;

  static constexpr float START_WIDTH = 300.0f;
  static constexpr float START_HEIGHT = 100.0f;
  static constexpr float START_LEFT = 25.0f;
  static constexpr float START_PADDING = 10.0f;
  static constexpr unsigned START_SIZE = 20.0f;
  static constexpr float START_RADIUS = 5.0f;
  static constexpr float START_POS = 2.0f;

  StartUI::StartUI()
  : m_currentChoice(0)
  {
    m_font = gResourceManager().getFont("fonts/DejaVuSans.ttf");
    assert(m_font);
  }

  void StartUI::moveDown() {
    m_currentChoice = (m_currentChoice + 1) % CHOICE_COUNT;
  }

  void StartUI::moveUp() {
    m_currentChoice = (m_currentChoice + CHOICE_COUNT - 1) % CHOICE_COUNT;
  }

  void StartUI::render(sf::RenderWindow& window) {
    displaySplashMessage(window);

    drawBox(window, START_POS, START_POS, START_WIDTH, START_HEIGHT);

    drawText(window, *m_font, START_POS + START_LEFT, START_POS + START_PADDING, START_SIZE, "Start new adventure");
    drawText(window, *m_font, START_POS + START_LEFT, START_POS + START_PADDING + 1 * (START_SIZE + START_PADDING), START_SIZE, "Load adventure");
    drawText(window, *m_font, START_POS + START_LEFT, START_POS + START_PADDING + 2 * (START_SIZE + START_PADDING), START_SIZE, "Quit");

    float x = START_POS + START_LEFT / 2;
    float y = START_POS + START_PADDING + 0.4f * START_SIZE + m_currentChoice * (START_SIZE + START_PADDING);

    sf::CircleShape pointer(START_RADIUS, 3);
    pointer.setOrigin(START_RADIUS, START_RADIUS);
    pointer.setPosition(x, y);
    pointer.rotate(90);
    pointer.setFillColor(sf::Color::White);
    window.draw(pointer);
  }

  static constexpr float LOADING_PADDING = 60.0f;

  void StartUI::displaySplashMessage(sf::RenderWindow& window, bool loading) {
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

}
