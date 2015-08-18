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
#include "MessageManager.h"

#include "DataManager.h"
#include "Singletons.h"

namespace akgr {


  void MessageManager::postMessage(const std::string& name, float time) {
    auto data = gDataManager().getMessageDataFor(name);

    if (data == nullptr) {
      return;
    }

    m_items.push_back({ time, data });
  }

  void MessageManager::update(float dt)  {
    if (m_items.empty()) {
      return;
    }

    auto& currentItem = m_items.front();
    currentItem.remainingTime -= dt;

    if (currentItem.remainingTime < 0) {
      m_items.pop_front();
    }
  }

  void MessageManager::render(sf::RenderWindow& window)  {
    if (m_items.empty()) {
      return;
    }

    auto& currentItem = m_items.front();
    m_ui.setMessage(*currentItem.data);
    m_ui.render(window);
  }

}
