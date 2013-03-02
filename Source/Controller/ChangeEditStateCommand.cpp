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

#include "ChangeEditStateCommand.h"

#include "Model/Face.h"
#include "Model/MapDocument.h"

#include <cassert>

namespace TrenchBroom {
    namespace Controller {
        ChangeEditStateCommand::ChangeEditStateCommand(Model::MapDocument& document, const wxString& name, Model::EditState::Type previousState) :
        DocumentCommand(Command::ChangeEditState, document, true, name, false),
        m_state(previousState),
        m_affectAll(true),
        m_replace(false),
        m_mruTexture(NULL) {}

        ChangeEditStateCommand::ChangeEditStateCommand(Model::MapDocument& document, const wxString& name, Model::EditState::Type newState, const Model::EntityList& entities, const Model::BrushList& brushes, bool replace) :
        DocumentCommand(Command::ChangeEditState, document, true, name, false),
        m_state(newState),
        m_affectAll(false),
        m_replace(replace),
        m_entities(entities),
        m_brushes(brushes),
        m_mruTexture(NULL) {}
        
        ChangeEditStateCommand::ChangeEditStateCommand(Model::MapDocument& document, const wxString& name, Model::EditState::Type newState, const Model::FaceList& faces, bool replace) :
        DocumentCommand(Command::ChangeEditState, document, true, name, false),
        m_state(newState),
        m_affectAll(false),
        m_replace(replace),
        m_faces(faces),
        m_mruTexture(NULL) {
            assert(m_state == Model::EditState::Selected || m_state == Model::EditState::Default);
        }
        
        bool ChangeEditStateCommand::performDo() {
            if (m_affectAll) {
                if (m_state == Model::EditState::Selected)
                    m_changeSet = document().editStateManager().deselectAll();
                else if (m_state == Model::EditState::Hidden)
                    m_changeSet = document().editStateManager().unhideAll();
                else if (m_state == Model::EditState::Locked)
                    m_changeSet = document().editStateManager().unlockAll();
            } else {
                if (!m_faces.empty()) {
                    m_changeSet = document().editStateManager().setSelected(m_faces, m_state == Model::EditState::Selected, m_replace);
                    m_mruTexture = document().mruTexture();
                    document().setMruTexture(m_faces.back()->texture());
                } else {
                    if (!m_entities.empty()) {
                        if (!m_brushes.empty())
                            m_changeSet = document().editStateManager().setEditState(m_entities, m_brushes, m_state, m_replace);
                        else
                            m_changeSet = document().editStateManager().setEditState(m_entities, m_state, m_replace);
                    } else if (!m_brushes.empty()) {
                        m_changeSet = document().editStateManager().setEditState(m_brushes, m_state, m_replace);
                    }
                }
            }
            
            return true;
        }
        
        bool ChangeEditStateCommand::performUndo() {
            m_changeSet = document().editStateManager().undoChangeSet(m_changeSet);
            document().setMruTexture(m_mruTexture);
            return true;
        }

        ChangeEditStateCommand* ChangeEditStateCommand::select(Model::MapDocument& document, Model::Entity& entity) {
            Model::EntityList entities;
            entities.push_back(&entity);
            return select(document, entities);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::select(Model::MapDocument& document, Model::Brush& brush) {
            Model::BrushList brushes;
            brushes.push_back(&brush);
            return select(document, brushes);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::select(Model::MapDocument& document, Model::Face& face) {
            Model::FaceList faces;
            faces.push_back(&face);
            return select(document, faces);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::select(Model::MapDocument& document, const Model::EntityList& entities) {
            return select(document, entities, Model::EmptyBrushList);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::select(Model::MapDocument& document, const Model::BrushList& brushes) {
            return select(document, Model::EmptyEntityList, brushes);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::select(Model::MapDocument& document, const Model::EntityList& entities, const Model::BrushList& brushes) {
            return new ChangeEditStateCommand(document, makeObjectActionName(wxT("Select"), entities, brushes), Model::EditState::Selected, entities, brushes, false);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::select(Model::MapDocument& document, const Model::FaceList& faces) {
            return new ChangeEditStateCommand(document, faces.size() == 1 ? wxT("Select Face") : wxT("Select Faces"), Model::EditState::Selected, faces, false);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::deselect(Model::MapDocument& document, Model::Entity& entity) {
            Model::EntityList entities;
            entities.push_back(&entity);
            return deselect(document, entities);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::deselect(Model::MapDocument& document, Model::Brush& brush) {
            Model::BrushList brushes;
            brushes.push_back(&brush);
            return deselect(document, brushes);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::deselect(Model::MapDocument& document, Model::Face& face) {
            Model::FaceList faces;
            faces.push_back(&face);
            return deselect(document, faces);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::deselect(Model::MapDocument& document, const Model::EntityList& entities) {
            return deselect(document, entities, Model::EmptyBrushList);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::deselect(Model::MapDocument& document, const Model::BrushList& brushes) {
            return deselect(document, Model::EmptyEntityList, brushes);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::deselect(Model::MapDocument& document, const Model::EntityList& entities, const Model::BrushList& brushes) {
            return new ChangeEditStateCommand(document, makeObjectActionName(wxT("Deselect"), entities, brushes), Model::EditState::Default, entities, brushes, false);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::deselect(Model::MapDocument& document, const Model::FaceList& faces) {
            return new ChangeEditStateCommand(document, faces.size() == 1 ? wxT("Deselect Face") : wxT("Deselect Faces"), Model::EditState::Default, faces, false);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::replace(Model::MapDocument& document, Model::Entity& entity) {
            Model::EntityList entities;
            entities.push_back(&entity);
            return replace(document, entities);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::replace(Model::MapDocument& document, Model::Brush& brush) {
            Model::BrushList brushes;
            brushes.push_back(&brush);
            return replace(document, brushes);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::replace(Model::MapDocument& document, Model::Face& face) {
            Model::FaceList faces;
            faces.push_back(&face);
            return replace(document, faces);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::replace(Model::MapDocument& document, const Model::EntityList& entities) {
            return replace(document, entities, Model::EmptyBrushList);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::replace(Model::MapDocument& document, const Model::BrushList& brushes) {
            return replace(document, Model::EmptyEntityList, brushes);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::replace(Model::MapDocument& document, const Model::EntityList& entities, const Model::BrushList& brushes) {
            return new ChangeEditStateCommand(document, makeObjectActionName(wxT("Select"), entities, brushes), Model::EditState::Selected, entities, brushes, true);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::replace(Model::MapDocument& document, const Model::FaceList& faces) {
            return new ChangeEditStateCommand(document, faces.size() == 1 ? wxT("Select Face") : wxT("Select Faces"), Model::EditState::Selected, faces, true);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::deselectAll(Model::MapDocument& document) {
            return new ChangeEditStateCommand(document, wxT("Deselect All"), Model::EditState::Selected);
        }

        ChangeEditStateCommand* ChangeEditStateCommand::hide(Model::MapDocument& document, const Model::EntityList& entities, const Model::BrushList& brushes) {
            return new ChangeEditStateCommand(document, makeObjectActionName(wxT("Hide"), entities, brushes), Model::EditState::Hidden, entities, brushes, false);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::unHide(Model::MapDocument& document, const Model::EntityList& entities, const Model::BrushList& brushes) {
            return new ChangeEditStateCommand(document, makeObjectActionName(wxT("Unhide"), entities, brushes), Model::EditState::Default, entities, brushes, false);
        }

        ChangeEditStateCommand* ChangeEditStateCommand::unhideAll(Model::MapDocument& document) {
            return new ChangeEditStateCommand(document, wxT("Unhide All"), Model::EditState::Hidden);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::lock(Model::MapDocument& document, const Model::EntityList& entities, const Model::BrushList& brushes) {
            return new ChangeEditStateCommand(document, makeObjectActionName(wxT("Lock"), entities, brushes), Model::EditState::Locked, entities, brushes, false);
        }
        
        ChangeEditStateCommand* ChangeEditStateCommand::unLock(Model::MapDocument& document, const Model::EntityList& entities, const Model::BrushList& brushes) {
            return new ChangeEditStateCommand(document, makeObjectActionName(wxT("Unlock"), entities, brushes), Model::EditState::Default, entities, brushes, false);
        }

        ChangeEditStateCommand* ChangeEditStateCommand::unlockAll(Model::MapDocument& document) {
            return new ChangeEditStateCommand(document, wxT("Unlock All"), Model::EditState::Locked);
        }
    }
}
