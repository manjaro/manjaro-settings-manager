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

#include "SupportedLocalesModel.h"
#include "LanguageCommon.h"

#include <QtCore/QFile>

#include <QDebug>

SupportedLocalesModel::SupportedLocalesModel( QObject* parent )
    : QAbstractItemModel( parent )
{
    m_rootItem = new SupportedLocalesItem( QString( "key" ), QString( "value" ) );
    init( m_rootItem );
}


SupportedLocalesModel::~SupportedLocalesModel()
{
    delete m_rootItem;
}


QVariant
SupportedLocalesModel::data( const QModelIndex& index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    SupportedLocalesItem* item = static_cast<SupportedLocalesItem*>( index.internalPointer() );

    switch ( role )
    {
    case Qt::DisplayRole :
        switch ( index.column() )
        {
        case 0:
            return item->value();
        case 1:
            return item->key();
        }
        break;
    case KeyRole :
        return item->key();
    case ValueRole:
        return item->value();
    }

    return QVariant();
}


Qt::ItemFlags
SupportedLocalesModel::flags( const QModelIndex& index ) const
{
    if ( !index.isValid() )
        return 0;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant
SupportedLocalesModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
    {
        if ( section == 0 )
            return m_rootItem->key();
    }

    return QVariant();
}


QModelIndex
SupportedLocalesModel::index( int row, int column, const QModelIndex& parent ) const
{
    if ( !hasIndex( row, column, parent ) )
        return QModelIndex();

    SupportedLocalesItem* parentItem;

    if ( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<SupportedLocalesItem*>( parent.internalPointer() );

    SupportedLocalesItem* childItem = parentItem->child( row );
    if ( childItem )
        return createIndex( row, column, childItem );
    else
        return QModelIndex();
}


QModelIndex
SupportedLocalesModel::parent( const QModelIndex& index ) const
{
    if ( !index.isValid() )
        return QModelIndex();

    SupportedLocalesItem* childItem = static_cast<SupportedLocalesItem*>( index.internalPointer() );
    SupportedLocalesItem* parentItem = childItem->parent();

    if ( parentItem == m_rootItem )
        return QModelIndex();

    return createIndex( parentItem->row(), 0, parentItem );
}


int
SupportedLocalesModel::rowCount( const QModelIndex& parent ) const
{
    if ( parent.column() > 0 )
        return 0;

    SupportedLocalesItem* parentItem;
    if ( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<SupportedLocalesItem*>( parent.internalPointer() );

    return parentItem->childCount();
}


int
SupportedLocalesModel::columnCount( const QModelIndex& parent ) const
{
    if ( parent.isValid() )
        return static_cast<SupportedLocalesItem*>( parent.internalPointer() )->columnCount();
    else
        return m_rootItem->columnCount();
}


QHash<int, QByteArray>
SupportedLocalesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[KeyRole] = "key";
    roles[ValueRole] = "value";
    return roles;
}


void
SupportedLocalesModel::init( SupportedLocalesItem* parent )
{
    QStringList localeList { LanguageCommon::supportedLocales() };
    foreach ( const QString localeString, localeList )
    {
        Locale locale( localeString.toLatin1() );
        // Get language and country in current system locale
        UnicodeString uDisplayLanguage;
        UnicodeString uDisplayCountry;
        locale.getDisplayLanguage( locale, uDisplayLanguage );
        locale.getDisplayCountry( locale, uDisplayCountry );

        // Capitalize language and country
        UErrorCode status;
        BreakIterator* titleIterator = BreakIterator::createTitleInstance( locale, status );
        uDisplayLanguage = uDisplayLanguage.toTitle( titleIterator );
        uDisplayCountry = uDisplayCountry.toTitle( titleIterator );

        QString language = locale.getLanguage();
        QString country = locale.getCountry();
        QString displayLanguage = unicodeStringToQString( uDisplayLanguage );
        QString displayCountry = unicodeStringToQString( uDisplayCountry );

        // Search if we already added this language to the tree
        QModelIndexList languageIndexList = match( index( 0,0 ),
                                            KeyRole,
                                            language,
                                            -1,
                                            Qt::MatchFixedString );
        SupportedLocalesItem* languageItem;
        QModelIndex languageIndex;
        if ( languageIndexList.count() == 0 )
        {
            // Not found, add the language to the root
            languageItem = new SupportedLocalesItem( language, displayLanguage, parent );
            parent->appendChild( languageItem );
        }
        else
        {
            Q_ASSERT( languageIndexList.count() == 1 );
            // Found, convert index to a item
            languageIndex = languageIndexList.first();
            languageItem = static_cast<SupportedLocalesItem*>( languageIndex.internalPointer() );
        }

        // Search if we already added this country to this language
        QModelIndexList countryIndexList = match( languageIndex.child( 0,0 ),
                                           KeyRole,
                                           country,
                                           -1,
                                           Qt::MatchFixedString );
        SupportedLocalesItem* countryItem;
        QModelIndex countryIndex;
        if ( countryIndexList.count() == 0 )
        {
            // Not found, add the country to the language
            countryItem = new SupportedLocalesItem( country, displayCountry, languageItem );
            languageItem->appendChild( countryItem );
        }
        else
        {
            Q_ASSERT( countryIndexList.count() == 1 );
            // Found, convert index to a item
            countryIndex = countryIndexList.first();
            countryItem = static_cast<SupportedLocalesItem*>( countryIndex.internalPointer() );
        }

        // Add the locale code to the language
        SupportedLocalesItem* localeItem = new SupportedLocalesItem( localeString, localeString, countryItem );
        countryItem->appendChild( localeItem );
    }
}


icu::UnicodeString
SupportedLocalesModel::qStringToUnicodeString( const QString& sourceStr )
{
    return UnicodeString( static_cast<const ushort*>( sourceStr.utf16() ),
                          sourceStr.length() );
}


QString
SupportedLocalesModel::unicodeStringToQString( const icu::UnicodeString& sourceStr )
{
    return QString( reinterpret_cast<const QChar*>( sourceStr.getBuffer() ),
                    sourceStr.length() );
}
