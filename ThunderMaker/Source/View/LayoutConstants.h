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

#ifndef TrenchBroom_LayoutConstants_h
#define TrenchBroom_LayoutConstants_h

#include <wx/wx.h>

namespace TrenchBroom {
    namespace View {
        namespace LayoutConstants {
#if defined _WIN32
            static const int NotebookPageInnerMargin        = 5;
            static const int NotebookPageExtraBottomMargin  = 2;
            static const int StaticBoxInnerMargin           = 15;
            static const int StaticBoxTopMargin             = 5;
            static const int StaticBoxBottomMargin          = 0;
            static const int ControlHorizontalMargin        = 10;
            static const int ControlVerticalMargin          = 10;
            static const int ControlMargin                  = 10;
            static const int CheckBoxHierarchyLeftMargin    = 20;
            static const int CheckBoxVerticalMargin         = 5;
            static const int DefaultVerticalMargin          = 10;
            static const int DialogOuterMargin              = 10;
            static const int DialogButtonMargin             = 7;
            static const int ToggleButtonStyle              = 0;
#elif defined __APPLE__
            static const int NotebookPageInnerMargin        = 5;
            static const int NotebookPageExtraBottomMargin  = 2;
            static const int StaticBoxInnerMargin           = 10;
            static const int StaticBoxTopMargin             = 0;
            static const int StaticBoxBottomMargin          = 0;
            static const int ControlHorizontalMargin        = 10;
            static const int ControlVerticalMargin          = 5;
            static const int ControlMargin                  = 5;
            static const int CheckBoxHierarchyLeftMargin    = 20;
            static const int CheckBoxVerticalMargin         = 0;
            static const int DefaultVerticalMargin          = 5;
            static const int DialogOuterMargin              = 10;
            static const int DialogButtonMargin             = 0;
            static const int ToggleButtonStyle              = wxBORDER_SUNKEN;
#elif defined __linux__
            static const int NotebookPageInnerMargin        = 5;
            static const int NotebookPageExtraBottomMargin  = 2;
            static const int StaticBoxInnerMargin           = 15;
            static const int StaticBoxTopMargin             = 0;
            static const int StaticBoxBottomMargin          = 15;
            static const int ControlHorizontalMargin        = 10;
            static const int ControlVerticalMargin          = 10;
            static const int ControlMargin                  = 10;
            static const int CheckBoxHierarchyLeftMargin    = 20;
            static const int CheckBoxVerticalMargin         = 0;
            static const int DefaultVerticalMargin          = 10;
            static const int DialogOuterMargin              = 10;
            static const int DialogButtonMargin             = 7;
            static const int ToggleButtonStyle              = 0;
#endif
        }
    }
}

#endif
