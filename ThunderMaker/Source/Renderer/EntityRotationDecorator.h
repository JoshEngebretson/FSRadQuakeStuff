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

#ifndef __TrenchBroom__EntityRotationDecorator__
#define __TrenchBroom__EntityRotationDecorator__

#include "Renderer/EntityDecorator.h"

#include "Utility/Color.h"

namespace TrenchBroom {
    namespace Model {
        class Map;
    }
    
    namespace Renderer {
        class VertexArray;
        
        class EntityRotationDecorator : public EntityDecorator {
        private:
            Color m_color;
            VertexArray* m_vertexArray;
            bool m_valid;
        public:
            EntityRotationDecorator(const Model::Map& map, const Color& color);
            
            inline void invalidate() {
                m_valid = false;
            }
            
            void render(Vbo& vbo, RenderContext& context);
        };
    }
}

#endif /* defined(__TrenchBroom__EntityRotationDecorator__) */
