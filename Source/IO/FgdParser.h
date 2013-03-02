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

#ifndef __TrenchBroom__FgdParser__
#define __TrenchBroom__FgdParser__

#include "IO/ClassInfo.h"
#include "IO/StreamTokenizer.h"
#include "Model/EntityDefinition.h"
#include "Model/PropertyDefinition.h"
#include "Utility/Color.h"
#include "Utility/List.h"
#include "Utility/VecMath.h"

#include <iostream>
#include <map>

using namespace TrenchBroom::Math;

namespace TrenchBroom {
    namespace Model {
        class EntityDefinition;
    }
    
    namespace IO {
        namespace FgdTokenType {
            static const unsigned int Integer           = 1 <<  0; // integer number
            static const unsigned int Decimal           = 1 <<  1; // decimal number
            static const unsigned int Word              = 1 <<  2; // letter or digits, no whitespace
            static const unsigned int QuotedString      = 1 <<  3; // "anything but quotes"
            static const unsigned int OParenthesis      = 1 <<  4; // opening parenthesis: (
            static const unsigned int CParenthesis      = 1 <<  5; // closing parenthesis: )
            static const unsigned int OBracket          = 1 <<  6; // opening bracket: [
            static const unsigned int CBracket          = 1 <<  7; // closing bracket: ]
            static const unsigned int Equality          = 1 <<  8; // equality sign: =
            static const unsigned int Colon             = 1 <<  9; // colon: :
            static const unsigned int Comma             = 1 << 10; // comma: ,
            static const unsigned int Eof               = 1 << 11; // endo fo file
        }
        
        class FgdTokenEmitter : public TokenEmitter<FgdTokenEmitter> {
        private:
            StringStream m_buffer;
        protected:
            bool isDelimiter(char c) {
                return isWhitespace(c) || c == '(' || c == ')' || c == '{' || c == '}' || c == '?' || c == ';' || c == ':' || c == ',' || c == '=';
            }
            
            Token doEmit(Tokenizer& tokenizer, size_t line, size_t column);
        };

        class FgdParser {
        protected:
            Color m_defaultEntityColor;
            StreamTokenizer<FgdTokenEmitter> m_tokenizer;
            ClassInfo::Map m_baseClasses;
            
            String typeNames(unsigned int types);
            inline void expect(unsigned int types, Token& token) {
                if ((token.type() & types) == 0)
                    throw ParserException(token.line(), token.column(), "Expected token type " + typeNames(types) + " but got " + typeNames(token.type()));
            }

            Model::PropertyDefinition::Ptr parseTargetSourceProperty(const String& propertyKey);
            Model::PropertyDefinition::Ptr parseTargetDestinationProperty(const String& propertyKey);
            Model::PropertyDefinition::Ptr parseStringProperty(const String& propertyKey);
            Model::PropertyDefinition::Ptr parseIntegerProperty(const String& propertyKey);
            Model::PropertyDefinition::Ptr parseFloatProperty(const String& propertyKey);
            Model::PropertyDefinition::Ptr parseChoicesProperty(const String& propertyKey);
            Model::PropertyDefinition::Ptr parseFlagsProperty(const String& propertyKey);
            Model::PropertyDefinition::Map parseProperties();
            
            BBox parseSize();
            Color parseColor();
            StringList parseBaseClasses();
            Model::ModelDefinition::List parseModels();
            
            ClassInfo parseClass();
            Model::EntityDefinition* parseSolidClass();
            Model::EntityDefinition* parsePointClass();
            void parseBaseClass();
        public:
            FgdParser(const Color& defaultEntityColor, std::istream& stream) :
            m_defaultEntityColor(defaultEntityColor), 
            m_tokenizer(stream) {}
            
            Model::EntityDefinition* nextDefinition();
        };
    }
}

#endif /* defined(__TrenchBroom__FgdParser__) */
