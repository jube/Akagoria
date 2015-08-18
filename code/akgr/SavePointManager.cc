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
#include "SavePointManager.h"

#include <cassert>
#include <cstdlib>
#include <array>
#include <string>
#include <fstream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <game/Log.h>

#include "Character.h"
#include "Hero.h"
#include "HeroAttributes.h"
#include "RequirementManager.h"
#include "Singletons.h"

namespace akgr {

  static boost::filesystem::path getRootPath(const char *env, const char *default_dir) {
    const char *value = std::getenv(env);

    if (value) {
      return boost::filesystem::path(value);
    }

    value = std::getenv("HOME");
    assert(value);

    boost::filesystem::path root(value);
    return root / default_dir;
  }

  // http://www.freedesktop.org/wiki/Specifications/basedir-spec/

  static boost::filesystem::path getUserDataPath() {
    boost::filesystem::path root = getRootPath("XDG_DATA_HOME", ".local/share");
    return root / "akagoria";
  }

//   static boost::filesystem::path getUserConfigPath() {
//     boost::filesystem::path root = getRootPath("XDG_CONFIG_HOME", ".config");
//     return root / "akagoria";
//   }


  SavePointManager::SavePointManager()
  : m_saveDirectory(getUserDataPath())
  {
    if (!boost::filesystem::exists(m_saveDirectory)) {
      game::Log::info(game::Log::GENERAL, "Creating save directory: '%s'\n", m_saveDirectory.string().c_str());
      auto created = boost::filesystem::create_directories(m_saveDirectory);
      assert(created);
    }
  }

  static constexpr int SLOT_MIN = 0;
  static constexpr int SLOT_MAX = 2;

  bool SavePointManager::hasSlot(int slot) const {
    if (slot < SLOT_MIN || slot > SLOT_MAX) {
      return false;
    }

    auto path = getSlotFilename(slot);
    return boost::filesystem::exists(path);
  }

  void SavePointManager::loadFromSlot(int slot) {
    if (slot < SLOT_MIN || slot > SLOT_MAX) {
      game::Log::warning(game::Log::GENERAL, "Wrong saving slot: %i\n", slot);
      return;
    }

    auto path = getSlotFilename(slot);

    if (!boost::filesystem::exists(path)) {
      game::Log::warning(game::Log::GENERAL, "Slot does not exist: %i\n", slot);
      return;
    }

    std::ifstream file(path.string());

    boost::archive::text_iarchive archive(file);
    archive >> gHero();
    archive >> gHeroAttributes();
    archive >> gRequirementManager();
    archive >> gCharacterManager();
  }

  void SavePointManager::saveToSlot(int slot) {
    if (slot < SLOT_MIN || slot > SLOT_MAX) {
      game::Log::warning(game::Log::GENERAL, "Wrong saving slot: %i\n", slot);
      return;
    }

    auto path = getSlotFilename(slot);

    std::ofstream file(path.string());

    boost::archive::text_oarchive archive(file);
    archive << gHero();
    archive << gHeroAttributes();
    archive << gRequirementManager();
    archive << gCharacterManager();
  }

  static constexpr std::size_t TIME_INFO_SIZE = 1024;

  std::string SavePointManager::getSlotInfo(int slot) const {
    if (slot < SLOT_MIN || slot > SLOT_MAX) {
      game::Log::warning(game::Log::GENERAL, "Wrong saving slot: %i\n", slot);
      return "(forbidden slot)\n-\n-";
    }

    auto path = getSlotFilename(slot);

    if (!boost::filesystem::exists(path)) {
      return "(empty)\n-\n-";
    }

    std::string info = "slot#" + std::to_string(slot) + '\n';

    // TODO: add region name
    info += "<region>\n";

    auto time = boost::filesystem::last_write_time(path);
    std::array<char, TIME_INFO_SIZE> timeInfo;
    std::strftime(timeInfo.data(), timeInfo.size(), "%F %T", std::localtime(&time));
    info += timeInfo.data();

    return info;
  }

  boost::filesystem::path SavePointManager::getSlotFilename(int slot) const {
    std::string filename = "slot" + std::to_string(slot) + ".akgr";
    boost::filesystem::path path = m_saveDirectory / filename;
    return path;
  }
}
