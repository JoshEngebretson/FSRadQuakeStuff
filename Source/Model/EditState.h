/*
 Copyright (C) 2010-2012 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TrenchBroom_EditState_h
#define TrenchBroom_EditState_h

namespace TrenchBroom {
    namespace Model {
        namespace EditState {
            typedef unsigned int Type;
            static const Type Default = 0;
            static const Type Selected = 1;
            static const Type Hidden = 2;
            static const Type Locked = 3;
            static const unsigned int Count = 4;
        }
    }
}

#endif
