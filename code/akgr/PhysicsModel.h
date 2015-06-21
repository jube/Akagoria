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
#ifndef AKGR_PHYSICS_MODEL_H
#define AKGR_PHYSICS_MODEL_H

#include <Box2D/Box2D.h>

#include <game/Model.h>

namespace akgr {

  class PhysicsModel : public game::Model {
  public:
    PhysicsModel();

    b2World& getWorld() {
      return m_world;
    }

    virtual void update(float dt) override;

  private:
    b2World m_world;
  };

}


#endif // AKGR_PHYSICS_MODEL_H
