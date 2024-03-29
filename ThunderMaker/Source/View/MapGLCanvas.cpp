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

#include "MapGLCanvas.h"

#include "Controller/CameraEvent.h"
#include "Controller/Input.h"
#include "Controller/InputController.h"
#include "Model/MapDocument.h"
#include "Renderer/ApplyMatrix.h"
#include "Renderer/Camera.h"
#include "Renderer/MapRenderer.h"
#include "Renderer/RenderContext.h"
#include "Renderer/SharedResources.h"
#include "Renderer/VertexArray.h"
#include "Model/Filter.h"
#include "Utility/Console.h"
#include "Utility/Preferences.h"
#include "Utility/VecMath.h"
#include "View/DocumentViewHolder.h"
#include "View/EditorView.h"
#include "View/DragAndDrop.h"

#include <wx/settings.h>
#include <wx/wx.h>

#include <cassert>

using namespace TrenchBroom::Math;

namespace TrenchBroom {
    namespace View {
        BEGIN_EVENT_TABLE(MapGLCanvas, wxGLCanvas)
        EVT_PAINT(MapGLCanvas::OnPaint)
        EVT_KEY_DOWN(MapGLCanvas::OnKeyDown)
        EVT_KEY_UP(MapGLCanvas::OnKeyUp)
        EVT_LEFT_DOWN(MapGLCanvas::OnMouseLeftDown)
        EVT_LEFT_UP(MapGLCanvas::OnMouseLeftUp)
        EVT_LEFT_DCLICK(MapGLCanvas::OnMouseLeftDClick)
        EVT_RIGHT_DOWN(MapGLCanvas::OnMouseRightDown)
        EVT_RIGHT_UP(MapGLCanvas::OnMouseRightUp)
        EVT_RIGHT_DCLICK(MapGLCanvas::OnMouseRightDClick)
        EVT_MIDDLE_DOWN(MapGLCanvas::OnMouseMiddleDown)
        EVT_MIDDLE_UP(MapGLCanvas::OnMouseMiddleUp)
        EVT_MIDDLE_DCLICK(MapGLCanvas::OnMouseMiddleDClick)
        EVT_MOTION(MapGLCanvas::OnMouseMove)
        EVT_MOUSEWHEEL(MapGLCanvas::OnMouseWheel)
        EVT_MOUSE_CAPTURE_LOST(MapGLCanvas::OnMouseCaptureLost)
        EVT_KILL_FOCUS(MapGLCanvas::OnKillFocus)
        EVT_IDLE(MapGLCanvas::OnIdle)
        END_EVENT_TABLE()

        wxDragResult MapGLCanvasDropTarget::OnEnter(wxCoord x, wxCoord y, wxDragResult def) {
            assert(CurrentDropSource != NULL);
            CurrentDropSource->setShowFeedback(false);

            wxTextDataObject* dataObject = static_cast<wxTextDataObject*>(CurrentDropSource->GetDataObject());
            wxString text = dataObject->GetText();
            m_inputController.dragEnter(text.ToStdString(), x, y);

            return wxDragCopy;
        }

        wxDragResult MapGLCanvasDropTarget::OnDragOver(wxCoord x, wxCoord y, wxDragResult def) {
            wxTextDataObject* dataObject = static_cast<wxTextDataObject*>(CurrentDropSource->GetDataObject());
            wxString text = dataObject->GetText();
            m_inputController.dragMove(text.ToStdString(), x, y);

            return wxDragCopy;
        }

        void MapGLCanvasDropTarget::OnLeave() {
            assert(CurrentDropSource != NULL);
            CurrentDropSource->setShowFeedback(true);
            m_inputController.dragLeave();
        }

        bool MapGLCanvasDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& data) {
            assert(CurrentDropSource != NULL);

            wxTextDataObject* dataObject = static_cast<wxTextDataObject*>(CurrentDropSource->GetDataObject());
            wxString text = dataObject->GetText();
            return m_inputController.drop(text.ToStdString(), x, y);
        }

        bool MapGLCanvas::HandleModifierKey(int keyCode, bool down) {
            Controller::ModifierKeyState key;
            switch (keyCode) {
                case WXK_SHIFT:
                    key = Controller::ModifierKeys::MKShift;
                    break;
                case WXK_ALT:
                    key = Controller::ModifierKeys::MKAlt;
                    break;
                case WXK_CONTROL:
                    key = Controller::ModifierKeys::MKCtrlCmd;
                    break;
                default:
                    key = Controller::ModifierKeys::MKNone;
                    break;
            }

            if (key != Controller::ModifierKeys::MKNone) {
                if (down)
                    m_inputController->modifierKeyDown(key);
                else
                    m_inputController->modifierKeyUp(key);
                return true;
            }

            return false;
        }

        MapGLCanvas::MapGLCanvas(wxWindow* parent, DocumentViewHolder& documentViewHolder) :
        wxGLCanvas(parent, wxID_ANY, documentViewHolder.document().sharedResources().attribs()),
        m_documentViewHolder(documentViewHolder),
        m_glContext(new wxGLContext(this, documentViewHolder.document().sharedResources().sharedContext())),
        m_inputController(new Controller::InputController(documentViewHolder)),
        m_hasFocus(false),
        m_ignoreNextClick(true) {
            SetDropTarget(new MapGLCanvasDropTarget(*m_inputController));
        }

        MapGLCanvas::~MapGLCanvas() {
            if (m_inputController != NULL) {
                delete m_inputController;
                m_inputController = NULL;
            }
            if (m_glContext != NULL) {
                wxDELETE(m_glContext);
                m_glContext = NULL;
            }
        }

        void MapGLCanvas::OnPaint(wxPaintEvent& event) {
            if (!m_documentViewHolder.valid())
                return;

            EditorView& view = m_documentViewHolder.view();

            wxPaintDC(this);
			if (SetCurrent(*m_glContext)) {
				Preferences::PreferenceManager& prefs = Preferences::PreferenceManager::preferences();
				const Color& backgroundColor = prefs.getColor(Preferences::BackgroundColor);
				glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glBindTexture(GL_TEXTURE_2D, 0);
				glDisable(GL_TEXTURE_2D);

				view.camera().update(0.0f, 0.0f, GetClientSize().x, GetClientSize().y);

                Renderer::ShaderManager& shaderManager = m_documentViewHolder.document().sharedResources().shaderManager();
                Utility::Grid& grid = m_documentViewHolder.document().grid();
				Renderer::RenderContext renderContext(view.camera(), view.filter(), shaderManager, grid, view.viewOptions(), view.console());
				view.renderer().render(renderContext);

                // draw focus rectangle
                if (FindFocus() == this) {
                    Mat4f ortho = Mat4f::Identity;
                    ortho.setOrtho(-1.0f, 1.0f, 0.0f, 0.0f, GetClientSize().x, GetClientSize().y);

                    renderContext.transformation().loadMatrix(ortho);

                    wxColour color = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);
                    float r = static_cast<float>(color.Red()) / 0xFF;
                    float g = static_cast<float>(color.Green()) / 0xFF;
                    float b = static_cast<float>(color.Blue()) / 0xFF;
                    float a = 1.0f;

                    float w = GetSize().x;
                    float h = GetSize().y;
                    float t = 3.0f;

                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDisable(GL_CULL_FACE);
					glDisable(GL_DEPTH_TEST);
                    glBegin(GL_QUADS);

                    // top
                    glColor4f(r, g, b, a);
                    glVertex2f(0.0f, 0.0f);
                    glVertex2f(w, 0.0f);
                    glColor4f(r, g, b, 0.5f * a);
                    glVertex2f(w - t, t);
                    glVertex2f(t, t);

                    // bottom
                    glColor4f(r, g, b, a);
                    glVertex2f(w, h);
                    glVertex2f(0.0f, h);
                    glColor4f(r, g, b, 0.5f * a);
                    glVertex2f(t, h - t);
                    glVertex2f(w - t, h - t);

                    // left
                    glColor4f(r, g, b, a);
                    glVertex2f(0.0f, h);
                    glVertex2f(0.0f, 0.0f);
                    glColor4f(r, g, b, 0.5f * a);
                    glVertex2f(t, t);
                    glVertex2f(t, h - t);

                    // right
                    glColor4f(r, g, b, a);
                    glVertex2f(w, 0.0f);
                    glVertex2f(w, h);
                    glColor4f(r, g, b, 0.5f * a);
                    glVertex2f(w - t, h - t);
                    glVertex2f(w - t, t);
                    glEnd();

					glEnable(GL_CULL_FACE);
					glEnable(GL_DEPTH_TEST);
                }

				SwapBuffers();
			} else {
				view.console().error("Unable to set current OpenGL context");
			}
        }

        void MapGLCanvas::OnKeyDown(wxKeyEvent& event) {
            m_ignoreNextClick = false;
            HandleModifierKey(event.GetKeyCode(), true);
        }

        void MapGLCanvas::OnKeyUp(wxKeyEvent& event) {
            m_ignoreNextClick = false;
            HandleModifierKey(event.GetKeyCode(), false);
        }

        void MapGLCanvas::OnMouseLeftDown(wxMouseEvent& event) {
            if (m_ignoreNextClick)
                return;

            SetFocus();
			CaptureMouse();
            m_inputController->mouseDown(event.GetX(), event.GetY(), Controller::MouseButtons::MBLeft);
        }

        void MapGLCanvas::OnMouseLeftUp(wxMouseEvent& event) {
            if (m_ignoreNextClick) {
                m_ignoreNextClick = false;
                return;
            }

            SetFocus();
			if (GetCapture() == this)
				ReleaseMouse();
            m_inputController->mouseUp(event.GetX(), event.GetY(), Controller::MouseButtons::MBLeft);
        }

        void MapGLCanvas::OnMouseLeftDClick(wxMouseEvent& event) {
            if (m_ignoreNextClick) {
                m_ignoreNextClick = false;
                return;
            }

            SetFocus();
            if (GetCapture() == this)
                ReleaseMouse();
            m_inputController->mouseDClick(event.GetX(), event.GetY(), Controller::MouseButtons::MBLeft);
        }

        void MapGLCanvas::OnMouseRightDown(wxMouseEvent& event) {
            if (m_ignoreNextClick)
                return;

            SetFocus();
			CaptureMouse();
            m_inputController->mouseDown(event.GetX(), event.GetY(), Controller::MouseButtons::MBRight);
        }

        void MapGLCanvas::OnMouseRightUp(wxMouseEvent& event) {
            if (m_ignoreNextClick) {
                m_ignoreNextClick = false;
                return;
            }

            SetFocus();
			if (GetCapture() == this)
				ReleaseMouse();
            if (!m_inputController->mouseUp(event.GetX(), event.GetY(), Controller::MouseButtons::MBRight)) {
                wxMenu* popupMenu = m_documentViewHolder.view().createEntityPopupMenu();
                popupMenu->UpdateUI(&m_documentViewHolder.view());
                PopupMenu(popupMenu);
                m_inputController->hidePointEntityPreview();
            }
        }

        void MapGLCanvas::OnMouseRightDClick(wxMouseEvent& event) {
            if (m_ignoreNextClick) {
                m_ignoreNextClick = false;
                return;
            }

            SetFocus();
			if (GetCapture() == this)
				ReleaseMouse();
            m_inputController->mouseDClick(event.GetX(), event.GetY(), Controller::MouseButtons::MBRight);
        }

        void MapGLCanvas::OnMouseMiddleDown(wxMouseEvent& event) {
            if (m_ignoreNextClick)
                m_ignoreNextClick = false;

            SetFocus();
			CaptureMouse();
            m_inputController->mouseDown(event.GetX(), event.GetY(), Controller::MouseButtons::MBMiddle);
        }

        void MapGLCanvas::OnMouseMiddleUp(wxMouseEvent& event) {
            if (m_ignoreNextClick) {
                m_ignoreNextClick = false;
                return;
            }

            SetFocus();
			if (GetCapture() == this)
				ReleaseMouse();
            m_inputController->mouseUp(event.GetX(), event.GetY(), Controller::MouseButtons::MBMiddle);
        }

        void MapGLCanvas::OnMouseMiddleDClick(wxMouseEvent& event) {
            if (m_ignoreNextClick) {
                m_ignoreNextClick = false;
                return;
            }

            SetFocus();
			if (GetCapture() == this)
				ReleaseMouse();
            m_inputController->mouseDClick(event.GetX(), event.GetY(), Controller::MouseButtons::MBMiddle);
        }

        void MapGLCanvas::OnMouseMove(wxMouseEvent& event) {
            m_inputController->mouseMove(event.GetX(), event.GetY());
        }

        void MapGLCanvas::OnMouseWheel(wxMouseEvent& event) {
            m_ignoreNextClick = false;
            float delta = static_cast<float>(event.GetWheelRotation()) / event.GetWheelDelta() * event.GetLinesPerAction();
            if (event.GetWheelAxis() == wxMOUSE_WHEEL_HORIZONTAL)
                m_inputController->scroll(delta, 0.0f);
            else if (event.GetWheelAxis() == wxMOUSE_WHEEL_VERTICAL)
                m_inputController->scroll(0.0f, delta);
        }

        void MapGLCanvas::OnMouseCaptureLost(wxMouseCaptureLostEvent& event) {
            m_inputController->endDrag();
        }

        void MapGLCanvas::OnKillFocus(wxFocusEvent& event) {
            m_ignoreNextClick = true;
            event.Skip();
        }

        void MapGLCanvas::OnIdle(wxIdleEvent& event) {
            // this is a fix for Mac OS X, where the kill focus event is not properly sent
            // FIXME: remove this as soon as this bug is fixed in wxWidgets 2.9.5

#ifdef __APPLE__
            wxWindow* focus = FindFocus();
            if (m_hasFocus != (focus == this)) {
                m_hasFocus = (focus == this);
                if (!m_hasFocus)
                    m_ignoreNextClick = true;
            }
#endif
        }

    }
}
