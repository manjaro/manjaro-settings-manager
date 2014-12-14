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

#include "KeyboardItem.h"

KeyboardItem::KeyboardItem(const QString &key, const QString &description, KeyboardItem *parent)
{
    parentItem_ = parent;
    key_ = key;
    description_ = description;
}

KeyboardItem::~KeyboardItem()
{
    qDeleteAll(childItems_);
}

void KeyboardItem::appendChild(KeyboardItem *item)
{
    childItems_.append(item);
}

QString KeyboardItem::key() const
{
    return key_;
}

QString KeyboardItem::description() const
{
    return description_;
}

KeyboardItem *KeyboardItem::child(int row)
{
    return childItems_.value(row);
}

int KeyboardItem::childCount() const
{
    return childItems_.count();
}

int KeyboardItem::columnCount() const
{
    return 2;
}

int KeyboardItem::row() const
{
    if (parentItem_)
        return parentItem_->childItems_.indexOf(const_cast<KeyboardItem*>(this));

    return 0;
}

KeyboardItem *KeyboardItem::parent()
{
    return parentItem_;
}
