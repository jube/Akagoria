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
#ifndef AKGR_MATHS_H
#define AKGR_MATHS_H

namespace akgr {

  constexpr float PI = 3.141592653589793f;
  constexpr float PI_2 = 1.57079632679489661923f;

  template<typename T>
  constexpr T square(T val) {
    return val * val;
  }

  constexpr float squareDistance(const sf::Vector2f& lhs, const sf::Vector2f& rhs) {
    return square(lhs.x - rhs.x) + square(lhs.y - rhs.y);
  }


}

#endif // AKGR_MATHS_H
