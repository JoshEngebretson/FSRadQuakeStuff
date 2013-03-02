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

#ifndef TrenchBroom_Grid_h
#define TrenchBroom_Grid_h

#include "Utility/VecMath.h"

using namespace TrenchBroom::Math;

namespace TrenchBroom {
    namespace Model {
        class Entity;
        class Face;
    }
    
    namespace Utility {
        class Grid {
        public:
            static const unsigned int MaxSize = 9;
        private:
            static const float SnapAngle;
            unsigned int m_size;
            bool m_snap;
            bool m_visible;
        public:
            Grid(unsigned int size) : m_size(size), m_snap(true), m_visible(true) {}

            
            inline unsigned int size() const {
                return m_size;
            }
            
            inline void setSize(unsigned int size) {
                if (m_size == size)
                    return;
                
                if (size > MaxSize)
                    m_size = MaxSize;
                else
                    m_size = size;
            }
            
            inline void incSize() {
                if (m_size < MaxSize)
                    m_size++;
            }
            
            inline void decSize() {
                if (m_size > 0)
                    m_size--;
            }
            
            inline unsigned int actualSize() const {
                if (m_snap)
                    return 1 << m_size;
                return 1;
            }
            
            inline void toggleVisible() {
                m_visible = !m_visible;
            }
            
            inline bool visible() const {
                return m_visible;
            }
            
            inline void toggleSnap() {
                m_snap = !m_snap;
            }
            
            inline bool snap() const {
                return m_snap;
            }

            inline float angle() const {
                if (m_snap)
                    return 15.0f;
                return 1.0f;
            }

            float snap(float f) const;
            float snapAngle(float a) const;
            float snapUp(float f, bool skip = false) const;
            float snapDown(float f, bool skip = false) const;
            float offset(float f) const;
            Vec3f snap(const Vec3f& p) const;
            Vec3f snapUp(const Vec3f& p, bool skip = false) const;
            Vec3f snapDown(const Vec3f& p, bool skip = false) const;
            Vec3f snapTowards(const Vec3f& p, const Vec3f& d, bool skip = false) const;
            Vec3f offset(const Vec3f& p) const;
            Vec3f snap(const Vec3f& p, const Plane& onPlane) const;
            
            float intersectWithRay(const Ray& ray, unsigned int skip) const;

            Vec3f moveDeltaForEntity(const Vec3f& origin, const BBox& worldBounds, const Vec3f& delta) const;
            Vec3f moveDeltaForEntity(const Model::Face& face, const BBox& bounds, const BBox& worldBounds, const Ray& ray, const Vec3f& position) const;
            Vec3f moveDelta(const BBox& bounds, const BBox& worldBounds, const Vec3f& delta) const;
            Vec3f moveDelta(const Vec3f& point, const BBox& worldBounds, const Vec3f& delta) const;
            Vec3f moveDelta(const Vec3f& delta) const;
            Vec3f combineDeltas(const Vec3f& delta1, const Vec3f& delta2) const;
            Vec3f referencePoint(const BBox& bounds);
            // float moveDistance(const Model::Face& face, Vec3f& delta);
        };
    }
}

#endif
