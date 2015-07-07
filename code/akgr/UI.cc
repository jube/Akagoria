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

    const sf::Color fillColor(0x04, 0x08, 0x84, 0xC0);

    // draw speaker box and text

    sf::RectangleShape speakerShape({ SPEAKER_WIDTH, SPEAKER_HEIGHT });
    speakerShape.setPosition(x + WORDS_PADDING, y - SPEAKER_HEIGHT);
    speakerShape.setFillColor(fillColor);
    speakerShape.setOutlineColor(sf::Color::White);
    speakerShape.setOutlineThickness(1);
    window.draw(speakerShape);

    sf::Text speakerText;
    speakerText.setFont(*m_font);
    speakerText.setCharacterSize(SPEAKER_SIZE);
    speakerText.setColor(sf::Color::White);
    speakerText.setString(m_currentLine->speaker);
    auto speakerRect = speakerText.getLocalBounds();
    speakerText.setOrigin(speakerRect.left, speakerRect.top);
    speakerText.setPosition(x + WORDS_PADDING + SPEAKER_PADDING, y - SPEAKER_HEIGHT + SPEAKER_PADDING);
    window.draw(speakerText);

    // draw words box and text

    sf::RectangleShape wordsShape({ WORDS_WIDTH, WORDS_HEIGHT });
    wordsShape.setPosition(x, y);
    wordsShape.setFillColor(fillColor);
    wordsShape.setOutlineColor(sf::Color::White);
    wordsShape.setOutlineThickness(1);
    window.draw(wordsShape);

    sf::Text wordsText;
    wordsText.setFont(*m_font);
    wordsText.setCharacterSize(WORDS_SIZE);
    wordsText.setColor(sf::Color::White);
    wordsText.setString(m_currentLine->words);
    auto wordsRect = wordsText.getLocalBounds();
    wordsText.setOrigin(wordsRect.left, wordsRect.top);
    wordsText.setPosition(x + WORDS_PADDING, y + WORDS_PADDING);
    window.draw(wordsText);
  }

}
