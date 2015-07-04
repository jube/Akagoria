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
#include "RequirementManager.h"

namespace akgr {

  bool RequirementManager::hasRequirement(const std::string& req) {
    auto id = game::Hash(req);
    return hasRequirement(id);
  }

  bool RequirementManager::hasRequirement(game::Id req) {
    return m_requirements.find(req) != m_requirements.end();
  }

  void RequirementManager::addRequirement(const std::string& req) {
    auto id = game::Hash(req);
    addRequirement(id);
  }

  void RequirementManager::addRequirement(game::Id req) {
    m_requirements.insert(req);
  }

  void RequirementManager::removeRequirement(const std::string& req) {
    auto id = game::Hash(req);
    removeRequirement(id);
  }

  void RequirementManager::removeRequirement(game::Id req) {
    m_requirements.erase(req);
  }

}
