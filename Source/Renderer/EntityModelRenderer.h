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

#ifndef TrenchBroom_EntityModelRenderer_h
#define TrenchBroom_EntityModelRenderer_h

#include "Utility/VecMath.h"

using namespace TrenchBroom::Math;

namespace TrenchBroom {
    namespace Model {
        class Entity;
    }
    
    namespace Renderer {
        class ShaderProgram;
        class Transformation;
        
        class EntityModelRenderer {
        public:
            virtual ~EntityModelRenderer() {};
            virtual void render(ShaderProgram& shaderProgram, Transformation& transformation, const Model::Entity& entity);
            virtual void render(ShaderProgram& shaderProgram, Transformation& transformation, const Vec3f& position, const Quat& rotation);
            virtual void render(ShaderProgram& shaderProgram) = 0;
            virtual const Vec3f& center() const = 0;
            virtual const BBox& bounds() const = 0;
            virtual BBox boundsAfterTransformation(const Mat4f& transformation) const = 0;
        };
    }
}

#endif
