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

#include "AliasModelRenderer.h"

#include <GL/glew.h>
#include "Model/Alias.h"
#include "Model/Entity.h"
#include "Renderer/MapRenderer.h"
#include "Renderer/Palette.h"
#include "Renderer/RenderContext.h"
#include "Renderer/TextureRenderer.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Vbo.h"

#include <cassert>

namespace TrenchBroom {
    namespace Renderer {
        AliasModelRenderer::AliasModelRenderer(const Model::Alias& alias, unsigned int frameIndex, unsigned int skinIndex, Vbo& vbo, const Palette& palette) :
        m_alias(alias),
        m_frameIndex(frameIndex),
        m_skinIndex(skinIndex),
        m_palette(palette),
        m_texture(NULL),
        m_vbo(vbo),
        m_vertexArray(NULL) {}

        AliasModelRenderer::~AliasModelRenderer() {
            m_frameIndex = 0;
            m_skinIndex = 0;
            delete m_vertexArray;
            m_vertexArray = NULL;
        }

        void AliasModelRenderer::render(ShaderProgram& shaderProgram) {
            if (m_vertexArray == NULL) {
                assert(m_skinIndex < m_alias.skins().size());
                assert(m_frameIndex < m_alias.frames().size());
                
                Model::AliasSkin& skin = *m_alias.skins()[m_skinIndex];
                m_texture = TextureRendererPtr(new TextureRenderer(skin, 0, m_palette));

                Model::AliasSingleFrame& frame = m_alias.frame(m_frameIndex);
                const Model::AliasFrameTriangleList& triangles = frame.triangles();
                unsigned int vertexCount = static_cast<unsigned int>(3 * triangles.size());
                
                m_vertexArray = new VertexArray(m_vbo, GL_TRIANGLES, vertexCount,
                                                Attribute::position3f(),
                                                Attribute::texCoord02f());

                SetVboState mapVbo(m_vbo, Vbo::VboMapped);
                for (unsigned int i = 0; i < triangles.size(); i++) {
                    Model::AliasFrameTriangle& triangle = *triangles[i];
                    for (unsigned int j = 0; j < 3; j++) {
                        Model::AliasFrameVertex& vertex = triangle[j];
                        m_vertexArray->addAttribute(vertex.position());
                        m_vertexArray->addAttribute(vertex.texCoords());
                    }
                }
            }

            assert(m_vertexArray != NULL);
            
            glActiveTexture(GL_TEXTURE0);
            m_texture->activate();
            shaderProgram.setUniformVariable("Texture", 0);
            m_vertexArray->render();
            m_texture->deactivate();
        }

        const Vec3f& AliasModelRenderer::center() const {
            return m_alias.firstFrame().center();
        }

        const BBox& AliasModelRenderer::bounds() const {
            return m_alias.firstFrame().bounds();
        }

        BBox AliasModelRenderer::boundsAfterTransformation(const Mat4f& transformation) const {
            Model::AliasSingleFrame& frame = m_alias.frame(m_frameIndex);
            const Model::AliasFrameTriangleList& triangles = frame.triangles();

            BBox bounds;
            bounds.min = bounds.max = transformation * (*triangles[0])[0].position();
            
            for (unsigned int i = 1; i < triangles.size(); i++) {
                Model::AliasFrameTriangle& triangle = *triangles[i];
                for (unsigned int j = 0; j < 3; j++) {
                    Model::AliasFrameVertex& vertex = triangle[j];
                    bounds.mergeWith(transformation * vertex.position());
                }
            }
            
            return bounds;
        }
    }
}
