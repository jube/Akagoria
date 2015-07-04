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
#include "DialogManager.h"

#include <cassert>

#include <yaml-cpp/yaml.h>

#include <game/Log.h>

#include "DataManager.h"
#include "GameEvents.h"
#include "Singletons.h"

namespace akgr {

  DialogManager::DialogManager()
  : m_currentDialog(nullptr)
  , m_currentLine(0)
  {
    m_font = gResourceManager().getFont("fonts/DejaVuSans.ttf");
    assert(m_font);
  }

  bool DialogManager::start(const std::string& name) {
    if (m_currentDialog != nullptr) {
      game::Log::warning(game::Log::GENERAL, "Can not load a dialogue because one is already loaded: '%s'\n", name.c_str());
      return false;
    }

    m_currentDialogName = name;
    m_currentDialog = gDataManager().getDialogDataFor(name);
    m_currentLine = 0;

    if (m_currentDialog == nullptr) {
      return false;
    }

    assert(!m_currentDialog->content.empty());
    game::Log::info(game::Log::GENERAL, "Dialog loaded: '%s'\n", name.c_str());
//     game::Log::info(game::Log::GENERAL, "Dialog first line:: '%s'\n", m_currentDialog->content[m_currentLine].line.c_str());
    return true;
  }

  bool DialogManager::showNextLine() {
    if (m_currentDialog == nullptr) {
      game::Log::warning(game::Log::GENERAL, "The dialogue is not available anymore.\n");
      return false;
    }

    m_currentLine++;

    if (m_currentLine == m_currentDialog->content.size()) {
      game::Log::info(game::Log::GENERAL, "End of the dialogue.\n");
      m_currentDialog = nullptr;

      DialogEndEvent event;
      event.name = m_currentDialogName;
      gEventManager().triggerEvent(&event);

      return false;
    }

    return true;
  }

  bool DialogManager::hasNextLine() const {
    return m_currentDialog != nullptr;
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

  void DialogManager::render(sf::RenderWindow& window) {
    if (m_currentDialog == nullptr) {
      return;
    }

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
    speakerText.setString(m_currentDialog->content[m_currentLine].speaker);
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
    wordsText.setString(m_currentDialog->content[m_currentLine].words);
    auto wordsRect = wordsText.getLocalBounds();
    wordsText.setOrigin(wordsRect.left, wordsRect.top);
    wordsText.setPosition(x + WORDS_PADDING, y + WORDS_PADDING);
    window.draw(wordsText);
  }

}
