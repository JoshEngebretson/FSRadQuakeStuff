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

#ifndef __TrenchBroom__TextureBrowserCanvas__
#define __TrenchBroom__TextureBrowserCanvas__

#include "Model/TextureManager.h"
#include "Renderer/Text/StringManager.h"
#include "View/CellLayoutGLCanvas.h"

namespace TrenchBroom {
    namespace Model {
        class Texture;
        class TextureCollection;
        class TextureManager;
    }
    
    namespace Renderer {
        namespace Text {
            class FontDescriptor;
        }
        
        class Shader;
        class ShaderProgram;
        class TextureRenderer;
        class Vbo;
    }
    
    namespace Utility {
        class Console;
    }
    
    namespace View {
        class DocumentViewHolder;
        
        class TextureGroupData {
        public:
            Model::TextureCollection* textureCollection;
            Renderer::Text::StringRendererPtr stringRenderer;
            
            TextureGroupData(Model::TextureCollection* i_textureCollection, Renderer::Text::StringRendererPtr i_stringRenderer) :
            textureCollection(i_textureCollection),
            stringRenderer(i_stringRenderer) {}
            
            TextureGroupData() :
            textureCollection(NULL),
            stringRenderer(NULL) {}
        };
        
        class TextureCellData {
        public:
            Model::Texture* texture;
            Renderer::TextureRenderer* textureRenderer;
            Renderer::Text::StringRendererPtr stringRenderer;
            
            TextureCellData(Model::Texture* i_texture, Renderer::TextureRenderer* i_textureRenderer, Renderer::Text::StringRendererPtr i_stringRenderer) :
            texture(i_texture),
            textureRenderer(i_textureRenderer),
            stringRenderer(i_stringRenderer) {}
        };
        
        class TextureBrowserCanvas : public CellLayoutGLCanvas<TextureCellData, TextureGroupData> {
        protected:
            DocumentViewHolder& m_documentViewHolder;
            Model::Texture* m_selectedTexture;
            
            typedef std::map<Model::Texture*, Renderer::Text::StringRendererPtr> StringRendererCache;
            typedef std::pair<Model::Texture*, Renderer::Text::StringRendererPtr> StringRendererCacheEntry;
            StringRendererCache m_stringRendererCache;
            
            bool m_group;
            bool m_hideUnused;
            Model::TextureSortOrder::Type m_sortOrder;
            String m_filterText;
            
            void addTextureToLayout(Layout& layout, Model::Texture* texture, const Renderer::Text::FontDescriptor& font);
            virtual void doInitLayout(Layout& layout);
            virtual void doReloadLayout(Layout& layout);
            virtual void doClear();
            virtual void doRender(Layout& layout, float y, float height);
            virtual void handleLeftClick(Layout& layout, float x, float y);
        public:
            TextureBrowserCanvas(wxWindow* parent, wxWindowID windowId, wxScrollBar* scrollBar, DocumentViewHolder& documentViewHolder);
            ~TextureBrowserCanvas();
            
            inline void setSortOrder(Model::TextureSortOrder::Type sortOrder) {
                if (sortOrder == m_sortOrder)
                    return;
                m_sortOrder = sortOrder;
                reload();
                Refresh();
            }
        
            inline void setGroup(bool group) {
                if (group == m_group)
                    return;
                m_group = group;
                reload();
                Refresh();
            }
        
            inline void setHideUnused(bool hideUnused) {
                if (hideUnused == m_hideUnused)
                    return;
                m_hideUnused = hideUnused;
                reload();
                Refresh();
            }
        
            inline void setFilterText(const String& filterText) {
                if (filterText == m_filterText)
                    return;
                m_filterText = filterText;
                reload();
                Refresh();
            }
            
            inline Model::Texture* selectedTexture() const {
                return m_selectedTexture;
            }
            
            inline void setSelectedTexture(Model::Texture* texture) {
                if (texture == m_selectedTexture)
                    return;
                m_selectedTexture = texture;
                Refresh();
            }
        };
    }
}

#endif /* defined(__TrenchBroom__TextureBrowserCanvas__) */
