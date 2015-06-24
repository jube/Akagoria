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
#include "Body.h"

#include "PhysicsModel.h"

namespace akgr {

  Body::Body(int floor, b2Body *body)
  : m_floor(floor)
  , m_body(body)
  {

  }

  Location Body::getLocation() const {
    return { getPosition(), getFloor() };
  }

  int Body::getFloor() const {
    return m_floor;
  }

  sf::Vector2f Body::getPosition() const {
    auto pos = m_body->GetPosition();
    return { pos.x / PhysicsModel::BOX2D_SCALE, pos.y / PhysicsModel::BOX2D_SCALE };
  }

  float Body::getAngle() const {
    return m_body->GetAngle();
  }

  void Body::setAngleAndVelocity(float angle, float velocity) {
    m_body->SetTransform(m_body->GetPosition(), angle);
    m_body->SetAngularVelocity(0.0f);

    b2Rot rot(angle);
    b2Vec2 vel(0.0f, velocity * PhysicsModel::BOX2D_SCALE);
    vel = b2Mul(rot, vel);

    m_body->SetLinearVelocity(vel);
  }

}
