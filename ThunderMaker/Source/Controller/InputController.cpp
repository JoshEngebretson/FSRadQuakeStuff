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

#include "InputController.h"

#include "Controller/AddObjectsCommand.h"
#include "Controller/CreateEntityFromMenuHelper.h"
#include "Controller/MoveObjectsCommand.h"
#include "Controller/ChangeEditStateCommand.h"
#include "Controller/RemoveObjectsCommand.h"
#include "Controller/ReparentBrushesCommand.h"
#include "Controller/CameraTool.h"
#include "Controller/ClipTool.h"
#include "Controller/CreateBrushTool.h"
#include "Controller/CreateEntityTool.h"
#include "Controller/MoveObjectsTool.h"
#include "Controller/MoveVerticesTool.h"
#include "Controller/ResizeBrushesTool.h"
#include "Controller/RotateObjectsTool.h"
#include "Controller/SelectionTool.h"
#include "Controller/SetFaceAttributesTool.h"
#include "Model/EditStateManager.h"
#include "Model/Entity.h"
#include "Model/EntityDefinition.h"
#include "Model/Map.h"
#include "Model/MapDocument.h"
#include "Model/Picker.h"
#include "Renderer/BoxGuideRenderer.h"
#include "Renderer/MapRenderer.h"
#include "Renderer/SharedResources.h"
#include "Utility/Console.h"
#include "Utility/Grid.h"
#include "View/DocumentViewHolder.h"

namespace TrenchBroom {
    namespace Controller {
        void InputController::updateModalTool() {
            if (m_dragTool != NULL)
                return;
            
            if (m_modalTool == NULL) {
                m_modalTool = m_toolChain->modalTool(m_inputState);
                if (m_modalTool != NULL) {
                    m_toolChain->setSuppressed(m_inputState, true, m_modalTool);
                    updateHits();
                }
            } else {
                if (!m_modalTool->isModal(m_inputState) || m_modalTool != m_toolChain->modalTool(m_inputState)) {
                    m_toolChain->setSuppressed(m_inputState, false);
                    updateHits();
                    m_modalTool = m_toolChain->modalTool(m_inputState);
                    if (m_modalTool != NULL) {
                        m_toolChain->setSuppressed(m_inputState, true, m_modalTool);
                        updateHits();
                    }
                }
            }
        }
        
        void InputController::updateHits() {
            m_inputState.invalidate();
            m_toolChain->updateHits(m_inputState);
        }
        
        void InputController::updateViews() {
            if (m_documentViewHolder.valid())
                m_documentViewHolder.document().UpdateAllViews();
        }
        
        void InputController::toggleTool(Tool* tool) {
            if (tool != NULL && tool->active()) {
                tool->deactivate(m_inputState);
            } else {
                if (m_clipTool->active())
                    m_clipTool->deactivate(m_inputState);
                if (m_moveVerticesTool->active())
                    m_moveVerticesTool->deactivate(m_inputState);
                if (m_rotateObjectsTool->active())
                    m_rotateObjectsTool->deactivate(m_inputState);
                if (tool != NULL)
                    tool->activate(m_inputState);
            }
            updateHits();
            updateModalTool();
            updateViews();
        }

        InputController::InputController(View::DocumentViewHolder& documentViewHolder) :
        m_documentViewHolder(documentViewHolder),
        m_inputState(m_documentViewHolder.view().camera(), m_documentViewHolder.document().picker()),
        m_cameraTool(NULL),
        m_clipTool(NULL),
        m_createBrushTool(NULL),
        m_createEntityTool(NULL),
        m_moveObjectsTool(NULL),
        m_rotateObjectsTool(NULL),
        m_resizeBrushesTool(NULL),
        m_setFaceAttributesTool(NULL),
        m_selectionTool(NULL),
        m_toolChain(NULL),
        m_dragTool(NULL),
        m_modalTool(NULL),
        m_cancelledDrag(false),
        m_discardNextMouseUp(false),
        m_modifierKeys(ModifierKeys::MKNone),
        m_createEntityHelper(NULL),
        m_selectionGuideRenderer(NULL),
        m_selectedFilter(Model::SelectedFilter(m_documentViewHolder.view().filter())) {
            m_cameraTool = new CameraTool(m_documentViewHolder, *this);
            m_clipTool = new ClipTool(m_documentViewHolder, *this);
            m_moveVerticesTool = new MoveVerticesTool(m_documentViewHolder, *this, 24.0f, 16.0f, 2.5f);
            m_createBrushTool = new CreateBrushTool(m_documentViewHolder, *this);
            m_createEntityTool = new CreateEntityTool(m_documentViewHolder, *this);
            m_moveObjectsTool = new MoveObjectsTool(m_documentViewHolder, *this);
            m_rotateObjectsTool = new RotateObjectsTool(m_documentViewHolder, *this, 64.0f, 32.0f, 32.0f);
            m_resizeBrushesTool = new ResizeBrushesTool(m_documentViewHolder, *this);
            m_setFaceAttributesTool = new SetFaceAttributesTool(m_documentViewHolder, *this);
            m_selectionTool = new SelectionTool(m_documentViewHolder, *this);
            
            m_cameraTool->setNextTool(m_clipTool);
            m_clipTool->setNextTool(m_rotateObjectsTool);
            m_rotateObjectsTool->setNextTool(m_moveVerticesTool);
            m_moveVerticesTool->setNextTool(m_createEntityTool);
            m_createEntityTool->setNextTool(m_createBrushTool);
            m_createBrushTool->setNextTool(m_moveObjectsTool);
            m_moveObjectsTool->setNextTool(m_resizeBrushesTool);
            m_resizeBrushesTool->setNextTool(m_setFaceAttributesTool);
            m_setFaceAttributesTool->setNextTool(m_selectionTool);
            m_toolChain = m_cameraTool;
            
            m_createBrushTool->activate(m_inputState);
            m_moveObjectsTool->activate(m_inputState);
            m_resizeBrushesTool->activate(m_inputState);
            
            m_documentViewHolder.view().renderer().addFigure(new InputControllerFigure(*this));
        }
        
        InputController::~InputController() {
            m_toolChain = NULL;
            m_dragTool = NULL;
            m_modalTool = NULL;
            
            delete m_cameraTool;
            m_cameraTool = NULL;
            delete m_clipTool;
            m_clipTool = NULL;
            delete m_moveVerticesTool;
            m_moveVerticesTool = NULL;
            delete m_createBrushTool;
            m_createBrushTool = NULL;
            delete m_createEntityTool;
            m_createEntityTool = NULL;
            delete m_moveObjectsTool;
            m_moveObjectsTool = NULL;
            delete m_rotateObjectsTool;
            m_rotateObjectsTool = NULL;
            delete m_resizeBrushesTool;
            m_resizeBrushesTool = NULL;
            delete m_setFaceAttributesTool;
            m_setFaceAttributesTool = NULL;
            delete m_selectionTool;
            m_selectionTool = NULL;
        }
        
        void InputController::modifierKeyDown(ModifierKeyState modifierKey) {
            if ((m_modifierKeys & modifierKey) == 0) {
                m_modifierKeys |= modifierKey;
                updateHits();
                m_toolChain->modifierKeyChange(m_inputState);
                updateModalTool();
                updateViews();
            }
        }
        
        void InputController::modifierKeyUp(ModifierKeyState modifierKey) {
            if ((m_modifierKeys & modifierKey) != 0) {
                m_modifierKeys &= ~modifierKey;
                updateHits();
                m_toolChain->modifierKeyChange(m_inputState);
                updateModalTool();
                updateViews();
            }
        }
        
        bool InputController::mouseDown(int x, int y, MouseButtonState mouseButton) {
            if (m_dragTool != NULL)
                return false;

            m_inputState.mouseMove(x, y);
            m_clickPos = wxPoint(m_inputState.x(), m_inputState.y());
            m_inputState.mouseDown(mouseButton);
            updateHits();
            bool handled = m_toolChain->mouseDown(m_inputState) != NULL;
            updateModalTool();
            updateViews();
            return handled;
        }
        
        bool InputController::mouseUp(int x, int y, MouseButtonState mouseButton) {
            m_inputState.mouseMove(x, y);
            if (m_discardNextMouseUp) {
                m_discardNextMouseUp = false;
                m_inputState.mouseUp(mouseButton);
                return false;
            }
            
            bool handled = false;
            if (m_dragTool != NULL) {
                m_dragTool->endDrag(m_inputState);
                m_dragTool = NULL;
                handled = true;
            } else if (!m_cancelledDrag) {
                handled = m_toolChain->mouseUp(m_inputState) != NULL;
            }
            
            m_inputState.mouseUp(mouseButton);
            m_cancelledDrag = false;
            
            updateHits();
            updateModalTool();
            updateViews();
            return handled;
        }
        
        bool InputController::mouseDClick(int x, int y, MouseButtonState mouseButton) {
            m_discardNextMouseUp = true;
            
            m_inputState.mouseMove(x, y);
            m_inputState.mouseDown(mouseButton);
            updateHits();
            bool handled = m_toolChain->mouseDClick(m_inputState) != NULL;
            updateModalTool();
            updateViews();
            return handled;
        }
        
        void InputController::mouseMove(int x, int y) {
            if (m_inputState.mouseButtons() != MouseButtons::MBNone) {
                if (m_dragTool == NULL && !m_cancelledDrag &&
                    (std::abs(m_clickPos.x - x) > 1 ||
                     std::abs(m_clickPos.y - y) > 1)) {
                        m_dragTool = m_toolChain->startDrag(m_inputState);
                        m_discardNextMouseUp = false;
                }
                m_inputState.mouseMove(x, y);
                updateHits();
                if (m_dragTool != NULL) {
                    if (!m_dragTool->drag(m_inputState)) {
                        m_dragTool->endDrag(m_inputState);
                        m_dragTool = NULL;
                        m_cancelledDrag = true;
                    }
                } else {
                    m_toolChain->mouseMove(m_inputState);
                }
            } else {
                m_inputState.mouseMove(x, y);
                updateHits();
                m_toolChain->mouseMove(m_inputState);
            }
            
            updateModalTool();
            updateViews();
        }
        
        void InputController::scroll(float x, float y) {
            m_inputState.scroll(x, y);
            updateHits();
            
            if (m_dragTool != NULL)
                m_dragTool->scroll(m_inputState);
            else
                m_toolChain->scroll(m_inputState);
            
            updateModalTool();
            updateViews();
        }
        
        void InputController::cancelDrag() {
            if (m_dragTool != NULL) {
                m_dragTool->cancelDrag(m_inputState);
                m_dragTool = NULL;
                m_inputState.mouseUp(m_inputState.mouseButtons());
            }
            
            updateHits();
            updateModalTool();
            updateViews();
        }
        
        void InputController::endDrag() {
            if (m_dragTool != NULL) {
                m_dragTool->endDrag(m_inputState);
                m_dragTool = NULL;
                m_inputState.mouseUp(m_inputState.mouseButtons());
            }
            
            updateHits();
            updateModalTool();
            updateViews();
        }
        
        void InputController::dragEnter(const String& payload, int x, int y) {
            assert(m_dragTool == NULL);
            
            deactivateAll();
            m_inputState.mouseMove(x, y);
            m_createEntityTool->activate(m_inputState);
            
            updateHits();
            Tool* dragTool = m_toolChain->dragEnter(m_inputState, payload);
            updateModalTool();
            m_dragTool = dragTool;
            updateViews();
        }
        
        void InputController::dragMove(const String& payload, int x, int y) {
            if (m_dragTool == NULL)
                return;
            
            m_inputState.mouseMove(x, y);
            updateHits();
            m_dragTool->dragMove(m_inputState);
            updateViews();
        }
        
        bool InputController::drop(const String& payload, int x, int y) {
            if (m_dragTool == NULL)
                return false;
            
            updateHits();
            bool success = m_dragTool->dragDrop(m_inputState);
            m_createEntityTool->deactivate(m_inputState);
            m_dragTool = NULL;
            
            updateHits();
            updateModalTool();
            updateViews();
            
            return success;
        }
        
        void InputController::dragLeave() {
            if (m_dragTool == NULL)
                return;
            
            updateHits();
            m_dragTool->dragLeave(m_inputState);
            m_createEntityTool->deactivate(m_inputState);
            m_dragTool = NULL;
            
            updateHits();
            updateModalTool();
            updateViews();
        }
        
        void InputController::objectsChange() {
            delete m_selectionGuideRenderer;
            m_selectionGuideRenderer = NULL;
            
            updateHits();
            m_toolChain->objectsChange(m_inputState);
            updateModalTool();
            updateViews();
        }
        
        void InputController::editStateChange(const Model::EditStateChangeSet& changeSet) {
            delete m_selectionGuideRenderer;
            m_selectionGuideRenderer = NULL;
            
            if (m_documentViewHolder.document().editStateManager().selectedBrushes().empty())
                deactivateAll();
            
            updateHits();
            m_toolChain->editStateChange(m_inputState, changeSet);
            updateModalTool();
            updateViews();
        }
        
        void InputController::cameraChange() {
            updateHits();
            m_toolChain->cameraChange(m_inputState);
            updateModalTool();
            updateViews();
        }
        
        void InputController::gridChange() {
            updateHits();
            m_toolChain->gridChange(m_inputState);
            updateModalTool();
            updateViews();
        }
        
        void InputController::render(Renderer::Vbo& vbo, Renderer::RenderContext& context) {
            m_toolChain->render(m_inputState, vbo, context);
            
            Model::EditStateManager& editStateManager = m_documentViewHolder.document().editStateManager();
            if (!moveVerticesToolActive() && !clipToolActive() &&
                editStateManager.hasSelectedObjects()) {
                Preferences::PreferenceManager& prefs = Preferences::PreferenceManager::preferences();

                if (m_selectionGuideRenderer == NULL)
                    m_selectionGuideRenderer = new Renderer::BoxGuideRenderer(editStateManager.bounds(),
                                                                              m_documentViewHolder.document().picker(),
                                                                              m_documentViewHolder.view().filter(),
                                                                              m_documentViewHolder.document().sharedResources().stringManager());
                
                if (m_moveObjectsTool->dragType() != Tool::DTNone ||
                    m_rotateObjectsTool->dragType() != Tool::DTNone ||
                    m_resizeBrushesTool->dragType() != Tool::DTNone ||
                    m_inputState.pickResult().first(Model::HitType::ObjectHit, true, m_selectedFilter) != NULL ||
                    m_inputState.pickResult().first(Model::HitType::NearEdgeHit, true, m_selectedFilter) != NULL ||
                    m_inputState.pickResult().first(Model::HitType::RotateHandleHit, true, m_documentViewHolder.view().filter()) != NULL) {
                    m_selectionGuideRenderer->setColor(prefs.getColor(Preferences::HoveredGuideColor));
                    m_selectionGuideRenderer->setShowSizes(true);
                } else {
                    m_selectionGuideRenderer->setColor(prefs.getColor(Preferences::GuideColor));
                    m_selectionGuideRenderer->setShowSizes(false);
                }

                m_selectionGuideRenderer->render(vbo, context);
            }
            
            m_toolChain->renderOverlay(m_inputState, vbo, context);
            
            if (m_createEntityHelper != NULL)
                m_createEntityHelper->render(vbo, context);
        }
        
        void InputController::freeRenderResources() {
            m_toolChain->freeRenderResources();
            delete m_selectionGuideRenderer;
            m_selectionGuideRenderer = NULL;
            delete m_createEntityHelper;
            m_createEntityHelper = NULL;
        }
        
        void InputController::toggleClipTool() {
            toggleTool(m_clipTool);
        }
        
        bool InputController::clipToolActive() {
            return m_clipTool->active();
        }
        
        void InputController::toggleClipSide() {
            assert(clipToolActive());
            m_clipTool->toggleClipSide();
            updateViews();
        }
        
        bool InputController::canDeleteClipPoint() {
            return clipToolActive() && m_clipTool->numPoints() > 0;
        }
        
        void InputController::deleteClipPoint() {
            assert(canDeleteClipPoint());
            m_clipTool->deleteLastPoint();
            updateViews();
        }
        
        bool InputController::canPerformClip() {
            return clipToolActive() && m_clipTool->numPoints() > 0;
        }
        
        void InputController::performClip() {
            assert(canPerformClip());
            m_clipTool->performClip();
            updateHits();
            updateViews();
        }
        
        void InputController::toggleMoveVerticesTool() {
            toggleTool(m_moveVerticesTool);
        }
        
        bool InputController::moveVerticesToolActive() {
            return m_moveVerticesTool->active();
        }
        
        void InputController::toggleRotateObjectsTool() {
            toggleTool(m_rotateObjectsTool);
        }
        
        bool InputController::rotateObjectsToolActive() {
            return m_rotateObjectsTool->active();
        }
        
        void InputController::deactivateAll() {
            toggleTool(NULL);
        }
        
        void InputController::createEntity(Model::EntityDefinition& definition) {
            if (definition.type() == Model::EntityDefinition::PointEntity) {
                Model::MapDocument& document = m_documentViewHolder.document();
                View::EditorView& view = m_documentViewHolder.view();
                
                const BBox& worldBounds = document.map().worldBounds();
                Model::Entity* entity = new Model::Entity(worldBounds);
                entity->setProperty(Model::Entity::ClassnameKey, definition.name());
                entity->setDefinition(&definition);
                
                Vec3f delta;
                Utility::Grid& grid = document.grid();
                
                Model::FaceHit* hit = static_cast<Model::FaceHit*>(m_inputState.pickResult().first(Model::HitType::FaceHit, true, view.filter()));
                if (hit != NULL) {
                    delta = grid.moveDeltaForEntity(hit->face(), entity->bounds(), worldBounds, m_inputState.pickRay(), hit->hitPoint());
                } else {
                    Vec3f newPosition = m_documentViewHolder.view().camera().defaultPoint(m_inputState.pickRay().direction);
                    delta = grid.moveDeltaForEntity(entity->bounds().center(), worldBounds, newPosition - entity->bounds().center());
                }
                
                // delta = grid.snap(delta);
                
                StringStream commandName;
                commandName << "Create ";
                commandName << definition.name();
                
                ChangeEditStateCommand* deselectAll = ChangeEditStateCommand::deselectAll(document);
                AddObjectsCommand* addEntity = AddObjectsCommand::addEntity(document, *entity);
                ChangeEditStateCommand* selectEntity = ChangeEditStateCommand::select(document, *entity);
                MoveObjectsCommand* moveEntity = MoveObjectsCommand::moveEntity(document, *entity, delta, document.textureLock());
                
                CommandProcessor::BeginGroup(document.GetCommandProcessor(), commandName.str());
                document.GetCommandProcessor()->Submit(deselectAll);
                document.GetCommandProcessor()->Submit(addEntity);
                document.GetCommandProcessor()->Submit(selectEntity);
                document.GetCommandProcessor()->Submit(moveEntity);
                CommandProcessor::EndGroup(document.GetCommandProcessor());
            } else {
                Model::MapDocument& document = m_documentViewHolder.document();
                Model::EditStateManager& editStateManager = document.editStateManager();
                assert(editStateManager.selectionMode() == Model::EditStateManager::SMBrushes);
                const Model::BrushList brushes = editStateManager.selectedBrushes(); // we need a copy here!
                assert(!brushes.empty());
                
                const BBox& worldBounds = document.map().worldBounds();
                Model::Entity* entity = new Model::Entity(worldBounds);
                entity->setProperty(Model::Entity::ClassnameKey, definition.name());
                entity->setDefinition(&definition);
                
                StringStream commandName;
                commandName << "Create ";
                commandName << definition.name();
                
                CommandProcessor::BeginGroup(document.GetCommandProcessor(), commandName.str());
                
                ChangeEditStateCommand* deselectAll = ChangeEditStateCommand::deselectAll(document);
                document.GetCommandProcessor()->Submit(deselectAll);

                AddObjectsCommand* addEntity = AddObjectsCommand::addEntity(document, *entity);
                document.GetCommandProcessor()->Submit(addEntity);

                ReparentBrushesCommand* reparent = ReparentBrushesCommand::reparent(document, brushes, *entity);
                document.GetCommandProcessor()->Submit(reparent);

                const Model::EntityList emptyParents = reparent->emptyParents();
                if (!emptyParents.empty()) {
                    RemoveObjectsCommand* remove = RemoveObjectsCommand::removeEntities(document, emptyParents);
                    document.GetCommandProcessor()->Submit(remove);
                }
                
                ChangeEditStateCommand* select = ChangeEditStateCommand::select(document, brushes);
                document.GetCommandProcessor()->Submit(select);
                
                CommandProcessor::EndGroup(document.GetCommandProcessor());
            }
        }
        
        Model::Entity* InputController::canReparentBrushes(const Model::BrushList& brushes) {
            Model::MapDocument& document = m_documentViewHolder.document();
            View::EditorView& view = m_documentViewHolder.view();
            
            Model::Hit* hit = m_inputState.pickResult().first(Model::HitType::ObjectHit, false, view.filter());
            Model::Entity* newParent = NULL;
            if (hit == NULL) {
                newParent = document.map().worldspawn();
            } else if (hit->type() == Model::HitType::FaceHit) {
                Model::FaceHit* faceHit = static_cast<Model::FaceHit*>(hit);
                newParent = faceHit->face().brush()->entity();
            } else {
                Model::EntityHit* entityHit = static_cast<Model::EntityHit*>(hit);
                newParent = &entityHit->entity();
            }
            
            assert(newParent != NULL);
            
            Model::BrushList::const_iterator it, end;
            for (it = brushes.begin(), end = brushes.end(); it != end; ++it) {
                const Model::Brush& brush = **it;
                if (brush.entity() != newParent)
                    return newParent;
            }
            
            return NULL;
        }
        
        void InputController::reparentBrushes(const Model::BrushList& brushes) {
            Model::MapDocument& document = m_documentViewHolder.document();
            View::EditorView& view = m_documentViewHolder.view();
            
            Model::Hit* hit = m_inputState.pickResult().first(Model::HitType::ObjectHit, false, view.filter());
            Model::Entity* newParent = NULL;
            if (hit == NULL) {
                newParent = document.map().worldspawn();
            } else if (hit->type() == Model::HitType::FaceHit) {
                Model::FaceHit* faceHit = static_cast<Model::FaceHit*>(hit);
                newParent = faceHit->face().brush()->entity();
            } else {
                Model::EntityHit* entityHit = static_cast<Model::EntityHit*>(hit);
                newParent = &entityHit->entity();
            }
            
            assert(newParent != NULL);
            
            StringStream commandName;
            commandName << "Add Brushes to ";
            commandName << *newParent->classname();
            
            ChangeEditStateCommand* deselectAll = ChangeEditStateCommand::deselectAll(document);
            ReparentBrushesCommand* reparent = ReparentBrushesCommand::reparent(document, brushes, *newParent);
            ChangeEditStateCommand* select = ChangeEditStateCommand::select(document, brushes);
            
            CommandProcessor::BeginGroup(document.GetCommandProcessor(), commandName.str());
            document.GetCommandProcessor()->Submit(deselectAll);
            document.GetCommandProcessor()->Submit(reparent);
            
            const Model::EntityList emptyParents = reparent->emptyParents();
            if (!emptyParents.empty()) {
                RemoveObjectsCommand* remove = RemoveObjectsCommand::removeEntities(document, emptyParents);
                document.GetCommandProcessor()->Submit(remove);
            }
            
            document.GetCommandProcessor()->Submit(select);
            CommandProcessor::EndGroup(document.GetCommandProcessor());
        }
        
        void InputController::showPointEntityPreview(Model::PointEntityDefinition& definition) {
            Model::MapDocument& document = m_documentViewHolder.document();
            View::EditorView& view = m_documentViewHolder.view();

            Vec3f origin;
            Utility::Grid& grid = document.grid();
            
            Model::FaceHit* hit = static_cast<Model::FaceHit*>(m_inputState.pickResult().first(Model::HitType::FaceHit, true, view.filter()));
            if (hit != NULL) {
                origin = grid.moveDeltaForEntity(hit->face(), definition.bounds(), document.map().worldBounds(), m_inputState.pickRay(), hit->hitPoint());
            } else {
                Vec3f newPosition = m_documentViewHolder.view().camera().defaultPoint(m_inputState.pickRay().direction);
                origin = grid.moveDeltaForEntity(Vec3f::Null, document.map().worldBounds(), newPosition);
            }
            
            // origin = grid.snap(origin);

            if (m_createEntityHelper == NULL)
                m_createEntityHelper = new CreateEntityFromMenuHelper(document);
            m_createEntityHelper->show(definition, origin);
            updateViews();
        }
        
        void InputController::hidePointEntityPreview() {
            if (m_createEntityHelper != NULL) {
                m_createEntityHelper->hide();
                updateViews();
            }
        }

        InputControllerFigure::InputControllerFigure(InputController& inputController) :
        m_inputController(inputController) {}
        
        InputControllerFigure::~InputControllerFigure() {
            m_inputController.freeRenderResources();
        }
        
        void InputControllerFigure::render(Renderer::Vbo& vbo, Renderer::RenderContext& context) {
            m_inputController.render(vbo, context);
        }
    }
}
