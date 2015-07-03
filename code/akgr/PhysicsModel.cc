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

#include <cinttypes>

#include <tmx/Object.h>
#include <tmx/ObjectLayer.h>

#include <game/Event.h>
#include <game/Log.h>

#include "Singletons.h"

namespace akgr {

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

  static b2Fixture *createChainFixture(b2Body *body, int floor, bool isSolid, bool isSensor, const std::vector<b2Vec2>& points, bool isClosed) {
    assert(body);

    auto fixture = createFixture(floor, isSolid, isSensor);

    b2ChainShape shape;
    fixture.shape = &shape;

    if (isClosed) {
      shape.CreateLoop(points.data(), points.size());
    } else {
      shape.CreateChain(points.data(), points.size());
    }

    return body->CreateFixture(&fixture);
  }

  static b2Fixture *createFixtureFromObject(b2World& world, const tmx::Object *object, int floor, bool isSensor) {
    if (object->isRectangle()) {
      auto rectangleObject = static_cast<const tmx::Rectangle*>(object);

      float x = rectangleObject->getX();
      float y = rectangleObject->getY();
      float width = rectangleObject->getWidth();
      float height = rectangleObject->getHeight();

      x += width * 0.5f;
      y += height * 0.5f;

      b2BodyDef def;
      def.type = b2_staticBody;
      def.position = { x * PhysicsModel::BOX2D_SCALE, y * PhysicsModel::BOX2D_SCALE };
      auto body = world.CreateBody(&def);

      return createRectangleFixture(body, floor, true, isSensor, width, height);
    }

    if (object->isChain()) {
      auto chainObject = static_cast<const tmx::Chain *>(object);

      float x = chainObject->getX();
      float y = chainObject->getY();

      std::vector<b2Vec2> chain;

      for (auto point : *chainObject) {
        chain.emplace_back(point.x * PhysicsModel::BOX2D_SCALE, point.y * PhysicsModel::BOX2D_SCALE);
      }

      b2BodyDef def;
      def.type = b2_staticBody;
      def.position = { x * PhysicsModel::BOX2D_SCALE, y * PhysicsModel::BOX2D_SCALE };
      auto body = world.CreateBody(&def);

      return createChainFixture(body, floor, true, isSensor, chain, object->isPolygon());
    }

    return nullptr;
  }


  class PhysicsListener : public b2ContactListener {
  public:
    void addEventZone(int floor, const std::string& name, const tmx::Object *object, const std::string& event) {
      auto fixture = createFixtureFromObject(gPhysicsModel().getWorld(), object, floor, true);

      if (fixture == nullptr) {
        game::Log::warning(game::Log::PHYSICS, "An event zone could not be transformed into a fixture: '%s'\n", name.c_str());
        return;
      }

      auto type = game::Hash(event);

      m_eventZones[fixture] = type;
      m_eventNames[type] = event;
    }

    void addCollisionZone(int floor, const std::string& name, const tmx::Object *object) {
      auto fixture = createFixtureFromObject(gPhysicsModel().getWorld(), object, floor, false);

      if (fixture == nullptr) {
        game::Log::warning(game::Log::PHYSICS, "A collision zone could not be transformed into a fixture: '%s'\n", name.c_str());
        return;
      }
    }

    virtual void BeginContact(b2Contact* contact) override {
      b2Fixture* fixtureA = contact->GetFixtureA();
      b2Fixture* fixtureB = contact->GetFixtureB();

      triggerZoneEvent(fixtureB, fixtureA) || triggerZoneEvent(fixtureA, fixtureB);
    }

    bool triggerZoneEvent(b2Fixture *fixtureZone, b2Fixture *fixtureOther) {
      auto event = findEvent(fixtureZone);

      if (event == INVALID_EVENT) {
        return false;
      }

      gEventManager().triggerEvent(event, nullptr);
      game::Log::info(game::Log::PHYSICS, "Map event triggered: %s (%" PRIu64 ")\n", m_eventNames[event].c_str(), event);
      return true;
    }

    game::EventType findEvent(b2Fixture *fixture) {
      auto it = m_eventZones.find(fixture);

      if (it == m_eventZones.end()) {
        return INVALID_EVENT;
      }

      return it->second;
    }

  private:
    std::map<b2Fixture*, game::EventType> m_eventZones;
    std::map<game::EventType, std::string> m_eventNames;
  };

  namespace {
    struct ZoneConstructor : public tmx::LayerVisitor {
      ZoneConstructor(PhysicsListener& listener)
      : m_listener(listener)
      {

      }

      virtual void visitObjectLayer(const tmx::Map& map, const tmx::ObjectLayer& layer) override {
        if (!layer.hasProperty("kind")) {
          game::Log::warning(game::Log::PHYSICS, "No kind for the layer: '%s'\n", layer.getName().c_str());
          return;
        }

        if (layer.getProperty("kind", "") != "zone") {
          return;
        }

        int floor = std::stoi(layer.getProperty("floor", "0"));

        game::Log::info(game::Log::PHYSICS, "Loading zone layer: '%s' (floor: %i)\n", layer.getName().c_str(), floor);

        unsigned eventCount = 0;
        unsigned collisionCount = 0;

        for (auto obj : layer) {
          const std::string& name = obj->getName();

          if (obj->getType() == "event") {
            if (!obj->hasProperty("type")) {
              game::Log::warning(game::Log::PHYSICS, "No event type for the object: '%s'\n", name.c_str());
              continue;
            }

            const std::string& type = obj->getProperty("type", "");
            m_listener.addEventZone(floor, name, obj, type);
            eventCount++;
          } else if (obj->getType() == "collision") {
            m_listener.addCollisionZone(floor, name, obj);
            collisionCount++;
          } else {
            game::Log::warning(game::Log::PHYSICS, "No type for the object: '%s'\n", name.c_str());
          }

        }

        game::Log::info(game::Log::PHYSICS, "\tObjects loaded: %u event zones and %u collision zones\n", eventCount, collisionCount);
      }
    private:
      PhysicsListener& m_listener;
    };

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

  /*
   * PhysicsModel
   */

  PhysicsModel::PhysicsModel()
  : m_world({ 0.0f, 0.0f })
  , m_listener(nullptr)
  {

  }

  PhysicsModel::~PhysicsModel() {
    delete m_listener;
  }

  void PhysicsModel::update(float dt) {
    int32 velocityIterations = 10; // 6;
    int32 positionIterations = 8; // 2;
    m_world.Step(dt, velocityIterations, positionIterations);
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

  Body PhysicsModel::createCharacterBody(float x, float y, int floor, const CollisionData *data) {
    b2BodyDef def;
    def.type = b2_kinematicBody;
    def.position = { x * BOX2D_SCALE, y * BOX2D_SCALE };
    auto body = m_world.CreateBody(&def);
    addFixtureToBody(body, floor, true, false, data);
    return Body(floor, body);
  }

  void PhysicsModel::loadMap(tmx::Map& map) {
    assert(m_listener == nullptr);

    m_listener = new PhysicsListener;

    ZoneConstructor visitor(*m_listener);
    map.visitLayers(visitor);

    m_world.SetContactListener(m_listener);
  }

}
