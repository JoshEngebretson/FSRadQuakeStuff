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

#ifndef TrenchBroom_Line_h
#define TrenchBroom_Line_h

#include "Utility/Vec3f.h"

namespace TrenchBroom {
    namespace Math {
        class Line {
        public:
            Vec3f point;
            Vec3f direction;

            Line() : point(Vec3f::Null), direction(Vec3f::Null) {}
            
            Line(const Vec3f& i_point, const Vec3f& i_direction) : point(i_point), direction(i_direction) {}
            
            inline const Vec3f pointAtDistance(float distance) const {
                return Vec3f(point.x + direction.x * distance,
                             point.y + direction.y * distance,
                             point.z + direction.z * distance);
            }
        };
    }
}

#endif
