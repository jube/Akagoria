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
#ifndef AKGR_UI_H
#define AKGR_UI_H

#include <game/Entity.h>

#include "Data.h"

namespace akgr {

  class DialogUI : public game::Entity {
  public:
    DialogUI();

    void setDialogLine(const DialogData::Line& line);

    virtual void render(sf::RenderWindow& window) override;

  private:
    sf::Font *m_font;
    const DialogData::Line *m_currentLine;
  };

}

#endif // AKGR_UI_H