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

#include <game/Log.h>

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

  static constexpr uint16_t THIRD_FLOOR_INSIDE       = 0x8000;
  static constexpr uint16_t THIRD_FLOOR_OUTSIDE      = 0x4000;
  static constexpr uint16_t SECOND_FLOOR_INSIDE      = 0x2000;
  static constexpr uint16_t SECOND_FLOOR_OUTSIDE     = 0x1000;
  static constexpr uint16_t FIRST_FLOOR_INSIDE       = 0x0800;
  static constexpr uint16_t FIRST_FLOOR_OUTSIDE      = 0x0400;
  static constexpr uint16_t GROUND_FLOOR_INSIDE      = 0x0200;
  static constexpr uint16_t GROUND_FLOOR_OUTSIDE     = 0x0100;
  static constexpr uint16_t FIRST_BASEMENT_INSIDE    = 0x0080;
  static constexpr uint16_t FIRST_BASEMENT_OUTSIDE   = 0x0040;
  static constexpr uint16_t SECOND_BASEMENT_INSIDE   = 0x0020;
  static constexpr uint16_t SECOND_BASEMENT_OUTSIDE  = 0x0010;
  static constexpr uint16_t THIRD_BASEMENT_INSIDE    = 0x0008;
  static constexpr uint16_t THIRD_BASEMENT_OUTSIDE   = 0x0004;

  static uint16_t bitsFromFloor(int floor) {
    switch (floor) {
      case 7:
        return THIRD_FLOOR_INSIDE;
      case 6:
        return THIRD_FLOOR_OUTSIDE;
      case 5:
        return SECOND_FLOOR_INSIDE;
      case 4:
        return SECOND_FLOOR_OUTSIDE;
      case 3:
        return FIRST_FLOOR_INSIDE;
      case 2:
        return FIRST_FLOOR_OUTSIDE;
      case 1:
        return GROUND_FLOOR_INSIDE;
      case 0:
        return GROUND_FLOOR_OUTSIDE;
      case -1:
        return FIRST_BASEMENT_INSIDE;
      case -2:
        return FIRST_BASEMENT_OUTSIDE;
      case -3:
        return SECOND_BASEMENT_INSIDE;
      case -4:
        return SECOND_BASEMENT_OUTSIDE;
      case -5:
        return THIRD_BASEMENT_INSIDE;
      case -6:
        return THIRD_BASEMENT_OUTSIDE;
      default:
        return GROUND_FLOOR_OUTSIDE;
    }

    return 0xFFFF;
  }

  static b2FixtureDef createFixture(int floor, bool isSolid, bool isSensor) {
    auto floorBits = bitsFromFloor(floor);

    b2FixtureDef fixture;
    fixture.isSensor = isSensor;
    fixture.filter.categoryBits = fixture.filter.maskBits = (isSolid ? floorBits : 0x0000);
    fixture.density = 1.0f;
    fixture.friction = 0.0f;
    fixture.restitution = 0.0f;

    return fixture;
  }

  static b2Fixture *createCircleFixture(b2Body *body, int floor, bool isSolid, bool isSensor, float radius) {
    assert(body);

    auto fixture = createFixture(floor, isSolid, isSensor);

    b2CircleShape shape;
    fixture.shape = &shape;
    shape.m_radius = radius * PhysicsModel::BOX2D_SCALE;

    return body->CreateFixture(&fixture);
  }

  static b2Fixture *createRectangleFixture(b2Body *body, int floor, bool isSolid, bool isSensor, float width, float height) {
    assert(body);

    auto fixture = createFixture(floor, isSolid, isSensor);

    b2PolygonShape shape;
    fixture.shape = &shape;
    shape.SetAsBox(width  * PhysicsModel::BOX2D_SCALE * 0.5f, height * PhysicsModel::BOX2D_SCALE * 0.5f);

    return body->CreateFixture(&fixture);
  }

  static void addFixtureToBody(b2Body *body, int floor, bool isSolid, bool isSensor, const CollisionData *data) {
    assert(data);

    switch (data->shape) {
      case CollisionShape::CIRCLE:
        createCircleFixture(body, floor, isSolid, isSensor, data->circle.radius);
        break;
      case CollisionShape::RECTANGLE:
        createRectangleFixture(body, floor, isSolid, isSensor, data->rectangle.width, data->rectangle.height);
        break;
      default:
        game::Log::warning(game::Log::PHYSICS, "Unknown collision data shape\n");
        break;
    }
  }

  void PhysicsModel::addMapItem(float x, float y, int floor, const CollisionData *data) {
    b2BodyDef def;
    def.type = b2_staticBody;
    def.position = { x * BOX2D_SCALE, y * BOX2D_SCALE };
    auto body = m_world.CreateBody(&def);
    addFixtureToBody(body, floor, true, false, data);
  }

  Body PhysicsModel::createHeroBody(float x, float y, int floor, const CollisionData *data) {
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position = { x * BOX2D_SCALE, y * BOX2D_SCALE };
    auto body = m_world.CreateBody(&def);
    addFixtureToBody(body, floor, true, false, data);
    return Body(floor, body);
  }

}
