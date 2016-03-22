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

#include "PageWidget.h"


PageWidget::PageWidget( QWidget* parent ) :
    QWidget( parent )
{
    showApplyButton = false;
}


QPixmap
PageWidget::getIcon()
{
    return icon;
}


QString
PageWidget::getTitel()
{
    return titel;
}


QString
PageWidget::getName()
{
    return name;
}


bool
PageWidget::getShowApplyButton()
{
    return showApplyButton;
}


void
PageWidget::setTitel( QString titel )
{
    this->titel = titel;
}


void
PageWidget::setIcon( QPixmap icon )
{
    this->icon = icon;
}


void
PageWidget::setName( QString name )
{
    this->name = name;
}


void
PageWidget::setShowApplyButton( bool visible )
{
    this->showApplyButton = visible;
}
