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

#ifndef __TrenchBroom__AxisFigure__
#define __TrenchBroom__AxisFigure__

#include "Renderer/Figure.h"
#include "Utility/Color.h"
#include "Utility/VecMath.h"

using namespace TrenchBroom::Math;

namespace TrenchBroom {
    namespace Renderer {
        class VertexArray;
        
        class AxisFigure : public Figure {
        protected:
            float m_axisLength;
            bool m_axes[3];
            Color m_xColor;
            Color m_yColor;
            Color m_zColor;
            bool m_valid;
            VertexArray* m_vertexArray;
        public:
            AxisFigure(float axisLength);
            ~AxisFigure();
            
            inline void setAxes(bool xAxis, bool yAxis, bool zAxis) {
                m_axes[0] = xAxis;
                m_axes[1] = yAxis;
                m_axes[2] = zAxis;
                m_valid = false;
            }
            
            inline void setXColor(const Color& color) {
                m_xColor = color;
                m_valid = false;
            }
            
            inline void setYColor(const Color& color) {
                m_yColor = color;
                m_valid = false;
            }
            
            inline void setZColor(const Color& color) {
                m_zColor = color;
                m_valid = false;
            }

            void render(Vbo& vbo, RenderContext& context);
        };
    }
}

#endif /* defined(__TrenchBroom__AxisFigure__) */
