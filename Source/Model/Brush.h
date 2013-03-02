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

#ifndef __TrenchBroom__Brush__
#define __TrenchBroom__Brush__

#include "Model/BrushGeometry.h"
#include "Model/EditState.h"
#include "Model/FaceTypes.h"
#include "Model/MapObject.h"
#include "Utility/Allocator.h"
#include "Utility/VecMath.h"

#include <algorithm>
#include <iterator>
#include <vector>

using namespace TrenchBroom::Math;

namespace TrenchBroom {
    namespace Model {
        class Entity;
        class Face;
        class Texture;
        
        class Brush : public MapObject, public Utility::Allocator<Brush> {
        protected:
            class Entity* m_entity;
            FaceList m_faces;
            BrushGeometry* m_geometry;
            
            unsigned int m_selectedFaceCount;
            
            const BBox& m_worldBounds;
            
            size_t m_filePosition;
            
            void init();
        public:
            Brush(const BBox& worldBounds);
            Brush(const BBox& worldBounds, const Brush& brushTemplate);
            Brush(const BBox& worldBounds, const BBox& brushBounds, Texture* texture);
            ~Brush();
            
            void restore(const Brush& brushTemplate, bool checkId = false);
            
            inline MapObject::Type objectType() const {
                return MapObject::BrushObject;
            }
            
            inline class Entity* entity() const {
                return m_entity;
            }
            
            void setEntity(class Entity* entity);
            
            inline const FaceList& faces() const {
                return m_faces;
            }
            
            bool addFace(Face* face);
            
            void replaceFaces(const FaceList& newFaces);
            
            inline bool partiallySelected() const {
                return m_selectedFaceCount > 0;
            }
            
            inline void incSelectedFaceCount() {
                m_selectedFaceCount++;
            }
            
            inline void decSelectedFaceCount() {
                m_selectedFaceCount--;
            }
            
            virtual EditState::Type setEditState(EditState::Type editState);

            inline const BBox& worldBounds() const {
                return m_worldBounds;
            }
            
            inline const Vec3f& center() const {
                return m_geometry->center;
            }
            
            inline const BBox& bounds() const {
                return m_geometry->bounds;
            }
            
            inline const VertexList& vertices() const {
                return m_geometry->vertices;
            }
            
            inline const FaceList incidentFaces(const Vertex& vertex) const {
                const SideList sides = m_geometry->incidentSides(&vertex);
                FaceList result;
                result.reserve(sides.size());
                
                SideList::const_iterator it, end;
                for (it = sides.begin(), end = sides.end(); it != end; ++it) {
                    const Side& side = **it;
                    result.push_back(side.face);
                }
                
                return result;
            }
            
            inline const EdgeList& edges() const {
                return m_geometry->edges;
            }
            
            inline bool closed() const {
                return m_geometry->closed();
            }

            inline size_t filePosition() const {
                return m_filePosition;
            }
            
            inline void setFilePosition(size_t filePosition) {
                m_filePosition = filePosition;
            }

            void translate(const Vec3f& delta, bool lockTextures);
            void rotate90(Axis::Type axis, const Vec3f& center, bool clockwise, bool lockTextures);
            void rotate(const Quat& rotation, const Vec3f& center, bool lockTextures);
            void flip(Axis::Type axis, const Vec3f& center, bool lockTextures);
            
            void snap(unsigned int snapTo);
            
            bool canMoveBoundary(const Face& face, const Vec3f& delta) const;
            void moveBoundary(Face& face, const Vec3f& delta, bool lockTexture);
            
            bool canMoveVertices(const Vec3f::List& vertexPositions, const Vec3f& delta) const;
            Vec3f::List moveVertices(const Vec3f::List& vertexPositions, const Vec3f& delta);
            bool canMoveEdges(const EdgeList& edges, const Vec3f& delta) const;
            EdgeList moveEdges(const EdgeList& edges, const Vec3f& delta);
            bool canMoveFaces(const FaceList& faces, const Vec3f& delta) const;
            FaceList moveFaces(const FaceList& faces, const Vec3f& delta);
            
            bool canSplitEdge(Edge* edge, const Vec3f& delta) const;
            Vec3f splitEdge(Edge* edge, const Vec3f& delta);
            bool canSplitFace(Face* face, const Vec3f& delta) const;
            Vec3f splitFace(Face* face, const Vec3f& delta);
            
            void pick(const Ray& ray, PickResult& pickResults);
            bool containsPoint(const Vec3f point) const;
            bool intersectsBrush(const Brush& brush) const;
            bool containsBrush(const Brush& brush) const;
            bool intersectsEntity(const Entity& entity) const;
            bool containsEntity(const Entity& entity) const;
        };
    }
}

#endif /* defined(__TrenchBroom__Brush__) */
