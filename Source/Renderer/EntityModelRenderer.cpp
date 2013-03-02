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

#include "EntityModelRenderer.h"

#include <GL/glew.h>
#include "Model/Entity.h"
#include "Renderer/ApplyMatrix.h"
#include "Renderer/Transformation.h"
#include "Renderer/Shader/Shader.h"
#include "Utility/VecMath.h"

using namespace TrenchBroom::Math;

namespace TrenchBroom {
    namespace Renderer {
        void EntityModelRenderer::render(ShaderProgram& shaderProgram, Transformation& transformation, const Model::Entity& entity) {
            render(shaderProgram, transformation, entity.origin(), entity.rotation());
        }

        void EntityModelRenderer::render(ShaderProgram& shaderProgram, Transformation& transformation, const Vec3f& position, const Quat& rotation) {
            Mat4f matrix;
            matrix.translate(position);
            matrix.rotate(rotation);

            ApplyMatrix applyMatrux(transformation, matrix);
            render(shaderProgram);
        }
    }
}
