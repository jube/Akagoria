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
#include "Hero.h"

#include <cassert>

#include <game/Log.h>

#include "Data.h"
#include "GameEvents.h"
#include "Singletons.h"

namespace akgr {

  Hero::Hero(float x, float y, int floor)
  : game::Entity(1)
  , m_linear(Linear::STOP), m_angular(Angular::STOP)
  , m_mode(Mode::WALK)
  , m_staticAnimation("static"), m_forwardAnimation("forward"), m_backwardAnimation("backward")
  , m_currentAnimation(&m_staticAnimation) {
    gEventManager().registerHandler<DialogEndEvent>(&Hero::onDialogEnd, this);

    CollisionData data;
    data.shape = CollisionShape::RECTANGLE;
    data.rectangle.width = 60;
    data.rectangle.height = 55;
    m_body = gPhysicsModel().createHeroBody(x, y, floor, &data);

    auto texture = gResourceManager().getTexture("sprites/kalista.png");
    assert(texture);
    texture->setSmooth(true);

    m_staticAnimation.addFrame(texture, { 0, 0, 64, 64 }, 1.0f);

    m_forwardAnimation.addFrame(texture, {   0, 0, 64, 64 }, 0.15f);
    m_forwardAnimation.addFrame(texture, {  64, 0, 64, 64 }, 0.20f);
    m_forwardAnimation.addFrame(texture, {   0, 0, 64, 64 }, 0.15f);
    m_forwardAnimation.addFrame(texture, { 128, 0, 64, 64 }, 0.20f);

    m_backwardAnimation.addFrame(texture, {   0, 0, 64, 64 }, 0.20f);
    m_backwardAnimation.addFrame(texture, { 128, 0, 64, 64 }, 0.30f);
    m_backwardAnimation.addFrame(texture, {   0, 0, 64, 64 }, 0.20f);
    m_backwardAnimation.addFrame(texture, {  64, 0, 64, 64 }, 0.30f);
  }

  void Hero::tryToTalk() {
    if (m_mode == Mode::WALK) {
      TalkEvent event;
      event.loc = m_body.getLocation();
      event.isTalking = false;
      gEventManager().triggerEvent(&event);

      if (event.isTalking) {
        m_mode = Mode::TALK;
      }
    }
  }

  void Hero::broadcastLocation() {
    HeroLocationEvent event;
    event.loc = m_body.getLocation();
    gEventManager().triggerEvent(&event);
  }

  static constexpr float HOP = 150.0f;
  static constexpr float TURN = 3.0f;

  void Hero::update(float dt) {
    float angle = m_body.getAngle();
    float velocity = 0.0f;

    switch (m_angular) {
      case Angular::STOP:
        break;

      case Angular::LEFT:
        angle -= dt * TURN;
        break;

      case Angular::RIGHT:
        angle += dt * TURN;
        break;
    }

    if (m_mode == Mode::WALK) {
      switch (m_linear) {
        case Linear::STOP:
          m_currentAnimation = &m_staticAnimation;
          break;

        case Linear::FORWARD:
          m_currentAnimation = &m_forwardAnimation;
          velocity = -HOP;
          break;

        case Linear::BACKWARD:
          m_currentAnimation = &m_backwardAnimation;
          velocity = HOP * 0.5f;
          break;
      }

    }

    m_body.setAngleAndVelocity(angle, velocity);

    assert(m_currentAnimation);
    m_currentAnimation->update(dt);

    broadcastLocation();
  }

  static constexpr float PI_2 = 1.57079632679489661923f;

  void Hero::render(sf::RenderWindow& window) {
    m_currentAnimation->renderAt(window, getPosition(), m_body.getAngle() / PI_2 * 90.0f);
  }

  game::EventStatus Hero::onDialogEnd(game::EventType type, game::Event *event) {
    m_mode = Mode::WALK;
    return game::EventStatus::KEEP;
  }

}
