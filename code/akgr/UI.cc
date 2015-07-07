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



}
