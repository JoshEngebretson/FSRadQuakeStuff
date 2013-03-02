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

#ifndef __TrenchBroom__Command__
#define __TrenchBroom__Command__

#include "Model/BrushTypes.h"
#include "Model/EntityTypes.h"
#include "Model/MapDocument.h"

#include <wx/cmdproc.h>

namespace TrenchBroom {
    namespace Controller {
        class Command : public wxCommand {
        public:
            typedef enum {
                LoadMap,
                ClearMap,
                ChangeGrid,
                ChangeEditState,
                SetMod,
                SetEntityDefinitionFile,
                InvalidateRendererEntityState,
                InvalidateRendererBrushState,
                InvalidateRendererState,
                InvalidateEntityModelRendererCache,
                InvalidateInstancedRenderers,
                SetFaceAttributes,
                MoveTextures,
                RotateTextures,
                AddTextureCollection,
                RemoveTextureCollection,
                MoveTextureCollectionUp,
                MoveTextureCollectionDown,
                SetEntityPropertyValue,
                SetEntityPropertyKey,
                RemoveEntityProperty,
                AddObjects,
                MoveObjects,
                RotateObjects,
                FlipObjects,
                RemoveObjects,
                MoveVertices,
                SnapVertices,
                ResizeBrushes,
                ReparentBrushes,
                UpdateFigures
            } Type;
            
            typedef enum {
                None,
                Doing,
                Done,
                Undoing,
                Undone
            } State;
        private:
            Type m_type;
            State m_state;
        protected:
            virtual bool performDo() { return true; }
            virtual bool performUndo() { return true; }
            virtual void updateViews() {}
        public:
            static wxString makeObjectActionName(const wxString& action, const Model::EntityList& entities, const Model::BrushList& brushes) {
                assert(!entities.empty() || !brushes.empty());
                
                if (entities.empty())
                    return action + (brushes.size() == 1 ? wxT(" Brush") : wxT(" Brushes"));
                if (brushes.empty())
                    return action + (entities.size() == 1 ? wxT(" Entity") : wxT(" Entities"));
                return action + wxT(" Objects");
            }
            
            Command(Type type) :
            wxCommand(false, ""),
            m_type(type),
            m_state(None) {}

            Command(Type type, bool undoable, const wxString& name) :
            wxCommand(undoable, name),
            m_type(type),
            m_state(None) {}
            
            inline Type type() const {
                return m_type;
            }
            
            inline State state() const {
                return m_state;
            }
            
            bool Do() {
                State previous = m_state;
                m_state = Doing;
                bool result = performDo();
                if (result) {
                    updateViews();
                    m_state = Done;
                } else {
                    m_state = previous;
                }
                return result;
            }
            
            bool Undo() {
                State previous = m_state;
                m_state = Undoing;
                bool result = performUndo();
                if (result) {
                    updateViews();
                    m_state = Undone;
                } else {
                    m_state = previous;
                }
                return result;
            }
        };
        
        class DocumentCommand : public Command {
            Model::MapDocument& m_document;
            bool m_modifiesDocument;
        protected:
            inline Model::MapDocument& document() const {
                return m_document;
            }
            
            virtual void updateViews() {
                m_document.UpdateAllViews(NULL, this);
            }
        public:
            DocumentCommand(Type type, Model::MapDocument& document, bool undoable, const wxString& name, bool modifiesDocument) :
            Command(type, undoable, name),
            m_document(document),
            m_modifiesDocument(modifiesDocument) {}
            
            bool Do() {
                bool result = Command::Do();
                if (result && m_modifiesDocument)
                    document().incModificationCount();
                return result;
            }
            
            bool Undo() {
                bool result = Command::Undo();
                if (result && m_modifiesDocument)
                    document().decModificationCount();
                return result;
            }
        };
    }
}

#endif /* defined(__TrenchBroom__Command__) */
