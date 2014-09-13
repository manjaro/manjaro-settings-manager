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

#include "LocaleItem.h"

LocaleItem::LocaleItem(const QString &key, LocaleItem *parent)
{
    parentItem_ = parent;
    key_ = key;
}

LocaleItem::~LocaleItem()
{
    qDeleteAll(childItems_);
}

void LocaleItem::appendChild(LocaleItem *item)
{
    childItems_.append(item);
}

QString LocaleItem::key() const
{
    return key_;
}

LocaleItem *LocaleItem::child(int row)
{
    return childItems_.value(row);
}

int LocaleItem::childCount() const
{
    return childItems_.count();
}

int LocaleItem::columnCount() const
{
    return 1;
}

int LocaleItem::row() const
{
    if (parentItem_)
        return parentItem_->childItems_.indexOf(const_cast<LocaleItem*>(this));

    return 0;
}

LocaleItem *LocaleItem::parent()
{
    return parentItem_;
}

bool LocaleItem::operator==(const LocaleItem &other)
{
    return (key() == other.key());
}
