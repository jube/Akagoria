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
      game::Log::warning(game::Log::RESOURCES, "Can not load a dialogue because one is already loaded: '%s'\n", name.c_str());
      return false;
    }

    m_currentDialog = gDataManager().getDialogDataFor(name);
    m_currentLine = 0;

    if (m_currentDialog == nullptr) {
      return false;
    }

    assert(!m_currentDialog->content.empty());
    game::Log::info(game::Log::RESOURCES, "Dialog loaded: '%s'\n", name.c_str());
    game::Log::info(game::Log::RESOURCES, "Dialog first line:: '%s'\n", m_currentDialog->content[m_currentLine].line.c_str());
    return true;
  }

  bool DialogManager::nextLine() {
    if (m_currentDialog == nullptr) {
      game::Log::warning(game::Log::RESOURCES, "The dialogue is not available anymore.\n");
      return false;
    }

    m_currentLine++;

    if (m_currentLine == m_currentDialog->content.size()) {
      game::Log::info(game::Log::RESOURCES, "End of the dialogue.\n");
      m_currentDialog = nullptr;
      return false;
    }

    return true;
  }

  static constexpr float DIALOGUE_WIDTH = 600.0f;
  static constexpr float DIALOGUE_HEIGHT = 90.0f;
  static constexpr float DIALOGUE_BOTTOM = 40.0f;
  static constexpr float DIALOGUE_PADDING = 10.0f;

  static constexpr unsigned LINE_SIZE = 20;
  static constexpr unsigned SPEAKER_SIZE = 16;

  void DialogManager::render(sf::RenderWindow& window) {
    if (m_currentDialog == nullptr) {
      return;
    }

    float x = gWindowGeometry().getXCentered(DIALOGUE_WIDTH);
    float y = gWindowGeometry().getYFromBottom(DIALOGUE_HEIGHT + DIALOGUE_BOTTOM);

    sf::RectangleShape shape({ DIALOGUE_WIDTH, DIALOGUE_HEIGHT });
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color(0xFF, 0xCF, 0x86, 0xC0));
    window.draw(shape);

    sf::Text text;
    text.setFont(*m_font);
    text.setCharacterSize(LINE_SIZE);
    text.setColor(sf::Color::Black);

    text.setString(m_currentDialog->content[m_currentLine].line);
    sf::FloatRect rect = text.getLocalBounds();
    text.setOrigin(rect.left, rect.top);
    text.setPosition(x + DIALOGUE_PADDING, y + DIALOGUE_PADDING);
    window.draw(text);

    text.setCharacterSize(SPEAKER_SIZE);
    text.setString(m_currentDialog->content[m_currentLine].speaker);
    text.setStyle(sf::Text::Bold);
    rect = text.getLocalBounds();
    text.setOrigin(rect.left, rect.top);
    text.setPosition(x + DIALOGUE_PADDING, y - 30);
    window.draw(text);

  }

}
