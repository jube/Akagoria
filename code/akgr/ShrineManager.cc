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
#include "ShrineManager.h"

#include "GameEvents.h"
#include "MapEvents.h"
#include "Maths.h"
#include "Singletons.h"

namespace akgr {
  static constexpr std::size_t PARTICLES_COUNT = 20;
  static constexpr float MIN_RADIUS = 30.0f;

  ShrineManager::ShrineManager()
  : game::Entity(30)
  {
    gEventManager().registerHandler<UseEvent>(&ShrineManager::onUse, this);
  }

  void ShrineManager::addShrineManager(const Location& loc, Shrine shrine) {
    ParticleSystem system;
    system.loc = loc;
    system.shrine = shrine;
    system.particles.resize(PARTICLES_COUNT);

    for (std::size_t i = 0; i < PARTICLES_COUNT; ++i) {
      auto& particle = system.particles[i];
      particle.velocity = gRandom().computeUniformFloat(0.5 * PI, 1.5 * PI);
      particle.amplitude = MIN_RADIUS;
      particle.theta = gRandom().computeUniformFloat(0.0f, 2 * PI);
      particle.n = gRandom().computeUniformFloat(1.0f, 3.0f);
      particle.e = gRandom().computeUniformFloat(0.5f, 1.5f);
      particle.clockwise = (i % 2 == 0);
    }

    m_particlesSystems.emplace_back(std::move(system));
  }

  void ShrineManager::update(float dt) {
    int floor = m_tracker.getFloor();

    for (auto& system : m_particlesSystems) {
      if (system.loc.floor != floor) {
        continue;
      }

      for (std::size_t i = 0; i < PARTICLES_COUNT; ++i) {
        auto& particle = system.particles[i];

        if (particle.clockwise) {
          particle.theta += particle.velocity * dt;
        } else {
          particle.theta -= particle.velocity * dt;
        }
      }
    }
  }

  void ShrineManager::render(sf::RenderWindow& window) {
    int floor = m_tracker.getFloor();

    for (const auto& system : m_particlesSystems) {
      if (system.loc.floor != floor) {
        continue;
      }

      sf::CircleShape shape(1.5f);
      shape.setOutlineThickness(0.7f);

      switch (system.shrine) {
        case Shrine::PONA:
          shape.setFillColor(sf::Color(0xFF, 0x00, 0x00));
          shape.setOutlineColor(sf::Color(0xC0, 0x00, 0x00));
          break;
        case Shrine::TOMO:
//           shape.setFillColor(sf::Color(0x00, 0xFF, 0xFF));
//           shape.setOutlineColor(sf::Color(0x00, 0xC0, 0xC0));
          shape.setFillColor(sf::Color(0x00, 0xC0, 0xC0));
          shape.setOutlineColor(sf::Color(0x00, 0x80, 0x80));
          break;
        default:
          break;
      }

      const sf::Vector2f& center = system.loc.pos;

      for (std::size_t i = 0; i < PARTICLES_COUNT; ++i) {
        const auto& particle = system.particles[i];

        float rho = particle.amplitude * (1.0f + particle.e * std::cos(particle.n * particle.theta));
        float x = center.x + rho * std::cos(particle.theta);
        float y = center.y + rho * std::sin(particle.theta);

        shape.setPosition(x, y);
        window.draw(shape);
      }
    }
  }

  static constexpr float SHRINE_DISTANCE = 70;

  game::EventStatus ShrineManager::onUse(game::EventType type, game::Event *event) {
    auto useEvent = static_cast<UseEvent *>(event);

    for (const auto& system : m_particlesSystems) {
      if (system.loc.floor == useEvent->loc.floor) {
        float d2 = squareDistance(system.loc.pos, useEvent->loc.pos);
//         game::Log::info(game::Log::GENERAL, "Distance: %f\n", std::sqrt(d2));

        if (d2 < SHRINE_DISTANCE * SHRINE_DISTANCE) {
          switch (system.shrine) {
            case Shrine::PONA:
              gHeroAttributes().increaseHP(0.1f);
              break;
            case Shrine::TOMO:
              useEvent->kind = UseEvent::SAVE;
              break;
            default:
              break;
          }
        }
      }
    }

    return game::EventStatus::KEEP;
  }

}
