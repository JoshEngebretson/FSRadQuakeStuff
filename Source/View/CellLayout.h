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

#ifndef TrenchBroom_CellLayout_h
#define TrenchBroom_CellLayout_h

#include "Utility/VecMath.h"

#include <algorithm>
#include <cassert>
#include <limits>
#include <memory>
#include <vector>

using namespace TrenchBroom::Math;

namespace TrenchBroom {
    namespace View {
        class LayoutBounds {
        private:
            float m_x;
            float m_y;
            float m_width;
            float m_height;
        public:
            LayoutBounds() :
            m_x(0.0f),
            m_y(0.0f),
            m_width(0.0f),
            m_height(0.0f) {}

            LayoutBounds(float x, float y, float width, float height) :
            m_x(x),
            m_y(y),
            m_width(width),
            m_height(height) {}

            inline float left() const {
                return m_x;
            }

            inline float top() const {
                return m_y;
            }

            inline float right() const {
                return m_x + m_width;
            }

            inline float bottom() const {
                return m_y + m_height;
            }

            inline float midX() const {
                return m_x + m_width / 2.0f;
            }

            inline float midY() const {
                return m_y + m_height / 2.0f;
            }

            inline float width() const {
                return m_width;
            }

            inline float height() const {
                return m_height;
            }

            inline bool containsPoint(float x, float y) const {
                return x >= left() && x <= right() && y >= top() && y <= bottom();
            }

            inline bool intersectsY(float y, float height) const {
                return bottom() >= y || top() <= y + height;
            }
        };

        typedef enum {
            CRNone,
            CRWidth,
            CRHeight,
            CRBoth
        } LayoutCellRestriction;

        template <typename CellType>
        class LayoutCell {
        public:
        private:
            float m_scale;
            LayoutBounds m_cellBounds;
            LayoutBounds m_itemBounds;
            LayoutBounds m_titleBounds;
            CellType m_item;
        public:
            LayoutCell(CellType item, float x, float y, float itemWidth, float itemHeight, float titleWidth, float titleHeight, LayoutCellRestriction restriction, bool scaleUp, float maxUpScale, float fixedCellSize) :
            m_item(item) {
                float itemY = y;

                if (restriction != CRNone) {
                    assert(fixedCellSize > 0.0f);

                    m_scale = 1.0f;
                    float clippedTitleWidth = titleWidth;
                    float cellWidth;
                    float cellHeight;

                    if (restriction == CRWidth) {
                        if (scaleUp || itemWidth > fixedCellSize)
                            m_scale = (std::min)(maxUpScale, fixedCellSize / itemWidth);
                        clippedTitleWidth = (std::min)(fixedCellSize, titleWidth);
                        cellWidth = fixedCellSize;
                        cellHeight = m_scale * itemHeight;
                    } else if (restriction == CRHeight) {
                        if (scaleUp || itemHeight > fixedCellSize)
                            m_scale = (std::min)(maxUpScale, fixedCellSize / itemHeight);
                        else
                            itemY += fixedCellSize - itemHeight;
                        clippedTitleWidth = (std::min)(m_scale * itemWidth, titleWidth);
                        cellWidth = (std::max)(clippedTitleWidth, m_scale * itemWidth);
                        cellHeight = fixedCellSize;
                    } else {
                        if (itemWidth >= itemHeight && (scaleUp || itemWidth > fixedCellSize))
                            m_scale = (std::min)(maxUpScale, fixedCellSize / itemWidth);
                        else if (itemWidth <= itemHeight && (scaleUp || itemHeight > fixedCellSize))
                            m_scale = (std::min)(maxUpScale, fixedCellSize / itemHeight);
                        itemY += fixedCellSize - (m_scale * itemHeight);
                        clippedTitleWidth = (std::min)(fixedCellSize, titleWidth);
                        cellWidth = fixedCellSize;
                        cellHeight = fixedCellSize;
                    }

                    float scaledItemWidth = m_scale * itemWidth;
                    float scaledItemHeight = m_scale * itemHeight;

                    m_cellBounds = LayoutBounds(x, y, cellWidth, cellHeight + titleHeight);
                    m_itemBounds = LayoutBounds(x + (m_cellBounds.width() - scaledItemWidth) / 2.0f, itemY, scaledItemWidth, scaledItemHeight);
                    m_titleBounds = LayoutBounds(x + (m_cellBounds.width() - clippedTitleWidth) / 2.0f, m_itemBounds.bottom(), clippedTitleWidth, titleHeight);
                } else {
                    m_cellBounds = LayoutBounds(x, y, (std::max)(itemWidth, titleWidth), itemHeight + titleHeight);
                    m_itemBounds = LayoutBounds(x + (m_cellBounds.width() - itemWidth) / 2.0f, itemY, itemWidth, itemHeight);
                    m_titleBounds = LayoutBounds(x + (m_cellBounds.width() - titleWidth) / 2.0f, m_itemBounds.bottom(), titleWidth, titleHeight);
                }
            }

            inline bool hitTest(float x, float y) const {
                return m_cellBounds.containsPoint(x, y) || m_titleBounds.containsPoint(x, y);
            }

            inline float scale() const {
                return m_scale;
            }

            inline const LayoutBounds& cellBounds() const {
                return m_cellBounds;
            }

            inline const LayoutBounds& titleBounds() const {
                return m_titleBounds;
            }

            inline const LayoutBounds& itemBounds() const {
                return m_itemBounds;
            }

            inline void alignItemToBottom(float itemHeight) {
                float delta = itemHeight - m_itemBounds.height();
                assert(delta >= 0.0f);

                if (delta > 0.0f) {
                    m_itemBounds = LayoutBounds(m_itemBounds.left(), m_itemBounds.top() + delta, m_itemBounds.width(), m_itemBounds.height());
                    m_titleBounds = LayoutBounds(m_titleBounds.left(), m_titleBounds.top() + delta, m_titleBounds.width(), m_titleBounds.height());
                    m_cellBounds = LayoutBounds(m_cellBounds.left(), m_cellBounds.top(), m_cellBounds.width(), m_cellBounds.height() + delta);
                }
            }

            inline CellType item() const {
                return m_item;
            }

        };

        template <typename CellType>
        class LayoutRow {
        public:
            typedef LayoutCell<CellType> Cell;
            typedef std::vector<Cell> CellList;
        private:
            float m_cellMargin;
            float m_maxWidth;
            unsigned int m_maxCells;
            LayoutCellRestriction m_cellRestriction;
            bool m_scaleCellsUp;
            float m_maxUpScale;
            float m_fixedCellSize;
            LayoutBounds m_bounds;

            CellList m_cells;

            inline void alignItemsToBottom() {
                for (unsigned int i = 0; i < m_cells.size(); i++)
                    m_cells[i].alignItemToBottom(m_bounds.height());
            }
        public:
            LayoutRow(float x, float y, float cellMargin, float maxWidth, unsigned int maxCells, LayoutCellRestriction cellRestriction, bool scaleCellsUp, float maxUpScale, float fixedCellSize) :
            m_cellMargin(cellMargin),
            m_maxWidth(maxWidth),
            m_maxCells(maxCells),
            m_cellRestriction(cellRestriction),
            m_scaleCellsUp(scaleCellsUp),
            m_maxUpScale(maxUpScale),
            m_fixedCellSize(fixedCellSize),
            m_bounds(x, y, 0.0f, 0.0f) {}

            inline const Cell& operator[] (const size_t index) const {
                assert(index >= 0 && index < m_cells.size());
                return m_cells[index];
            }

            inline bool addItem(CellType item, float itemWidth, float itemHeight, float titleWidth, float titleHeight) {
                float x = m_bounds.right();
                float width = m_bounds.width();
                if (!m_cells.empty()) {
                    x += m_cellMargin;
                    width += m_cellMargin;
                }

                Cell cell(item, x, m_bounds.top(), itemWidth, itemHeight, titleWidth, titleHeight, m_cellRestriction, m_scaleCellsUp, m_maxUpScale, m_fixedCellSize);
                width += cell.cellBounds().width();

                if (m_maxCells == 0 && width > m_maxWidth && !m_cells.empty())
                    return false;
                if (m_maxCells > 0 && m_cells.size() >= m_maxCells - 1)
                    return false;

                float height = (std::max)(m_bounds.height(), cell.cellBounds().height());
                bool readjust = height > m_bounds.height();
                m_bounds = LayoutBounds(m_bounds.left(), m_bounds.top(), width, height);

                if (readjust && m_cellRestriction == CRWidth)
                    alignItemsToBottom();

                m_cells.push_back(cell);
                return true;
            }


            inline const CellList& cells() const {
                return m_cells;
            }

            inline bool cellAt(float x, float y, const Cell** result) const {
                for (unsigned int i = 0; i < m_cells.size(); i++) {
                    const Cell& cell = m_cells[i];
                    const LayoutBounds& cellBounds = cell.cellBounds();
                    if (x > cellBounds.right())
                        continue;
                    else if (x < cellBounds.left())
                        break;
                    if (cell.hitTest(x, y)) {
                        *result = &cell;
                        return true;
                    }
                }
                return false;
            }

            const LayoutBounds& bounds() const {
                return m_bounds;
            }

            bool intersectsY(float y, float height) const {
                return m_bounds.intersectsY(y, height);
            }

            size_t size() const {
                return m_cells.size();
            }
        };

        template <typename CellType, typename GroupType>
        class LayoutGroup {
        public:
            typedef LayoutRow<CellType> Row;
            typedef std::vector<Row> RowList;
        private:
            GroupType m_item;
            float m_cellMargin;
            float m_rowMargin;
            unsigned int m_maxCellsPerRow;
            LayoutCellRestriction m_cellRestriction;
            bool m_scaleCellsUp;
            float m_maxUpScale;
            float m_fixedCellSize;
            LayoutBounds m_titleBounds;
            LayoutBounds m_contentBounds;

            RowList m_rows;
            float m_maxWidth;
        public:
            inline const Row& operator[] (const size_t index) const {
                assert(index >= 0 && index < m_rows.size());
                return m_rows[index];
            }

            LayoutGroup(GroupType item, float x, float y, float cellMargin, float rowMargin, float titleHeight, float width, unsigned int maxCellsPerRow, LayoutCellRestriction cellRestriction, bool scaleCellsUp, float maxUpScale, float fixedCellSize) :
            m_item(item),
            m_cellMargin(cellMargin),
            m_rowMargin(rowMargin),
            m_maxCellsPerRow(maxCellsPerRow),
            m_cellRestriction(cellRestriction),
            m_scaleCellsUp(scaleCellsUp),
            m_maxUpScale(maxUpScale),
            m_fixedCellSize(fixedCellSize),
            m_titleBounds(0.0f, y, width + 2.0f * x, titleHeight),
            m_contentBounds(x, y + titleHeight + m_rowMargin, width, 0.0f) {}

            LayoutGroup(float x, float y, float cellMargin, float rowMargin, float width, unsigned int maxCellsPerRow, LayoutCellRestriction cellRestriction, bool scaleCellsUp, float maxUpScale, float fixedCellSize) :
            m_cellMargin(cellMargin),
            m_rowMargin(rowMargin),
            m_maxCellsPerRow(maxCellsPerRow),
            m_cellRestriction(cellRestriction),
            m_scaleCellsUp(scaleCellsUp),
            m_maxUpScale(maxUpScale),
            m_fixedCellSize(fixedCellSize),
            m_titleBounds(x, y, width, 0.0f),
            m_contentBounds(x, y, width, 0.0f) {}

            void addItem(CellType item, float itemWidth, float itemHeight, float titleWidth, float titleHeight) {
                if (m_rows.empty()) {
                    float y = m_contentBounds.top();
                    m_rows.push_back(Row(m_contentBounds.left(), y, m_cellMargin, m_contentBounds.width(), m_maxCellsPerRow, m_cellRestriction, m_scaleCellsUp, m_maxUpScale, m_fixedCellSize));
                }

                const LayoutBounds oldBounds = m_rows.back().bounds();
                const float oldRowHeight = m_rows.back().bounds().height();
                if (!m_rows.back().addItem(item, itemWidth, itemHeight, titleWidth, titleHeight)) {
                    float y = oldBounds.bottom() + m_rowMargin;
                    m_rows.push_back(Row(m_contentBounds.left(), y, m_cellMargin, m_contentBounds.width(), m_maxCellsPerRow, m_cellRestriction, m_scaleCellsUp, m_maxUpScale, m_fixedCellSize));

                    bool added = (m_rows.back().addItem(item, itemWidth, itemHeight, titleWidth, titleHeight));
                    assert(added);

                    const float newRowHeight = m_rows.back().bounds().height();
                    m_contentBounds = LayoutBounds(m_contentBounds.left(), m_contentBounds.top(), m_contentBounds.width(), m_contentBounds.height() + newRowHeight + m_rowMargin);
                } else {
                    const float newRowHeight = m_rows.back().bounds().height();
                    m_contentBounds = LayoutBounds(m_contentBounds.left(), m_contentBounds.top(), m_contentBounds.width(), m_contentBounds.height() + (newRowHeight - oldRowHeight));
                }
            }

            size_t indexOfRowAt(float y) const {
                for (size_t i = 0; i < m_rows.size(); i++) {
                    const Row& row = m_rows[i];
                    const LayoutBounds& rowBounds = row.bounds();
                    if (y < rowBounds.bottom())
                        return i;
                }
                
                return m_rows.size();
            }
            
            bool rowAt(float y, const Row** result) const {
                size_t index = indexOfRowAt(y);
                if (index == m_rows.size())
                    return false;
                
                *result = &m_rows[index];
                return true;
            }
            
            bool cellAt(float x, float y, const typename Row::Cell** result) const {
                for (unsigned int i = 0; i < m_rows.size(); i++) {
                    const Row& row = m_rows[i];
                    const LayoutBounds& rowBounds = row.bounds();
                    if (y > rowBounds.bottom())
                        continue;
                    else if (y < rowBounds.top())
                        break;
                    if (row.cellAt(x, y, result))
                        return true;
                }

                return false;
            }

            bool hitTest(float x, float y) const {
                return bounds().containsPoint(x, y);
            }

            const LayoutBounds& titleBounds() const {
                return m_titleBounds;
            }

            const LayoutBounds titleBoundsForVisibleRect(float y, float height, float groupMargin) const {
                if (intersectsY(y, height) && m_titleBounds.top() < y) {
                    if (y > m_contentBounds.bottom() - m_titleBounds.height() + groupMargin)
                        return LayoutBounds(m_titleBounds.left(), m_contentBounds.bottom() - m_titleBounds.height() + groupMargin, m_titleBounds.width(), m_titleBounds.height());
                    return LayoutBounds(m_titleBounds.left(), y, m_titleBounds.width(), m_titleBounds.height());
                }
                return m_titleBounds;
            }

            const LayoutBounds& contentBounds() const {
                return m_contentBounds;
            }

            const LayoutBounds bounds() const {
                return LayoutBounds(m_titleBounds.left(), m_titleBounds.top(), m_titleBounds.width(), m_contentBounds.bottom() - m_titleBounds.top());
            }

            bool intersectsY(float y, float height) const {
                return bounds().intersectsY(y, height);
            }

            GroupType item() const {
                return m_item;
            }

            size_t size() const {
                return m_rows.size();
            }
        };

        template <typename CellType, typename GroupType>
        class CellLayout {
        public:
            typedef LayoutGroup<CellType, GroupType> Group;
            typedef std::vector<Group> GroupList;
        private:
            float m_width;
            float m_cellMargin;
            float m_rowMargin;
            float m_groupMargin;
            float m_outerMargin;
            unsigned int m_maxCellsPerRow;
            LayoutCellRestriction m_cellRestriction;
            float m_fixedCellSize;
            bool m_scaleCellsUp;
            float m_maxUpScale;

            GroupList m_groups;
            bool m_valid;
            float m_height;

            void validate() {
                if (m_width <= 0.0f)
                    return;

                m_height = 2.0f * m_outerMargin;
                m_valid = true;
                if (!m_groups.empty()) {
                    GroupList copy = m_groups;
                    m_groups.clear();

                    for (unsigned int i = 0; i < copy.size(); i++) {
                        Group& group = copy[i];
                        addGroup(group.item(), group.titleBounds().height());
                        for (unsigned int j = 0; j < group.size(); j++) {
                            const typename Group::Row& row = group[j];
                            for (unsigned int k = 0; k < row.size(); k++) {
                                const typename Group::Row::Cell& cell = row[k];
                                const LayoutBounds& itemBounds = cell.itemBounds();
                                const LayoutBounds& titleBounds = cell.titleBounds();
                                float scale = cell.scale();
                                float itemWidth = itemBounds.width() / scale;
                                float itemHeight = itemBounds.height() / scale;
                                addItem(cell.item(), itemWidth, itemHeight, titleBounds.width(), titleBounds.height());
                            }
                        }
                    }
                }
            }
        public:
            const Group& operator[] (const size_t index) {
                assert(index >= 0 && index < m_groups.size());
                if (!m_valid)
                    validate();
                    return m_groups[index];
            }

            CellLayout(unsigned int maxCellsPerRow = 0) :
            m_width(1.0f),
            m_cellMargin(0.0f),
            m_rowMargin(0.0f),
            m_groupMargin(0.0f),
            m_outerMargin(0.0f),
            m_maxCellsPerRow(maxCellsPerRow),
            m_cellRestriction(CRNone),
            m_fixedCellSize(100.0f),
            m_scaleCellsUp(false),
            m_maxUpScale(std::numeric_limits<float>::max()) {
                invalidate();
            }

            inline void setCellMargin(float cellMargin) {
                if (m_cellMargin == cellMargin)
                    return;
                m_cellMargin = cellMargin;
                invalidate();
            }

            inline void setRowMargin(float rowMargin) {
                if (m_rowMargin == rowMargin)
                    return;
                m_rowMargin = rowMargin;
                invalidate();
            }

            inline void setGroupMargin(float groupMargin) {
                if (m_groupMargin == groupMargin)
                    return;
                m_groupMargin = groupMargin;
                invalidate();
            }

            inline void setOuterMargin(float outerMargin) {
                if (m_outerMargin == outerMargin)
                    return;
                m_outerMargin = outerMargin;
                invalidate();
            }

            void addGroup(const GroupType groupItem, float titleHeight) {
                if (!m_valid)
                    validate();

                float y = 0.0f;
                if (!m_groups.empty()) {
                    y = m_groups.back().bounds().bottom() + m_groupMargin;
                    m_height += m_groupMargin;
                }

                m_groups.push_back(Group(groupItem, m_outerMargin, y, m_cellMargin, m_rowMargin, titleHeight, m_width - 2.0f * m_outerMargin, m_maxCellsPerRow, m_cellRestriction, m_scaleCellsUp, m_maxUpScale, m_fixedCellSize));
                m_height += m_groups.back().bounds().height();
            }

            void addItem(const CellType item, float itemWidth, float itemHeight, float titleWidth, float titleHeight) {
                if (!m_valid)
                    validate();

                if (m_groups.empty()) {
                    m_groups.push_back(Group(m_outerMargin, m_outerMargin, m_cellMargin, m_rowMargin, m_width - 2.0f * m_outerMargin, m_maxCellsPerRow, m_cellRestriction, m_scaleCellsUp, m_maxUpScale, m_fixedCellSize));
                    m_height += titleHeight;
                    if (titleHeight > 0.0f)
                        m_height += m_rowMargin;
                }

                const float oldGroupHeight = m_groups.back().bounds().height();
                m_groups.back().addItem(item, itemWidth, itemHeight, titleWidth, titleHeight);
                const float newGroupHeight = m_groups.back().bounds().height();

                m_height += (newGroupHeight - oldGroupHeight);
            }

            inline void clear() {
                m_groups.clear();
                invalidate();
            }

            bool cellAt(float x, float y, const typename Group::Row::Cell** result) {
                if (!m_valid)
                    validate();

                for (unsigned int i = 0; i < m_groups.size(); i++) {
                    const Group& group = m_groups[i];
                    const LayoutBounds groupBounds = group.bounds();
                    if (y > groupBounds.bottom())
                        continue;
                    else if (y < groupBounds.top())
                        break;
                    if (group.cellAt(x, y, result))
                        return true;
                }

                return false;
            }

            bool groupAt(float x, float y, Group* result) {
                if (!m_valid)
                    validate();

                for (unsigned int i = 0; i < m_groups.size(); i++) {
                    Group* group = m_groups[i];
                    const LayoutBounds groupBounds = group->bounds();
                    if (y > groupBounds.bottom())
                        continue;
                    else if (y < groupBounds.top())
                        break;
                    if (group->hitTest(x, y)) {
                        result = group;
                        return true;
                    }
                }

                return false;
            }

            const LayoutBounds titleBoundsForVisibleRect(const Group& group, float y, float height) const {
                return group.titleBoundsForVisibleRect(y, height, m_groupMargin);
            }

            float rowPosition(float y, int offset) {
                if (!m_valid)
                    validate();

                size_t groupIndex = m_groups.size();
                for (size_t i = 0; i < m_groups.size(); i++) {
                    Group* candidate = &m_groups[i];
                    const LayoutBounds groupBounds = candidate->bounds();
                    if (y + m_rowMargin > groupBounds.bottom())
                        continue;
                    groupIndex = i;
                    break;
                }

                if (groupIndex == m_groups.size())
                    return y;
                
                size_t rowIndex = m_groups[groupIndex].indexOfRowAt(y);
                if (rowIndex == m_groups[groupIndex].size())
                    return y;

                if (offset == 0)
                    return y;
                
                int newIndex = static_cast<int>(rowIndex) + offset;
                if (newIndex < 0) {
                    while (newIndex < 0 && groupIndex > 0)
                        newIndex += m_groups[--groupIndex].size();
                } else if (newIndex >= static_cast<int>(m_groups[groupIndex].size())) {
                    while (newIndex >= static_cast<int>(m_groups[groupIndex].size()) && groupIndex < m_groups.size() - 1)
                        newIndex -= m_groups[groupIndex++].size();
                }
                
                if (groupIndex < m_groups.size()) {
                    if (newIndex >= 0) {
                        rowIndex = static_cast<size_t>(newIndex);
                        if (rowIndex < m_groups[groupIndex].size()) {
                            return m_groups[groupIndex][rowIndex].bounds().top();
                        }
                    }
                }
                
                
                return y;
            }
            
            inline size_t size() {
                if (!m_valid)
                    validate();
                return m_groups.size();
            }

            inline void invalidate() {
                m_valid = false;
            }

            inline void setWidth(float width) {
                if (m_width == width)
                    return;
                m_width = width;
                invalidate();
            }

            inline void setFixedCellSize(LayoutCellRestriction cellRestriction, float fixedCellSize) {
                if (cellRestriction == m_cellRestriction && fixedCellSize == m_fixedCellSize)
                    return;
                m_cellRestriction = cellRestriction;
                m_fixedCellSize = fixedCellSize;
                invalidate();
            }

            inline void setScaleCellsUp(bool scaleCellsUp, float maxUpScale) {
                if (scaleCellsUp == m_scaleCellsUp && maxUpScale == m_maxUpScale)
                    return;
                m_scaleCellsUp = scaleCellsUp;
                m_maxUpScale = maxUpScale;
                invalidate();
            }

            inline float fixedCellSize() const {
                return m_fixedCellSize;
            }

            inline float width() const {
                return m_width;
            }

            inline float height() {
                if (!m_valid)
                    validate();
                return m_height;
            }
            
            inline float outerMargin() const {
                return m_outerMargin();
            }
            
            inline float groupMargin() const {
                return m_groupMargin;
            }
            
            inline float rowMargin() const {
                return m_rowMargin;
            }
            
            inline float cellMargin() const {
                return m_cellMargin();
            }
        };
    }
}

#endif
