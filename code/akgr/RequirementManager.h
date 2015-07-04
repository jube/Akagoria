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
#ifndef AKGR_REQUIREMENT_MANAGER_H
#define AKGR_REQUIREMENT_MANAGER_H

#include <set>
#include <string>

#include <game/Id.h>

namespace akgr {

  class RequirementManager {
  public:

    bool hasRequirement(const std::string& req);
    bool hasRequirement(game::Id req);

    void addRequirement(const std::string& req);
    void addRequirement(game::Id req);

    void removeRequirement(const std::string& req);
    void removeRequirement(game::Id req);

  private:
    std::set<game::Id> m_requirements;
  };

}

#endif // AKGR_REQUIREMENT_MANAGER_H
