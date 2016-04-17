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

#ifndef SUPPORTEDLOCALESITEM_H
#define SUPPORTEDLOCALESITEM_H

#include <QtCore/QVariant>

class SupportedLocalesItem
{
public:
    explicit SupportedLocalesItem( const QString& key, const QString& value, SupportedLocalesItem* parent = 0 );
    ~SupportedLocalesItem();

    void appendChild( SupportedLocalesItem* item );

    QString key() const
    {
        return m_key;
    }
    QString value() const
    {
        return m_value;
    }

    SupportedLocalesItem* child( int row );
    int childCount() const;
    int columnCount() const;
    int row() const;
    SupportedLocalesItem* parent();

    bool operator==( const SupportedLocalesItem& other );

private:
    QString m_key;
    QString m_value;

    QList<SupportedLocalesItem*> m_childItems;
    SupportedLocalesItem* m_parentItem;
};

#endif // SUPPORTEDLOCALESITEM_H
