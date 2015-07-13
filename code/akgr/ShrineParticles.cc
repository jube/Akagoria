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
#include "ShrineParticles.h"

#include "Singletons.h"

namespace akgr {
  static constexpr std::size_t PARTICLES_COUNT = 20;
  static constexpr float MIN_RADIUS = 30.0f;
  static constexpr float PI = 3.1415926f;

  ShrineParticles::ShrineParticles()
  : game::Entity(30)
  {

  }

  void ShrineParticles::addShrineParticles(const sf::Vector2f& center, ShrineKind shrine) {
    ParticleSystem system;
    system.center = center;
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

    m_particles_systems.emplace_back(std::move(system));
  }

  void ShrineParticles::update(float dt) {
    for (auto& system : m_particles_systems) {
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

  void ShrineParticles::render(sf::RenderWindow& window) {
    for (const auto& system : m_particles_systems) {
      sf::CircleShape shape(1.5f);
      shape.setOutlineThickness(0.5f);

      switch (system.shrine) {
        case ShrineKind::TOMO:
          shape.setFillColor(sf::Color(0x00, 0xFF, 0xFF));
          shape.setOutlineColor(sf::Color(0x00, 0xC0, 0xC0));
          break;
        default:
          break;
      }

      for (std::size_t i = 0; i < PARTICLES_COUNT; ++i) {
        const auto& particle = system.particles[i];

        float rho = particle.amplitude * (1.0f + particle.e * std::cos(particle.n * particle.theta));
        float x = system.center.x + rho * std::cos(particle.theta);
        float y = system.center.y + rho * std::sin(particle.theta);

        shape.setPosition(x, y);
        window.draw(shape);
      }
    }
  }

}
