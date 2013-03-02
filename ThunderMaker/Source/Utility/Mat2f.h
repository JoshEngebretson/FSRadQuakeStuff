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

#ifndef TrenchBroom_Mat2f_h
#define TrenchBroom_Mat2f_h

#include "Utility/Vec2f.h"

#include <cassert>

namespace TrenchBroom {
    namespace Math {
        class Mat2f {
        public:
            float v[4];

            Mat2f() {
                for (unsigned int i = 0; i < 4; i++)
                    v[i] = 0;
            }
            
            Mat2f(float v11, float v12,
                  float v21, float v22) {
                v[0] = v11; v[2] = v12;
                v[1] = v21; v[3] = v22;
            }
            
            inline Mat2f& operator= (const Mat2f& right) {
                for (unsigned int i = 0; i < 4; i++)
                    v[i] = right.v[i];
                return *this;
            }
            
            inline const Mat2f operator- () const {
                return Mat2f(-v[0], -v[2],
                             -v[1], -v[3]);
            }

            inline const Mat2f operator+ (const Mat2f& right) const {
                return Mat2f(v[0] + right.v[0], v[2] + right.v[2],
                             v[1] + right.v[1], v[3] + right.v[3]);
            }
            
            inline const Mat2f operator- (const Mat2f& right) const {
                return Mat2f(v[0] - right.v[0], v[2] - right.v[2],
                             v[1] - right.v[1], v[3] - right.v[3]);
            }
            
            inline const Mat2f operator* (const float right) const {
                return Mat2f(v[0] * right, v[2] * right,
                             v[1] * right, v[3] * right);
            }
            
            inline const Vec2f operator* (const Vec2f& right) const {
                return Vec2f(v[0] * right.x + v[2] * right.y,
                             v[1] * right.x + v[3] * right.y);
            }
            
            inline const Mat2f operator* (const Mat2f& right) const {
                return Mat2f(v[0] * right.v[0] + v[2] * right.v[1], v[0] * right.v[2] + v[1] * right.v[3],
                             v[1] * right.v[0] + v[3] * right.v[1], v[1] * right.v[2] + v[3] * right.v[3]);
            }
            
            inline const Mat2f operator/ (const float right) const {
                return Mat2f(v[0] / right, v[2] / right,
                             v[1] / right, v[3] / right);
            }
            
            inline Mat2f& operator+= (const Mat2f& right) {
                for (unsigned int i = 0; i < 4; i++)
                    v[i] += right.v[i];
                return *this;
            }
            
            inline Mat2f& operator-= (const Mat2f& right) {
                for (unsigned int i = 0; i < 4; i++)
                    v[i] -= right.v[i];
                return *this;
            }
            
            inline Mat2f& operator*= (const float right) {
                for (unsigned int i = 0; i < 4; i++)
                    v[i] *= right;
                return *this;
            }
            
            inline Mat2f& operator*= (const Mat2f& right) {
                *this = *this * right;
                return *this;
            }
            
            inline Mat2f& operator/= (const float right) {
                *this *= (1.0f / right);
                return *this;
            }
            
            inline float& operator[] (const unsigned int index) {
                assert(index >= 0 && index < 4);
                return v[index];
            }
            
            inline const float& operator[] (const unsigned int index) const {
                assert(index >= 0 && index < 4);
                return v[index];
            }
            
            inline Mat2f& setIdentity() {
                for (unsigned int c = 0; c < 2; c++)
                    for (unsigned int r = 0; r < 2; r++)
                        v[c * 2 + r] = c == r ? 1.0f : 0.0f;
                return *this;
            }
            
            inline Mat2f& setValue(unsigned int row, unsigned int col, float value) {
                assert(row >= 0 && row < 2);
                assert(col >= 0 && col < 2);
                v[2 * col + row] = value;
                return *this;
            }
            
            inline Mat2f& setColumn(unsigned int col, const Vec2f& values) {
                assert(col >= 0 && col < 2);
                v[col + 0] = values.x;
                v[col + 1] = values.y;
                return *this;
            }
            
            inline Mat2f& invert(bool& invertible) {
                float det = determinant();
                if (det == 0.0f) {
                    invertible = false;
                } else {
                    invertible = true;
                    adjugate();
                    *this /= det;
                }
                return *this;
            }

            inline const Mat2f inverted(bool& invertible) const {
                Mat2f result = *this;
                result.invert(invertible);
                return result;
            }
            
            inline Mat2f& adjugate() {
                std::swap(v[0], v[3]);
                v[1] *= -1.0f;
                v[2] *= -1.0f;
                return *this;
            }

            inline const Mat2f adjugated() const {
                Mat2f result = *this;
                result.adjugate();
                return result;
            }
            
            inline Mat2f& negate() {
                for (unsigned int i = 0; i < 4; i++)
                    v[i] = -v[i];
                return *this;
            }

            inline const Mat2f negated() const {
                return Mat2f(v[0] * -1, v[1] * -1,
                             v[2] * -1, v[3] * -1);
            }
            
            inline Mat2f& transpose() {
                for (unsigned int c = 0; c < 2; c++)
                    for (unsigned int r = c + 1; r < 2; r++)
                        std::swap(v[c * 2 + r], v[r * 2 + c]);
                return *this;
            }

            inline const Mat2f transposed() const {
                Mat2f result = *this;
                result.transpose();
                return result;
            }
            
            inline float determinant() const {
                return v[0] * v[3] - v[2] * v[1];
            }
        };
        
        inline Mat2f operator*(float left, const Mat2f& right) {
            return Mat2f(left * right.v[0], left * right.v[1],
                         left * right.v[2], left * right.v[3]);
        }
    }
}

#endif
