/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Ramon Buldó <ramon@manjaro.org>
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

#include "KeyboardItem.h"

KeyboardItem::KeyboardItem( const QString& key, const QString& description, KeyboardItem* parent )
{
    m_parentItem = parent;
    m_key = key;
    m_description = description;
}


KeyboardItem::~KeyboardItem()
{
    qDeleteAll( m_childItems );
}


void
KeyboardItem::appendChild( KeyboardItem* item )
{
    m_childItems.append( item );
}


QString
KeyboardItem::key() const
{
    return m_key;
}


QString
KeyboardItem::description() const
{
    return m_description;
}


KeyboardItem*
KeyboardItem::child( int row )
{
    return m_childItems.value( row );
}


int
KeyboardItem::childCount() const
{
    return m_childItems.count();
}


int
KeyboardItem::columnCount() const
{
    return 2;
}


int
KeyboardItem::row() const
{
    if ( m_parentItem )
        return m_parentItem->m_childItems.indexOf( const_cast<KeyboardItem*>( this ) );

    return 0;
}


KeyboardItem*
KeyboardItem::parent()
{
    return m_parentItem;
}
