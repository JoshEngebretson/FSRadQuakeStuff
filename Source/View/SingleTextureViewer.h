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

#ifndef __TrenchBroom__SingleTextureViewer__
#define __TrenchBroom__SingleTextureViewer__

#include <GL/glew.h>
#include <wx/glcanvas.h>

class wxGLContext;

namespace TrenchBroom {
    namespace Model {
        class Texture;
    }
    
    namespace Renderer {
        class SharedResources;
        class TextureRendererManager;
    }
    
    namespace View {
        class SingleTextureViewer : public wxGLCanvas {
        private:
            Renderer::TextureRendererManager& m_textureRendererManager;
            wxGLContext* m_glContext;
            Model::Texture* m_texture;
        public:
            SingleTextureViewer(wxWindow* parent, Renderer::SharedResources& sharedResources);
            ~SingleTextureViewer();
 
            void setTexture(Model::Texture* texture);
            
            void OnPaint(wxPaintEvent& event);

            DECLARE_EVENT_TABLE()
        };
    }
}

#endif /* defined(__TrenchBroom__SingleTextureViewer__) */
