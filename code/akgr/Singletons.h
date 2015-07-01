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
#ifndef AKGR_SINGLETONS_H
#define AKGR_SINGLETONS_H

#include <game/EntityManager.h>
#include <game/EventManager.h>
#include <game/ResourceManager.h>
#include <game/Singleton.h>
#include <game/WindowGeometry.h>

#include "PhysicsModel.h"
#include "DataManager.h"
#include "DialogManager.h"

namespace akgr {

  extern game::Singleton<game::ResourceManager> gResourceManager;
  extern game::Singleton<game::EventManager> gEventManager;
  extern game::Singleton<game::EntityManager> gMainEntityManager;
  extern game::Singleton<game::EntityManager> gHeadsUpEntityManager;

  extern game::Singleton<DataManager> gDataManager;

  extern game::Singleton<PhysicsModel> gPhysicsModel;

  extern game::Singleton<DialogManager> gDialogManager;

  extern game::Singleton<game::WindowGeometry> gWindowGeometry;

}


#endif // AKGR_SINGLETONS_H
