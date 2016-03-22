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

#ifndef KEYBOARDITEM_H
#define KEYBOARDITEM_H

#include <QtCore/QVariant>


class KeyboardItem
{
public:
    explicit KeyboardItem( const QString& key, const QString& description, KeyboardItem* parent = 0 );
    ~KeyboardItem();

    void appendChild( KeyboardItem* item );

    QString key() const;
    QString description() const;

    KeyboardItem* child( int row );
    int childCount() const;
    int columnCount() const;
    int row() const;
    KeyboardItem* parent();

private:
    QString key_;
    QString description_;

    QList<KeyboardItem*> childItems_;
    KeyboardItem* parentItem_;
};

#endif // KEYBOARDITEM_H
