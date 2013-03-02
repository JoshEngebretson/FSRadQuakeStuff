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

#ifndef __TrenchBroom__MapDocument__
#define __TrenchBroom__MapDocument__

#include "Model/BrushTypes.h"
#include "Model/EntityTypes.h"
#include "Utility/String.h"

#include <wx/docview.h>
#include <wx/timer.h>

namespace TrenchBroom {
    namespace Controller {
        class Autosaver;
    }
    
    namespace Renderer {
        class SharedResources;
    }
    
    namespace Utility {
        class Console;
        class Grid;
        class ProgressIndicator;
    }
    
    namespace Model {
        class Brush;
        class EditStateManager;
        class Entity;
        class EntityDefinitionManager;
        class Face;
        class Map;
        class Octree;
        class Palette;
        class PointFile;
        class Picker;
        class Texture;
        class TextureManager;
        
        class MapDocument : public wxDocument {
            DECLARE_DYNAMIC_CLASS(MapDocument)
        protected:
            Controller::Autosaver* m_autosaver;
            wxTimer* m_autosaveTimer;
            Utility::Console* m_console;
            Renderer::SharedResources* m_sharedResources;
            Map* m_map;
            EditStateManager* m_editStateManager;
            Octree* m_octree;
            Picker* m_picker;
            TextureManager* m_textureManager;
            EntityDefinitionManager* m_definitionManager;
            Utility::Grid* m_grid;
            Model::Texture* m_mruTexture;
            String m_mruTextureName;
            bool m_textureLock;
            int m_modificationCount;
            
            mutable StringList m_searchPaths;
            mutable bool m_searchPathsValid;
            
            PointFile* m_pointFile;
            
            virtual bool DoOpenDocument(const wxString& file);
            virtual bool DoSaveDocument(const wxString& file);
            
            void clear();

            void loadPalette();
            void loadMap(std::istream& stream, Utility::ProgressIndicator& progressIndicator);
            void loadTextures(Utility::ProgressIndicator& progressIndicator);
        public:
            MapDocument();
            virtual ~MapDocument();

            std::istream& LoadObject(std::istream& stream);
            std::ostream& SaveObject(std::ostream& stream);
            
            void Modify(bool modify);
            
            Entity* worldspawn(bool create);
            void addEntity(Entity& entity);
            void removeEntity(Entity& entity);
            void entityWillChange(Entity& entity);
            void entityDidChange(Entity& entity);
            void entitiesWillChange(const EntityList& entities);
            void entitiesDidChange(const EntityList& entities);
            void addBrush(Entity& entity, Brush& brush);
            void removeBrush(Brush& brush);
            void brushWillChange(Brush& brush);
            void brushDidChange(Brush& brush);
            void brushesWillChange(const BrushList& brushes);
            void brushesDidChange(const BrushList& brushes);
            
            Utility::Console& console() const;
            Renderer::SharedResources& sharedResources() const;
            Map& map() const;
            EntityDefinitionManager& definitionManager() const;
            EditStateManager& editStateManager() const;
            TextureManager& textureManager() const;
            Picker& picker() const;
            Utility::Grid& grid() const;
            
            const StringList& searchPaths() const;
            void invalidateSearchPaths();
            
            bool pointFileExists();
            void loadPointFile();
            void unloadPointFile();
            bool pointFileLoaded();
            PointFile& pointFile();
            
            Model::Texture* mruTexture() const;
            void setMruTexture(Model::Texture* texture);
            
            bool textureLock() const;
            void setTextureLock(bool textureLock);

            void updateAfterTextureManagerChanged();
            void loadTextureWad(const String& path);
            void loadTextureWad(const String& path, size_t index);

            void setEntityDefinitionFile(const String& definitionFile);
            
            void incModificationCount();
            void decModificationCount();
            
            bool OnCreate(const wxString& path, long flags);
			bool OnNewDocument();
            bool OnOpenDocument(const wxString& path);
            void OnAutosaveTimer(wxTimerEvent& event);

            DECLARE_EVENT_TABLE();
        };
    }
}

#endif /* defined(__TrenchBroom__MapDocument__) */
