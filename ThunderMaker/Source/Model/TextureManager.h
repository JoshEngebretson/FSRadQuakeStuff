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

#ifndef __TrenchBroom__TextureManager__
#define __TrenchBroom__TextureManager__

#include "IO/Wad.h"
#include "Model/Texture.h"
#include "Model/TextureTypes.h"
#include "Utility/Color.h"
#include "Utility/String.h"

#include <algorithm>
#include <map>

namespace TrenchBroom {
    namespace IO {
        class Wad;
    }

    namespace Renderer {
        class Palette;
    }
    
    namespace Model {
        class Palette;
        
        namespace TextureSortOrder {
            typedef unsigned int Type;
            static const Type Name = 0;
            static const Type Usage = 1;
        }

        class CompareTexturesByName {
        public:
            inline bool operator() (const Texture* left, const Texture* right) const {
                return left->name() < right->name();
            }
        };
        
        class CompareTexturesByUsage {
        public:
            inline bool operator() (const Texture* left, const Texture* right) const {
                if (left->usageCount() == right->usageCount())
                    return left->name() < right->name();
                return left->usageCount() > right->usageCount();
            }
        };

        class TextureCollectionLoader {
        protected:
            IO::Wad m_wad;
        public:
            TextureCollectionLoader(const String& path) throw (IO::IOException);
            unsigned char* load(const Texture& texture, const Renderer::Palette& palette, Color& averageColor) throw (IO::IOException);
        };
        
        class TextureCollection {
        public:
            typedef std::auto_ptr<TextureCollectionLoader> LoaderPtr;
        private:
            TextureList m_textures;
            TextureList m_texturesByName;
            mutable TextureList m_texturesByUsage;
            String m_name;
            String m_path;
        public:
            TextureCollection(const String& name, const String& path) throw (IO::IOException);
            ~TextureCollection();
            
            inline const TextureList& textures() const {
                return m_textures;
            }
            
            inline TextureList textures(TextureSortOrder::Type order) const {
                if (order == TextureSortOrder::Name)
                    return m_texturesByName;
                std::sort(m_texturesByUsage.begin(), m_texturesByUsage.end(), CompareTexturesByUsage());
                return m_texturesByUsage;
            }
            
            inline const String& name() const {
                return m_name;
            }
            
            inline void update(const String& name, const String& path) {
                m_name = name;
                m_path = path;
            }
            
            LoaderPtr loader() const;
        };

        class TextureManager {
        private:
            typedef std::map<Texture*, TextureCollection*> TextureCollectionMap;
            
            TextureCollectionList m_collections;
            TextureCollectionMap m_collectionMap;
            TextureMap m_texturesCaseSensitive;
            TextureMap m_texturesCaseInsensitive;
            TextureList m_texturesByName;
            mutable TextureList m_texturesByUsage;
            void reloadTextures();
        public:
            ~TextureManager();
            
            void addCollection(TextureCollection* collection, size_t index);
            TextureCollection* removeCollection(size_t index);
            size_t indexOfTextureCollection(const String& name);
            void clear();
            
            inline const TextureCollectionList& collections() const {
                return m_collections;
            }
            
            inline const TextureList textures(TextureSortOrder::Type order) {
                if (order == TextureSortOrder::Name)
                    return m_texturesByName;
                std::sort(m_texturesByUsage.begin(), m_texturesByUsage.end(), CompareTexturesByUsage());
                return m_texturesByUsage;
            }
            
            inline Texture* texture(const std::string& name) {
                TextureMap::iterator it = m_texturesCaseSensitive.find(name);
                if (it == m_texturesCaseSensitive.end()) {
                    it = m_texturesCaseInsensitive.find(Utility::toLower(name));
                    if (it == m_texturesCaseInsensitive.end())
                        return NULL;
                }
                return it->second;
            }
            
            inline String wadProperty() const {
                StringStream str;
                for (size_t i = 0; i < m_collections.size(); i++) {
                    str << m_collections[i]->name();
                    if (i < m_collections.size() - 1)
                        str << ";";
                }
                return str.str();
            }
        };
    }
}

#endif /* defined(__TrenchBroom__TextureManager__) */
