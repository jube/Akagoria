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

#include <cassert>

#include <game/Log.h>

#include "PhysicsModel.h"

namespace akgr {
  Body::Body()
  : m_floor(0)
  , m_body(nullptr)
  {

  }

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
    assert(m_body);
    auto pos = m_body->GetPosition();
    return { pos.x / PhysicsModel::BOX2D_SCALE, pos.y / PhysicsModel::BOX2D_SCALE };
  }

  float Body::getAngle() const {
    assert(m_body);
    return m_body->GetAngle();
  }

  void Body::setAngleAndVelocity(float angle, float velocity) {
    assert(m_body);
    m_body->SetTransform(m_body->GetPosition(), angle);
    m_body->SetAngularVelocity(0.0f);

    b2Rot rot(angle);
    b2Vec2 vel(0.0f, velocity * PhysicsModel::BOX2D_SCALE);
    vel = b2Mul(rot, vel);

    m_body->SetLinearVelocity(vel);
  }

  static constexpr int MAX_FLOOR = 7;
  static constexpr int MIN_FLOOR = -6;

  void Body::moveUp() {
    if (m_floor > MAX_FLOOR - 2) {
      game::Log::error(game::Log::PHYSICS, "Trying to go above the maximum floor: %d\n", m_floor);
    }

    for (b2Fixture *fixture = m_body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
      b2Filter filter = fixture->GetFilterData();
      assert(filter.categoryBits == filter.maskBits);
      filter.categoryBits <<= 2;
      filter.maskBits <<= 2;
      fixture->SetFilterData(filter);
    }

    m_floor += 2;
  }

  void Body::moveDown() {
    if (m_floor < MIN_FLOOR - 2) {
      game::Log::error(game::Log::PHYSICS, "Trying to go below the minimum floor: %d\n", m_floor);
    }

    for (b2Fixture *fixture = m_body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
      b2Filter filter = fixture->GetFilterData();
      assert(filter.categoryBits == filter.maskBits);
      filter.categoryBits >>= 2;
      filter.maskBits >>= 2;
      fixture->SetFilterData(filter);
    }

    m_floor -= 2;
  }

  void Body::moveInside() {
    if (m_floor % 2 != 0) {
      game::Log::error(game::Log::PHYSICS, "Trying to go inside while not ouside: %d\n", m_floor);
    }

    for (b2Fixture *fixture = m_body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
      b2Filter filter = fixture->GetFilterData();
      assert(filter.categoryBits == filter.maskBits);
      filter.categoryBits <<= 1;
      filter.maskBits <<= 1;
      fixture->SetFilterData(filter);
    }

    m_floor += 1;
  }

  void Body::moveOutside() {
    if (m_floor % 2 != 1) {
      game::Log::error(game::Log::PHYSICS, "Trying to go outside while not inside: %d\n", m_floor);
    }

    for (b2Fixture *fixture = m_body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
      b2Filter filter = fixture->GetFilterData();
      assert(filter.categoryBits == filter.maskBits);
      filter.categoryBits >>= 1;
      filter.maskBits >>= 1;
      fixture->SetFilterData(filter);
    }

    m_floor -= 1;
  }

}
