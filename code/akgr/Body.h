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
#ifndef AKGR_BODY_H
#define AKGR_BODY_H

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_member.hpp>

#include <Box2D/Box2D.h>

#include "Location.h"

namespace akgr {

  class Body {
  public:
    Body();
    Body(int floor, b2Body *body);

    Body(const Body&) = delete;
    Body& operator=(const Body&) = delete;

    Body(Body&&) = default;
    Body& operator=(Body&&) = default;

    Location getLocation() const;

    int getFloor() const;

    sf::Vector2f getPosition() const;
    float getAngle() const;

    void setAngleAndVelocity(float angle, float velocity);

  private:
    int m_floor;
    b2Body *m_body;

  private:
    friend class boost::serialization::access;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const {
      ar << m_floor;
      b2Vec2 pos = m_body->GetPosition();
      ar << pos.x;
      ar << pos.y;
      float32 angle = m_body->GetAngle();
      ar << angle;
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version) {
      ar >> m_floor;
      b2Vec2 pos;
      ar >> pos.x;
      ar >> pos.y;
      float32 angle;
      ar >> angle;
      m_body->SetTransform(pos, angle);
    }

    template<class Archive>
    void serialize(Archive & ar, const unsigned int file_version) {
      boost::serialization::split_member(ar, *this, file_version);
    }

  };

}

#endif // AKGR_BODY_H
