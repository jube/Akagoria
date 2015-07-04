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
#include "Story.h"

#include <cassert>

#include <game/Log.h>

#include "GameEvents.h"
#include "Singletons.h"

namespace akgr {

  Story::Story()
  {
    gEventManager().registerHandler("IntroDialogEvent"_type, &Story::onIntroDialog, this);
    gEventManager().registerHandler<DialogEndEvent>(&Story::onDialogEnd, this);
  }

  game::EventStatus Story::onIntroDialog(game::EventType type, game::Event *event) {
    gDialogManager().start("Intro");
    return game::EventStatus::DIE;
  }

  game::EventStatus Story::onDialogEnd(game::EventType type, game::Event *event) {
    auto id = game::Hash(static_cast<DialogEndEvent*>(event)->name);
    Character *character = nullptr;

    switch (id) {
      case "ShagirConversation0"_id:
        character = gCharacterManager().getCharacter("Shagir");
        assert(character);
        character->attachDialog("ShagirConversation1");
        break;
      case "ShagirConversation1"_id:
        character = gCharacterManager().getCharacter("Shagir");
        assert(character);
        character->detachDialog();
        break;
      default:
        break;
    }

    return game::EventStatus::KEEP;
  }

}
