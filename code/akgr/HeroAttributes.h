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
#ifndef AKGR_HERO_ATTRIBUTES_H
#define AKGR_HERO_ATTRIBUTES_H

#include <boost/serialization/serialization.hpp>

#include <game/Entity.h>

namespace akgr {

  class HeroAttributes : public game::Entity {
  public:
    HeroAttributes();

    void setMaxHP(int healthPoints) {
      m_healthPointsMax = healthPoints;
    }

    void setMaxMP(int magicPoints) {
      m_magicPointsMax = magicPoints;
    }

    virtual void update(float dt) override;
    virtual void render(sf::RenderWindow& window) override;

  private:
    sf::Font *m_font;
    
    int m_healthPoints;
    int m_healthPointsMax;
    int m_magicPoints;
    int m_magicPointsMax;

  private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int file_version) {
      ar & m_healthPoints;
      ar & m_healthPointsMax;
      ar & m_magicPoints;
      ar & m_magicPointsMax;
    }

  };

}

#endif // AKGR_HERO_ATTRIBUTES_H
