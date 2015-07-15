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
#ifndef AKGR_HERO_H
#define AKGR_HERO_H

#include <boost/serialization/serialization.hpp>

#include <game/Animation.h>
#include <game/Event.h>
#include <game/Entity.h>

#include "Body.h"
#include "Location.h"

namespace akgr {

  class Hero : public game::Entity {
  public:
    Hero(const Location& loc = { { 100.0f, 100.0f }, 0 });

    sf::Vector2f getPosition() const {
      return m_body.getPosition();
    }

    Location getLocation() const {
      return m_body.getLocation();
    }

    void stopWalking() {
      m_linear = Linear::STOP;
    }

    void walkForward() {
      m_linear = Linear::FORWARD;
    }

    void walkBackward() {
      m_linear = Linear::BACKWARD;
    }

    void stopTurning() {
      m_angular = Angular::STOP;
    }

    void turnLeft() {
      m_angular = Angular::LEFT;
    }

    void turnRight() {
      m_angular = Angular::RIGHT;
    }

    void broadcastLocation();

    virtual void update(float dt) override;
    virtual void render(sf::RenderWindow& window) override;

  private:
    Body m_body;

    enum class Linear {
      STOP,
      FORWARD,
      BACKWARD,
    };

    Linear m_linear;

    enum class Angular {
      STOP,
      LEFT,
      RIGHT,
    };

    Angular m_angular;

    game::Animation m_staticAnimation;
    game::Animation m_forwardAnimation;
    game::Animation m_backwardAnimation;
    game::Animation *m_currentAnimation;

  private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int file_version) {
      ar & m_body;
    }
  };


}


#endif // AKGR_HERO_H
