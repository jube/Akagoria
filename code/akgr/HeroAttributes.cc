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
#include "HeroAttributes.h"

#include "Singletons.h"

namespace akgr {

  static constexpr int INITIAL_HP = 100;
  static constexpr int INITIAL_MP = 100;

  HeroAttributes::HeroAttributes()
  : m_healthPoints(0.75 * INITIAL_HP), m_healthPointsMax(INITIAL_HP)
  , m_magicPoints(0.5 * INITIAL_MP), m_magicPointsMax(INITIAL_MP)
  {
    m_font = gResourceManager().getFont("fonts/DejaVuSansMono-Bold.ttf");
    assert(m_font);
  }

  void HeroAttributes::update(float dt) {

  }

  static constexpr float ATTR_MARGIN_X = 10.0f;
  static constexpr float ATTR_MARGIN_Y = 10.0f;
  static constexpr float ATTR_MARGIN = 4.0f;

  static constexpr float ATTR_WIDTH = 400.0f;
  static constexpr float ATTR_HEIGHT = 15.0f;

  static constexpr unsigned ATTR_SIZE = 15;

  static constexpr float ATTR_SHIFT = 1.0f;

  static constexpr std::size_t BUFFER_SIZE = 1024;

  static void drawBar(sf::RenderWindow& window, float x, float y, float points, sf::Color color) {
    sf::Color gray(0x80, 0x80, 0x80);

    sf::RectangleShape rect({ ATTR_WIDTH, ATTR_HEIGHT });
    rect.setPosition(x, y);
    rect.setFillColor(gray);
    rect.setOutlineThickness(1.0f);
    rect.setOutlineColor(gray);
    window.draw(rect);

    rect.setSize({ points * ATTR_WIDTH, ATTR_HEIGHT });
    rect.setFillColor(color);
    window.draw(rect);
  }

  static void drawName(sf::RenderWindow& window, sf::Font& font, float x, float y, const char *name) {
    sf::Text text;
    text.setCharacterSize(ATTR_SIZE);
    text.setFont(font);
    text.setString(name);
    auto bounds = text.getLocalBounds();
    text.setOrigin(bounds.left, bounds.top);
    text.setColor(sf::Color::Black);
    text.setPosition(x, y);
    window.draw(text);

    text.setColor(sf::Color::White);
    text.move(ATTR_SHIFT, ATTR_SHIFT);
    window.draw(text);
  }

  static void drawNumbers(sf::RenderWindow& window, sf::Font& font, float x, float y, int points, int pointsMax) {
    std::array<char, BUFFER_SIZE> buffer;

    std::snprintf(buffer.data(), buffer.size(), "%i/%i", points, pointsMax);
    sf::Text text;
    text.setCharacterSize(ATTR_SIZE);
    text.setFont(font);
    text.setString(buffer.data());
    auto bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
    text.setColor(sf::Color::Black);
    text.setPosition(x, y);
    window.draw(text);

    text.setColor(sf::Color::White);
    text.move(ATTR_SHIFT, ATTR_SHIFT);
    window.draw(text);
  }


  void HeroAttributes::render(sf::RenderWindow& window) {
    float d = (ATTR_HEIGHT + 2.0f - ATTR_SIZE) / 2 - ATTR_SHIFT / 2;
    float mx = ATTR_MARGIN_X + (ATTR_WIDTH - ATTR_SHIFT) / 2;
    float my = ATTR_MARGIN_Y + (ATTR_HEIGHT - ATTR_SHIFT) / 2;

    // HP

    float hp = static_cast<float>(m_healthPoints) / static_cast<float>(m_healthPointsMax);
    drawBar(window, ATTR_MARGIN_X, ATTR_MARGIN_Y, hp, sf::Color(0xFF, 0x80, 0x80));
    drawName(window, *m_font, ATTR_MARGIN_X + d, ATTR_MARGIN_Y + d, "HP");
    drawNumbers(window, *m_font, mx, my, m_healthPoints, m_healthPointsMax);

    // MP

    float mp = static_cast<float>(m_magicPoints) / static_cast<float>(m_magicPointsMax);
    drawBar(window, ATTR_MARGIN_X, ATTR_MARGIN_Y + ATTR_HEIGHT + ATTR_MARGIN, mp, sf::Color(0x80, 0x80, 0xFF));
    drawName(window, *m_font, ATTR_MARGIN_X + d, ATTR_MARGIN_Y + ATTR_HEIGHT + ATTR_MARGIN + d, "MP");
    drawNumbers(window, *m_font, mx, my + ATTR_HEIGHT + ATTR_MARGIN, m_magicPoints, m_magicPointsMax);
  }

}
