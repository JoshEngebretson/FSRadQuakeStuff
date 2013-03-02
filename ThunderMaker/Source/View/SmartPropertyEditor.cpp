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

#include "SmartPropertyEditor.h"

#include "Controller/EntityPropertyCommand.h"
#include "Model/EditStateManager.h"
#include "Model/MapDocument.h"
#include "Utility/CommandProcessor.h"
#include "View/AngleEditor.h"
#include "View/ColorEditor.h"
#include "View/SpawnFlagsEditor.h"

#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

namespace TrenchBroom {
    namespace View {
        Model::MapDocument& SmartPropertyEditor::document() const {
            return m_manager.document();
        }

        EditorView& SmartPropertyEditor::view() const {
            return m_manager.view();
        }

        const Model::EntityList SmartPropertyEditor::selectedEntities() const {
            return document().editStateManager().allSelectedEntities();
        }

        void SmartPropertyEditor::setPropertyValue(const Model::PropertyValue& value, const wxString& commandName) {
            CommandProcessor::BeginGroup(document().GetCommandProcessor(), commandName);
            Controller::EntityPropertyCommand* command = Controller::EntityPropertyCommand::setEntityPropertyValue(document(), selectedEntities(), property(), value);
            document().GetCommandProcessor()->Submit(command);
            CommandProcessor::EndGroup(document().GetCommandProcessor());
        }

        SmartPropertyEditor::SmartPropertyEditor(SmartPropertyEditorManager& manager) :
        m_manager(manager),
        m_active(false) {}

        void SmartPropertyEditor::activate(wxWindow* parent) {
            assert(!m_active);
            
            createVisual(parent);
            parent->Layout();
            
            m_active = true;
        }
        
        void SmartPropertyEditor::deactivate() {
            assert(m_active);
            destroyVisual();
            m_active = false;
        }

        void SmartPropertyEditor::update() {
            assert(m_active);
            updateVisual();
        }
        
        void SmartPropertyEditor::setProperty(const Model::PropertyKey& property) {
            assert(m_active);
            m_property = property;
            updateVisual();
        }
        
        wxWindow* DefaultPropertyEditor::createVisual(wxWindow* parent) {
            assert(m_text == NULL);
            m_text = new wxStaticText(parent, wxID_ANY, wxT("No Smart Editor Available"));
            
            wxFont defaultFont = m_text->GetFont();
            wxFont boldFont(defaultFont.GetPointSize() + 2, defaultFont.GetFamily(), defaultFont.GetStyle(), wxFONTWEIGHT_BOLD);
            m_text->SetFont(boldFont);
            m_text->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));
            
            wxSizer* outerSizer = new wxBoxSizer(wxHORIZONTAL);
            outerSizer->AddStretchSpacer();
            outerSizer->Add(m_text, 0, wxALIGN_CENTER_VERTICAL);
            outerSizer->AddStretchSpacer();
            parent->SetSizer(outerSizer);

            return m_text;
        }
        
        void DefaultPropertyEditor::destroyVisual() {
            assert(m_text != NULL);
            m_text->Destroy();
            m_text = NULL;
        }

        void DefaultPropertyEditor::updateVisual() {}

        DefaultPropertyEditor::DefaultPropertyEditor(SmartPropertyEditorManager& manager) :
        SmartPropertyEditor(manager),
        m_text(NULL) {}

        void SmartPropertyEditorManager::activateEditor(SmartPropertyEditor* editor, const Model::PropertyKey& property) {
            if (m_activeEditor != editor) {
                deactivateEditor();
                m_activeEditor = editor;
                m_activeEditor->activate(m_panel);
            }
            m_activeEditor->setProperty(property);
        }
        
        void SmartPropertyEditorManager::deactivateEditor() {
            if (m_activeEditor != NULL) {
                m_activeEditor->deactivate();
                m_activeEditor = NULL;
            }
        }

        SmartPropertyEditorManager::SmartPropertyEditorManager(wxWindow* parent, DocumentViewHolder& documentViewHolder) :
        m_documentViewHolder(documentViewHolder),
        m_panel(new wxPanel(parent)),
        m_defaultEditor(new DefaultPropertyEditor(*this)),
        m_activeEditor(NULL) {
            m_editors["spawnflags"] = EditorPtr(new SpawnFlagsEditor(*this));
            m_editors["_color"] = EditorPtr(new ColorEditor(*this));
            m_editors["_sunlight_color"] = m_editors["_color"];
            m_editors["_sunlight_color2"] = m_editors["_color"];
            // m_editors["angle"] = new AngleEditor(*this);
            
            m_panel->SetMinSize(wxSize(wxDefaultSize.x, 130));
            
            wxSizer* outerSizer = new wxBoxSizer(wxHORIZONTAL);
            outerSizer->Add(m_panel, 1, wxEXPAND);
            parent->SetSizer(outerSizer);
            
            activateEditor(m_defaultEditor, "");
        }

        SmartPropertyEditorManager::~SmartPropertyEditorManager() {
            deactivateEditor();
            
            delete m_defaultEditor;
            m_defaultEditor = NULL;
        }

        void SmartPropertyEditorManager::selectEditor(const Model::PropertyKey& key) {
            EditorMap::const_iterator it = m_editors.find(key);
            SmartPropertyEditor* editor = it == m_editors.end() ? m_defaultEditor : it->second.get();
            activateEditor(editor, key);
        }
        
        void SmartPropertyEditorManager::updateEditor() {
            m_activeEditor->update();
        }
    }
}
