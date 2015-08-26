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
#ifndef AKGR_DATA_MANAGER_H
#define AKGR_DATA_MANAGER_H

#include <map>
#include <string>

#include <boost/filesystem.hpp>

#include <tmx/Map.h>

#include "Data.h"

namespace akgr {

  class DataManager {
  public:
    void load(const boost::filesystem::path& basedir);
    void loadMap(const tmx::Map& map);

    const CollisionData *getCollisionDataFor(const std::string& name) const;

    const SpriteData *getSpriteDataFor(const std::string& name) const;

    std::tuple<const CollisionData *, const SpriteData *> getItemDataFor(const std::string& name) const;

    void addPointOfInterestData(std::string name, const Location& loc);
    const PointOfInterestData *getPointOfInterestDataFor(const std::string& name) const;

    const DialogData *getDialogDataFor(const std::string& name) const;

    const MessageData *getMessageDataFor(const std::string& name) const;

    const QuestData *getQuestDataFor(const std::string& name) const;

  private:
    std::map<std::string, CollisionData> m_collisions;
    std::map<std::string, SpriteData> m_sprites;
    std::map<std::string, ItemData> m_items;
    std::map<std::string, PointOfInterestData> m_pois;
    std::map<std::string, DialogData> m_dialogues;
    std::map<std::string, MessageData> m_messages;
    std::map<std::string, QuestData> m_quests;
  };


}

#endif // AKGR_DATA_MANAGER_H
