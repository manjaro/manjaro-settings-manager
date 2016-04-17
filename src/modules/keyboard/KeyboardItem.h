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
    QString m_key;
    QString m_description;

    QList<KeyboardItem*> m_childItems;
    KeyboardItem* m_parentItem;
};

#endif // KEYBOARDITEM_H
