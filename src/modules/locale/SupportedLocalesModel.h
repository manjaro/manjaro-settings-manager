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

#ifndef SUPPORTEDLOCALESMODEL_H
#define SUPPORTEDLOCALESMODEL_H

#include "SupportedLocalesItem.h"

#include <QtCore/QAbstractItemModel>

#include "unicode/locid.h"
#include "unicode/brkiter.h"

class SupportedLocalesModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum SupportedLocalesRoles
    {
        KeyRole = Qt::UserRole + 1,
        ValueRole
    };

    explicit SupportedLocalesModel( QObject* parent = 0 );
    ~SupportedLocalesModel();

    QVariant data( const QModelIndex& index, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;
    QVariant headerData( int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole ) const;
    QModelIndex index( int row, int column,
                       const QModelIndex& parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex& index ) const;
    int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    int columnCount( const QModelIndex& parent = QModelIndex() ) const;

    static icu::UnicodeString qStringToUnicodeString( const QString& sourceStr );
    static QString unicodeStringToQString( const icu::UnicodeString& sourceStr );

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    void init( SupportedLocalesItem* parent );

    SupportedLocalesItem* m_rootItem;

signals:

public slots:
};

#endif // SUPPORTEDLOCALESMODEL_H
