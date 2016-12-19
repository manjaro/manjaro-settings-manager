/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Roland Singer <roland@manjaro.org>
 *
 *  Manjaro Settings Manager is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Manjaro Settings Manager is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Manjaro Settings Manager.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PageWidget.h"


PageWidget::PageWidget( QWidget* parent ) :
    QWidget( parent )
{
    m_showApplyButton = false;
}


QPixmap
PageWidget::getIcon()
{
    return m_icon;
}


QString
PageWidget::getTitle()
{
    return m_title;
}


QString
PageWidget::getName()
{
    return m_name;
}


bool
PageWidget::getShowApplyButton()
{
    return m_showApplyButton;
}


void
PageWidget::setTitle( QString title )
{
    this->m_title = title;
}


void
PageWidget::setIcon( QPixmap icon )
{
    this->m_icon = icon;
}


void
PageWidget::setName( QString name )
{
    this->m_name = name;
}


void
PageWidget::setShowApplyButton( bool visible )
{
    this->m_showApplyButton = visible;
}
