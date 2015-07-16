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
#include "Singletons.h"

namespace akgr {

  game::Singleton<game::Random> gRandom;
  game::Singleton<game::ResourceManager> gResourceManager;
  game::Singleton<game::EventManager> gEventManager;
  game::Singleton<game::EntityManager> gMainEntityManager;
  game::Singleton<game::EntityManager> gHeadsUpEntityManager;

  game::Singleton<DataManager> gDataManager;

  game::Singleton<PhysicsModel> gPhysicsModel;

  game::Singleton<CharacterManager> gCharacterManager;
  game::Singleton<Hero> gHero;
  game::Singleton<HeroAttributes> gHeroAttributes;
  game::Singleton<DialogManager> gDialogManager;
  game::Singleton<RequirementManager> gRequirementManager;
  game::Singleton<SavePointManager> gSavePointManager;
  game::Singleton<ShrineParticles> gShrineParticles;

  game::Singleton<game::WindowGeometry> gWindowGeometry;

}
