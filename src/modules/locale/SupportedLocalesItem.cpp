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

#include "SupportedLocalesItem.h"

SupportedLocalesItem::SupportedLocalesItem( const QString& key, const QString& value, SupportedLocalesItem* parent )
{
    m_parentItem = parent;
    m_key = key;
    m_value = value;
}


SupportedLocalesItem::~SupportedLocalesItem()
{
    qDeleteAll( m_childItems );
}


void SupportedLocalesItem::appendChild( SupportedLocalesItem* item )
{
    m_childItems.append( item );
}


SupportedLocalesItem* SupportedLocalesItem::child( int row )
{
    return m_childItems.value( row );
}


int SupportedLocalesItem::childCount() const
{
    return m_childItems.count();
}


int SupportedLocalesItem::columnCount() const
{
    return 2;
}


int SupportedLocalesItem::row() const
{
    if ( m_parentItem )
        return m_parentItem->m_childItems.indexOf( const_cast<SupportedLocalesItem*>( this ) );

    return 0;
}


SupportedLocalesItem* SupportedLocalesItem::parent()
{
    return m_parentItem;
}


bool SupportedLocalesItem::operator==( const SupportedLocalesItem& other )
{
    return ( key() == other.key() );
}
