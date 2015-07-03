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

  static constexpr float SPEAKER_WIDTH = 150.0f;
  static constexpr float SPEAKER_HEIGHT = 25.0f;
  static constexpr float SPEAKER_PADDING = 5.0f;

  static constexpr float DIALOG_WIDTH = 600.0f;
  static constexpr float DIALOG_HEIGHT = 90.0f;
  static constexpr float DIALOG_BOTTOM = 40.0f;
  static constexpr float DIALOG_PADDING = 10.0f;

  static constexpr unsigned LINE_SIZE = 20;
  static constexpr unsigned SPEAKER_SIZE = 16;

  void DialogManager::render(sf::RenderWindow& window) {
    if (m_currentDialog == nullptr) {
      return;
    }

    float x = gWindowGeometry().getXCentered(DIALOG_WIDTH);
    float y = gWindowGeometry().getYFromBottom(DIALOG_HEIGHT + DIALOG_BOTTOM);


    sf::RectangleShape speakerShape({ SPEAKER_WIDTH, SPEAKER_HEIGHT });
    speakerShape.setPosition(x + DIALOG_PADDING, y - SPEAKER_HEIGHT);
    speakerShape.setFillColor(sf::Color(0x04, 0x08, 0x84, 0xC0));
    speakerShape.setOutlineColor(sf::Color::White);
    speakerShape.setOutlineThickness(1);
    window.draw(speakerShape);

    sf::RectangleShape lineShape({ DIALOG_WIDTH, DIALOG_HEIGHT });
    lineShape.setPosition(x, y);
//    lineShape.setFillColor(sf::Color(0xFF, 0xCF, 0x86, 0xC0));
    lineShape.setFillColor(sf::Color(0x04, 0x08, 0x84, 0xC0));
    lineShape.setOutlineColor(sf::Color::White);
    lineShape.setOutlineThickness(1);
    window.draw(lineShape);

    sf::Text text;
    text.setFont(*m_font);
    text.setCharacterSize(LINE_SIZE);
    text.setColor(sf::Color::White);

    text.setString(m_currentDialog->content[m_currentLine].line);
    sf::FloatRect rect = text.getLocalBounds();
    text.setOrigin(rect.left, rect.top);
    text.setPosition(x + DIALOG_PADDING, y + DIALOG_PADDING);
    window.draw(text);

    text.setCharacterSize(SPEAKER_SIZE);
    text.setString(m_currentDialog->content[m_currentLine].speaker);
    rect = text.getLocalBounds();
    text.setOrigin(rect.left, rect.top);
    text.setPosition(x + DIALOG_PADDING + 2 * SPEAKER_PADDING, y - SPEAKER_HEIGHT + SPEAKER_PADDING);
    window.draw(text);

  }

}
