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
    m_ui.setDialogLine(m_currentDialog->content[m_currentLine]);
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

    m_ui.setDialogLine(m_currentDialog->content[m_currentLine]);
    return true;
  }

  bool DialogManager::hasNextLine() const {
    return m_currentDialog != nullptr;
  }

  void DialogManager::render(sf::RenderWindow& window) {
    if (m_currentDialog == nullptr) {
      return;
    }

    m_ui.render(window);
  }

}
