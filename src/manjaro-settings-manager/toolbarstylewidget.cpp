/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
 *
 *  Copyright (C) 2007 Free Software Foundation, Inc.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "toolbarstylewidget.h"

ToolBarStyleWidget::ToolBarStyleWidget(QWidget *parent) :
    QWidget(parent)
{
    QPalette palette = this->palette();
    palette.setBrush(QPalette::WindowText, palette.brightText());
    this->setPalette(palette);
}



void ToolBarStyleWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    QRegion emptyArea(rect());

     //draw border
    if(int fw = style()->pixelMetric(QStyle::PM_MenuBarPanelWidth, 0, this)) {
        QRegion borderReg;
        borderReg += QRect(0, 0, fw, height()); //left
        borderReg += QRect(width()-fw, 0, fw, height()); //right
        borderReg += QRect(0, 0, width(), fw); //top
        borderReg += QRect(0, height()-fw, width(), fw); //bottom
        p.setClipRegion(borderReg);
        emptyArea -= borderReg;
        QStyleOptionFrame frame;
        frame.rect = rect();
        frame.palette = palette();
        frame.state = QStyle::State_None;
        frame.lineWidth = style()->pixelMetric(QStyle::PM_MenuBarPanelWidth);
        frame.midLineWidth = 0;
        style()->drawPrimitive(QStyle::PE_PanelMenuBar, &frame, &p, this);
    }
    p.setClipRegion(emptyArea);
    QStyleOptionMenuItem menuOpt;
    menuOpt.palette = palette();
    menuOpt.state = QStyle::State_None;
    menuOpt.menuItemType = QStyleOptionMenuItem::EmptyArea;
    menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
    menuOpt.rect = rect();
    menuOpt.menuRect = rect();
    style()->drawControl(QStyle::CE_MenuBarEmptyArea, &menuOpt, &p, this);
}
