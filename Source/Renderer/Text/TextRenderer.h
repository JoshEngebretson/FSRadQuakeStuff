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

#ifndef TrenchBroom_TextRenderer_h
#define TrenchBroom_TextRenderer_h

#include "Renderer/ApplyMatrix.h"
#include "Renderer/Camera.h"
#include "Renderer/RenderContext.h"
#include "Renderer/RenderUtils.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Text/PathRenderer.h"
#include "Renderer/Text/StringManager.h"
#include "Utility/String.h"
#include "Utility/VecMath.h"

#include <algorithm>
#include <cassert>
#include <map>

using namespace TrenchBroom::Math;

namespace TrenchBroom {
    namespace Renderer {
        class RenderContext;
        namespace Text {
            
            namespace Alignment {
                typedef unsigned int Type;
                static const Type Top       = 1 << 0;
                static const Type Bottom    = 1 << 1;
                static const Type Left      = 1 << 2;
                static const Type Right     = 1 << 3;
                static const Type Center    = 1 << 4;
            }
            

            class TextAnchor {
            public:
                virtual ~TextAnchor() {}
                virtual const Vec3f position() = 0;
                virtual Alignment::Type alignment() {
                    return Alignment::Center;
                }
                
                Vec3f alignmentFactors() {
                    Vec3f factors;
                    if ((alignment() & Alignment::Left) != 0)
                        factors.x = 0.5f;
                    else if ((alignment() & Alignment::Right) != 0)
                        factors.x = -0.5f;
                    if ((alignment() & Alignment::Top) != 0)
                        factors.y = -0.5f;
                    if ((alignment() & Alignment::Bottom) != 0)
                        factors.y = 0.5f;
                    return factors;
                }
            };
            
            class SimpleTextAnchor : public TextAnchor {
            private:
                Vec3f m_position;
                Alignment::Type m_alignment;
            public:
                SimpleTextAnchor(const Vec3f& position, Alignment::Type alignment) :
                m_position(position),
                m_alignment(alignment) {}
                
                const Vec3f position() {
                    return m_position;
                }
                
                Alignment::Type alignment() {
                    return m_alignment;
                }
            };
            
            template <typename Key>
            class DefaultKeyComparator {
            public:
                inline bool operator()(const Key& lhs, const Key& rhs) const {
                    return lhs < rhs;
                }
            };
            
            template <typename Key, typename Comparator = DefaultKeyComparator<Key> >
            class TextRenderer {
            public:
                class TextRendererFilter {
                public:
                    TextRendererFilter() {}
                    virtual ~TextRendererFilter() {}
                    virtual bool stringVisible(RenderContext& context, const Key& key) = 0;
                };

                class SimpleTextRendererFilter : public TextRendererFilter {
                public:
                    bool stringVisible(RenderContext& context, const Key& key) {
                        return true;
                    }
                };
                
            protected:
                class TextEntry {
                protected:
                    FontDescriptor m_fontDescriptor;
                    String m_string;
                    StringRendererPtr m_stringRenderer;
                    TextAnchor* m_textAnchor;
                public:
                    TextEntry(const FontDescriptor& fontDescriptor, const String& string, StringRendererPtr stringRenderer, TextAnchor* textAnchor) :
                    m_fontDescriptor(fontDescriptor),
                    m_string(string),
                    m_stringRenderer(stringRenderer),
                    m_textAnchor(textAnchor) {}
                    
                    inline const FontDescriptor& fontDescriptor() const {
                        return m_fontDescriptor;
                    }
                    
                    inline const String& string() const {
                        return m_string;
                    }
                    
                    inline StringRendererPtr stringRenderer() const {
                        return m_stringRenderer;
                    }
                    
                    inline void setStringRenderer(StringRendererPtr stringRenderer) const {
                        m_stringRenderer = stringRenderer;
                    }
                    
                    inline TextAnchor* textAnchor() const {
                        return m_textAnchor;
                    }
                };
                
                class EntryWithDistance {
                protected:
                    TextEntry* m_entry;
                    float m_distance;
                public:
                    EntryWithDistance(TextEntry* entry, float distance) :
                    m_entry(entry),
                    m_distance(distance) {}
                    
                    inline TextEntry& entry() const {
                        return *m_entry;
                    }
                    
                    inline float distance() const {
                        return m_distance;
                    }
                };
                
                typedef std::map<Key, TextEntry, Comparator> TextMap;
                typedef std::vector<EntryWithDistance> EntryList;
                
                StringManager& m_stringManager;
                float m_fadeDistance;
                float m_hInset;
                float m_vInset;

                TextMap m_entries;
                Vbo* m_backgroundVbo;
                
                inline void addString(Key key, const FontDescriptor& fontDescriptor, const String& string, StringRendererPtr stringRenderer, TextAnchor* anchor) {
                    removeString(key);
                    m_entries.insert(std::pair<Key, TextEntry>(key, TextEntry(fontDescriptor, string, stringRenderer, anchor)));
                }
                
                EntryList visibleEntries(RenderContext& context, TextRendererFilter& filter) {
                    float cutoff = (m_fadeDistance + 100) * (m_fadeDistance + 100);
                    EntryList result;
                    
                    typename TextMap::iterator it, end;
                    for (it = m_entries.begin(), end = m_entries.end(); it != end; ++it) {
                        Key key = it->first;
                        if (filter.stringVisible(context, key)) {
                            TextEntry& entry = it->second;
                            TextAnchor* anchor = entry.textAnchor();
                            const Vec3f position = anchor->position();
                            
                            float dist2 = context.camera().squaredDistanceTo(position);
                            if (dist2 <= cutoff)
                                result.push_back(EntryWithDistance(&entry, std::sqrt(dist2)));
                        }
                    }
                    
                    return result;
                }
                
                void renderBackground(const EntryList& entries, RenderContext& context, ShaderProgram& shaderProgram, const Color& color) {
                    if (shaderProgram.activate()) {
                        Mat4f billboardMatrix = context.camera().billboardMatrix();
                        
                        unsigned int vertexCount = static_cast<unsigned int>(3 * 16 * entries.size()); // 16 triangles (for a rounded rect with 3 triangles per corner: 3 * 4 + 4 = 16)
                        VertexArray* vertexArray = new VertexArray(*m_backgroundVbo, GL_TRIANGLES, vertexCount,
                                                                   Attribute::position3f(),
                                                                   Attribute::color4f());
                        Vec2f::List vertices;
                        vertices.reserve(vertexCount);
                        
                        m_backgroundVbo->activate();
                        m_backgroundVbo->map();
                        for (unsigned int i = 0; i < entries.size(); i++) {
                            const EntryWithDistance& entryWithDistance = entries[i];
                            TextEntry& entry = entryWithDistance.entry();
                            float dist = entryWithDistance.distance();
                            float factor = dist / 300.0f;
                            
                            StringRendererPtr stringRenderer = entry.stringRenderer();
                            TextAnchor* anchor = entry.textAnchor();
                            const Vec3f position = anchor->position();
                            
                            Mat4f matrix;
                            matrix.translate(position);
                            matrix *= billboardMatrix;
                            matrix.scale(Vec3f(factor, factor, 0.0f));
                            
                            Vec3f alignment = anchor->alignmentFactors();
                            alignment.x *= (stringRenderer->width() + 2.0f * m_hInset);
                            alignment.y *= (stringRenderer->height() + 2.0f * m_vInset);
                            matrix.translate(alignment);

                            float a = 1.0f - (std::max)(dist - m_fadeDistance, 0.0f) / 100.0f;
                            roundedRect(stringRenderer->width() + 2.0f * m_hInset, stringRenderer->height() + 2.0f * m_vInset, 3.0f, 3, vertices);
                            for (unsigned int j = 0; j < vertices.size(); j++) {
                                Vec3f vertex = Vec3f(vertices[j].x, vertices[j].y, 0.0f);
                                vertexArray->addAttribute(matrix * vertex);
                                vertexArray->addAttribute(Color(color, color.w * a));
                            }
                            vertices.clear();
                        }
                        m_backgroundVbo->unmap();
                        
                        vertexArray->render();
                        m_backgroundVbo->deactivate();
                        shaderProgram.deactivate();
                        
                        delete vertexArray;
                    }
                }
                
                void renderText(const EntryList& entries, RenderContext& context, ShaderProgram& shaderProgram, const Color& color)  {
                    if (shaderProgram.activate()) {
                        Mat4f billboardMatrix = context.camera().billboardMatrix();
                        m_stringManager.activate();
                        glSetEdgeOffset(0.01f);
                        for (unsigned int i = 0; i < entries.size(); i++) {
                            const EntryWithDistance& entryWithDistance = entries[i];
                            TextEntry& entry = entryWithDistance.entry();
                            float dist = entryWithDistance.distance();
                            float factor = dist / 300.0f;
                            
                            StringRendererPtr stringRenderer = entry.stringRenderer();
                            TextAnchor* anchor = entry.textAnchor();
                            const Vec3f position = anchor->position();
                            
                            Mat4f matrix = Mat4f::Identity;
                            matrix.translate(position);
                            matrix *= billboardMatrix;
                            matrix.scale(Vec3f(factor, factor, 0.0f));
                            
                            Vec3f alignment = anchor->alignmentFactors();
                            alignment.x *= (stringRenderer->width() + 2.0f * m_hInset);
                            alignment.y *= (stringRenderer->height() + 2.0f * m_vInset);
                            matrix.translate(alignment);

                            matrix.translate(Vec3f(-stringRenderer->width() / 2.0f, -stringRenderer->height() / 2.0f, 0.0f));

                            ApplyMatrix applyBillboard(context.transformation(), matrix);

                            float a = 1.0f - (std::max)(dist - m_fadeDistance, 0.0f) / 100.0f;
                            shaderProgram.setUniformVariable("Color", Color(color, color.w * a));
                            stringRenderer->render();
                        }
                        glResetEdgeOffset();
                        m_stringManager.deactivate();
                        shaderProgram.deactivate();
                    }
                }
            public:
                TextRenderer(StringManager& stringManager) :
                m_stringManager(stringManager),
                m_fadeDistance(100.0f),
                m_hInset(2.5f),
                m_vInset(2.5f),
                m_backgroundVbo(NULL) {}
                
                ~TextRenderer() {
                    clear();
                    delete m_backgroundVbo;
                    m_backgroundVbo = NULL;
                }
                
                inline void addString(Key key, const FontDescriptor& fontDescriptor, const String& string, TextAnchor* anchor) {
                    StringRendererPtr stringRenderer = m_stringManager.stringRenderer(fontDescriptor, string);
                    assert(stringRenderer.get() != NULL);
                    
                    addString(key, fontDescriptor, string, stringRenderer, anchor);
                }
                
                inline void removeString(Key key)  {
                    typename TextMap::iterator it = m_entries.find(key);
                    if (it != m_entries.end()) {
                        TextEntry& entry = it->second;
                        delete entry.textAnchor();
                        m_entries.erase(it);
                    }
                }
                
                inline void updateString(Key key, const String& str) {
                    typename TextMap::iterator it = m_entries.find(key);
                    if (it != m_entries.end()) {
                        TextEntry& entry = it->second;
                        const FontDescriptor& fontDescriptor = entry.fontDescriptor();
                        const String& string = entry.string();
                        
                        StringRendererPtr stringRenderer = m_stringManager.stringRenderer(fontDescriptor, string);
                        entry.setStringRenderer(stringRenderer);
                    }
                }
                
                inline void transferString(Key key, TextRenderer& destination)  {
                    typename TextMap::iterator it = m_entries.find(key);
                    if (it != m_entries.end()) {
                        TextEntry& entry = it->second;
                        destination.addString(key, entry.fontDescriptor(), entry.string(), entry.stringRenderer(), entry.textAnchor());
                        m_entries.erase(it);
                    }
                }
                
                inline bool empty() const {
                    return m_entries.empty();
                }
                
                inline void clear()  {
                    typename TextMap::iterator it, end;
                    for (it = m_entries.begin(), end = m_entries.end(); it != end; ++it) {
                        TextEntry& entry = it->second;
                        delete entry.textAnchor();
                    }
                    m_entries.clear();
                }
                
                inline void setFadeDistance(float fadeDistance)  {
                    m_fadeDistance = fadeDistance;
                }
                
                void render(RenderContext& context, TextRendererFilter& filter, ShaderProgram& textProgram, const Color& textColor, ShaderProgram& backgroundProgram, const Color& backgroundColor) {
                    if (m_entries.empty())
                        return;
                    
                    EntryList entries = visibleEntries(context, filter);
                    if (entries.empty())
                        return;
                    
                    if (m_backgroundVbo == NULL)
                        m_backgroundVbo = new Vbo(GL_ARRAY_BUFFER, 0xFFFF);
                    
                    renderBackground(entries, context, backgroundProgram, backgroundColor);
                    renderText(entries, context, textProgram, textColor);
                }
            };
        }
    }
}

#endif
