/*
 *  Manjaro Settings Manager
 *  Ramon Buldó <ramon@manjaro.org>
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

#include "SupportedLocalesItem.h"


SupportedLocalesItem::SupportedLocalesItem( const QString& key, const QString& value, SupportedLocalesItem* parent )
{
    parentItem_ = parent;
    key_ = key;
    value_ = value;
}


SupportedLocalesItem::~SupportedLocalesItem()
{
    qDeleteAll( childItems_ );
}


void
SupportedLocalesItem::appendChild( SupportedLocalesItem* item )
{
    childItems_.append( item );
}


SupportedLocalesItem*
SupportedLocalesItem::child( int row )
{
    return childItems_.value( row );
}


int
SupportedLocalesItem::childCount() const
{
    return childItems_.count();
}


int
SupportedLocalesItem::columnCount() const
{
    return 2;
}


int SupportedLocalesItem::row() const
{
    if ( parentItem_ )
        return parentItem_->childItems_.indexOf( const_cast<SupportedLocalesItem*>( this ) );

    return 0;
}


SupportedLocalesItem*
SupportedLocalesItem::parent()
{
    return parentItem_;
}


bool
SupportedLocalesItem::operator==( const SupportedLocalesItem& other )
{
    return ( key() == other.key() );
}
