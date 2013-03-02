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

#ifndef __TrenchBroom__EditorView__
#define __TrenchBroom__EditorView__

#include "Model/TextureTypes.h"
#include "Utility/VecMath.h"

#include <wx/cmdproc.h>
#include <wx/docview.h>

using namespace TrenchBroom::Math;

namespace TrenchBroom {
    namespace Controller {
        class CameraMoveEvent;
        class CameraLookEvent;
        class CameraOrbitEvent;
        class CameraSetEvent;
        class InputController;
    }
    
    namespace Model {
        class Filter;
        class MapDocument;
    }
    
    namespace Renderer {
        class Camera;
        class MapRenderer;
    }
    
    namespace Utility {
        class Console;
    }
    
    namespace View {
        class Inspector;
        class MapWindow;
        class ViewOptions;
        
        class EditorView : public wxView {
            DECLARE_DYNAMIC_CLASS(EditorView)
        protected:
            typedef enum {
                ARoll,
                APitch,
                AYaw
            } RotationAxis;
            
            Renderer::Camera* m_camera;
            Renderer::MapRenderer* m_renderer;
            Model::Filter* m_filter;
            ViewOptions* m_viewOptions;
            wxMenu* m_createEntityPopupMenu;
            wxMenu* m_createPointEntityMenu;
            
            Vec3f moveDelta(Direction direction, bool snapToGrid);
            
            void submit(wxCommand* command, bool store = true);
            void moveTextures(Direction direction, bool snapToGrid);
            void rotateTextures(bool clockwise, bool snapToGrid);
            void moveObjects(Direction direction, bool snapToGrid);
            void rotateObjects(RotationAxis rotationAxis, bool clockwise);
            void flipObjects(bool horizontally);
            void moveVertices(Direction direction, bool snapToGrid);
            void removeObjects(const wxString& actionName);
            bool canPaste();
        public:
            EditorView();

            ViewOptions& viewOptions() const;
            Model::Filter& filter() const;
            Model::MapDocument& mapDocument() const;
            Renderer::Camera& camera() const;
            Renderer::MapRenderer& renderer() const;
            View::Inspector& inspector() const;
            Utility::Console& console() const;
            Controller::InputController& inputController() const;
            wxMenu* createEntityPopupMenu();

            void setModified(bool modified);
            
            bool OnCreate(wxDocument* doc, long flags);
            void OnUpdate(wxView* sender, wxObject* hint = (wxObject*) NULL);
            void OnChangeFilename();
            void OnDraw(wxDC* dc);

            bool OnClose(bool deleteWindow = true);
            
            void OnCameraMove(Controller::CameraMoveEvent& event);
            void OnCameraLook(Controller::CameraLookEvent& event);
            void OnCameraOrbit(Controller::CameraOrbitEvent& event);
            void OnCameraSet(Controller::CameraSetEvent& event);

            void OnFileNew(wxCommandEvent& event);
            void OnFileOpen(wxCommandEvent& event);
            void OnFileSave(wxCommandEvent& event);
            void OnFileSaveAs(wxCommandEvent& event);
            void OnFileLoadPointFile(wxCommandEvent& event);
            void OnFileUnloadPointFile(wxCommandEvent& event);
            void OnFileClose(wxCommandEvent& event);
            
            void OnUndo(wxCommandEvent& event);
            void OnRedo(wxCommandEvent& event);

            void OnEditCut(wxCommandEvent& event);
            void OnEditCopy(wxCommandEvent& event);
            void OnEditPaste(wxCommandEvent& event);
            void OnEditDelete(wxCommandEvent& event);
            
            void OnEditSelectAll(wxCommandEvent& event);
            void OnEditSelectSiblings(wxCommandEvent& event);
            void OnEditSelectTouching(wxCommandEvent& event);
            void OnEditSelectNone(wxCommandEvent& event);
            
            void OnEditHideSelected(wxCommandEvent& event);
            void OnEditHideUnselected(wxCommandEvent& event);
            void OnEditUnhideAll(wxCommandEvent& event);
            
            void OnEditLockSelected(wxCommandEvent& event);
            void OnEditLockUnselected(wxCommandEvent& event);
            void OnEditUnlockAll(wxCommandEvent& event);
            
            void OnEditToggleClipTool(wxCommandEvent& event);
            void OnEditToggleClipSide(wxCommandEvent& event);
            void OnEditPerformClip(wxCommandEvent& event);
            
            void OnEditToggleVertexTool(wxCommandEvent& event);
            void OnEditToggleRotateObjectsTool(wxCommandEvent& event);
            
            void OnEditMoveTexturesUp(wxCommandEvent& event);
            void OnEditMoveTexturesRight(wxCommandEvent& event);
            void OnEditMoveTexturesDown(wxCommandEvent& event);
            void OnEditMoveTexturesLeft(wxCommandEvent& event);
            void OnEditRotateTexturesCW(wxCommandEvent& event);
            void OnEditRotateTexturesCCW(wxCommandEvent& event);
            
            void OnEditMoveTexturesUpFine(wxCommandEvent& event);
            void OnEditMoveTexturesRightFine(wxCommandEvent& event);
            void OnEditMoveTexturesDownFine(wxCommandEvent& event);
            void OnEditMoveTexturesLeftFine(wxCommandEvent& event);
            void OnEditRotateTexturesCWFine(wxCommandEvent& event);
            void OnEditRotateTexturesCCWFine(wxCommandEvent& event);
            
            void OnEditMoveObjectsForward(wxCommandEvent& event);
            void OnEditMoveObjectsRight(wxCommandEvent& event);
            void OnEditMoveObjectsBackward(wxCommandEvent& event);
            void OnEditMoveObjectsLeft(wxCommandEvent& event);
            void OnEditMoveObjectsUp(wxCommandEvent& event);
            void OnEditMoveObjectsDown(wxCommandEvent& event);
            void OnEditRollObjectsCW(wxCommandEvent& event);
            void OnEditRollObjectsCCW(wxCommandEvent& event);
            void OnEditPitchObjectsCW(wxCommandEvent& event);
            void OnEditPitchObjectsCCW(wxCommandEvent& event);
            void OnEditYawObjectsCW(wxCommandEvent& event);
            void OnEditYawObjectsCCW(wxCommandEvent& event);
            void OnEditFlipObjectsH(wxCommandEvent& event);
            void OnEditFlipObjectsV(wxCommandEvent& event);
            void OnEditDuplicateObjects(wxCommandEvent& event);
            void OnEditCorrectVertices(wxCommandEvent& event);
            void OnEditSnapVertices(wxCommandEvent& event);

            void OnEditMoveVerticesForward(wxCommandEvent& event);
            void OnEditMoveVerticesBackward(wxCommandEvent& event);
            void OnEditMoveVerticesLeft(wxCommandEvent& event);
            void OnEditMoveVerticesRight(wxCommandEvent& event);
            void OnEditMoveVerticesUp(wxCommandEvent& event);
            void OnEditMoveVerticesDown(wxCommandEvent& event);
            
            void OnEditToggleTextureLock(wxCommandEvent& event);
            void OnEditShowMapProperties(wxCommandEvent& event);
            
            void OnViewToggleShowGrid(wxCommandEvent& event);
            void OnViewToggleSnapToGrid(wxCommandEvent& event);
            void OnViewSetGridSize(wxCommandEvent& event);
            void OnViewIncGridSize(wxCommandEvent& event);
            void OnViewDecGridSize(wxCommandEvent& event);

            void OnViewMoveCameraForward(wxCommandEvent& event);
            void OnViewMoveCameraBackward(wxCommandEvent& event);
            void OnViewMoveCameraLeft(wxCommandEvent& event);
            void OnViewMoveCameraRight(wxCommandEvent& event);
            void OnViewMoveCameraUp(wxCommandEvent& event);
            void OnViewMoveCameraDown(wxCommandEvent& event);
            void OnViewCenterCameraOnSelection(wxCommandEvent& event);
            void OnViewMoveCameraToNextPoint(wxCommandEvent& event);
            void OnViewMoveCameraToPreviousPoint(wxCommandEvent& event);
            
            void OnUpdateMenuItem(wxUpdateUIEvent& event);
            
            void OnPopupReparentBrushes(wxCommandEvent& event);
            void OnPopupUpdateReparentBrushesMenuItem(wxUpdateUIEvent& event);
            void OnPopupCreatePointEntity(wxCommandEvent& event);
            void OnPopupUpdatePointMenuItem(wxUpdateUIEvent& event);
            void OnPopupCreateBrushEntity(wxCommandEvent& event);
            void OnPopupUpdateBrushMenuItem(wxUpdateUIEvent& event);
            
            DECLARE_EVENT_TABLE();
        };
    }
}

#endif /* defined(__TrenchBroom__EditorView__) */
