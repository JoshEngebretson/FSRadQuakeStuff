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
#ifndef __TrenchBroom__EntityDefinitionManager__
#define __TrenchBroom__EntityDefinitionManager__

#include "Model/EntityDefinitionTypes.h"
#include "Model/EntityDefinition.h"
#include "Utility/String.h"

#include <map>

namespace TrenchBroom {
    namespace Utility {
        class Console;
    }
    
    namespace Model {
        class EntityDefinition;
        
        class EntityDefinitionManager {
        public:
            enum SortOrder {
                Name,
                Usage
            };
            
            typedef std::map<String, EntityDefinitionList> EntityDefinitionGroups;
        protected:
            class CompareEntityDefinitionsByName {
            private:
                bool m_shortName;
            public:
                CompareEntityDefinitionsByName(bool shortName) :
                m_shortName(shortName) {}
                
                inline bool operator() (const EntityDefinition* left, const EntityDefinition* right) const {
                    if (m_shortName)
                        return left->shortName() < right->shortName();
                    return left->name() < right->name();
                }
            };

            class CompareEntityDefinitionsByUsage {
            public:
                inline bool operator() (const EntityDefinition* left, const EntityDefinition* right) const {
                    if (left->usageCount() == right->usageCount())
                        return left->name() < right->name();
                    return left->usageCount() > right->usageCount();
                }
            };
            
            typedef std::map<String, EntityDefinition*> EntityDefinitionMap;

            Utility::Console& m_console;
            String m_path;
            EntityDefinitionMap m_entityDefinitions;
        public:
            EntityDefinitionManager(Utility::Console& console);
            ~EntityDefinitionManager();
            
            static StringList builtinDefinitionFiles();
            
            void load(const String& path);
            void clear();
            
            EntityDefinition* definition(const String& name);
            EntityDefinitionList definitions(EntityDefinition::Type type, SortOrder order = Name);
            EntityDefinitionGroups groups(EntityDefinition::Type type, SortOrder order = Name);
        };
    }
}

#endif /* defined(__TrenchBroom__EntityDefinitionManager__) */
