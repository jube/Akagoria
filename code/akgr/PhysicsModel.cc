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
#include "PhysicsModel.h"

namespace akgr {
  PhysicsModel::PhysicsModel()
  : m_world({ 0.0f, 0.0f })
  {

  }

  void PhysicsModel::update(float dt) {
    int32 velocityIterations = 10; // 6;
    int32 positionIterations = 8; // 2;
    m_world.Step(dt, velocityIterations, positionIterations);
  }

}
