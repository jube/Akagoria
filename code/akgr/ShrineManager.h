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
#ifndef AKGR_SHRINE_MANAGER_H
#define AKGR_SHRINE_MANAGER_H

#include <array>
#include <vector>

#include <game/Entity.h>
#include <game/Event.h>

#include "FloorTracker.h"
#include "Location.h"

namespace akgr {

  enum class Shrine {
    MOLI,
    PONA,
    SEWI,
    TOMO,
  };

  class ShrineManager : public game::Entity {
  public:
    ShrineManager();

    void addShrineManager(const Location& loc, Shrine shrine);

    virtual void update(float dt) override;
    virtual void render(sf::RenderWindow& window) override;

  private:
    struct Particle {
      float velocity;
      float amplitude;
      float theta;
      float n;
      float e;
      bool clockwise;
    };

    struct ParticleSystem {
      Location loc;
      Shrine shrine;
      std::vector<Particle> particles;
    };

    FloorTracker m_tracker;
    std::vector<ParticleSystem> m_particlesSystems;

  private:
    game::EventStatus onUse(game::EventType type, game::Event *event);

  };

}


#endif // AKGR_SHRINE_MANAGER_H
