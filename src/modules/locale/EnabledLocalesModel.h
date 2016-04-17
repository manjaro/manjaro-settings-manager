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

#ifndef ENABLEDLOCALESMODEL_H
#define ENABLEDLOCALESMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QObject>
#include <QtCore/QVariant>

#include "unicode/locid.h"
#include "unicode/brkiter.h"

class EnabledLocalesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum EnabledLocalesRoles
    {
        LocaleCodeRole = Qt::UserRole + 1,
        CountryRole,
        LanguageRole,
        AddressRole,
        CollateRole,
        CtypeRole,
        IdentificationRole,
        LangRole,
        LanguageLcRole,
        MeasurementRole,
        MonetaryRole,
        MessagesRole,
        NameRole,
        NumericRole,
        PaperRole,
        TelephoneRole,
        TimeRole
    };

    struct SystemLocales
    {

    };

    explicit EnabledLocalesModel( QObject* parent = 0 );
    void init();
    void updateSystemLocales();

    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool insertLocale( int row, int count, const QString& localeCode );
    bool removeLocale( int row, int count );

    void setAddress( const QModelIndex& index );
    void setCollate( const QModelIndex& index );
    void setCtype( const QModelIndex& index );
    void setIdentification( const QModelIndex& index );
    void setLang( const QModelIndex& index );
    void setLanguage( const QModelIndex& index );
    void setMeasurement( const QModelIndex& index );
    void setMonetary( const QModelIndex& index );
    void setMessages( const QModelIndex& index );
    void setName( const QModelIndex& index );
    void setNumeric( const QModelIndex& index );
    void setPaper( const QModelIndex& index );
    void setTelephone( const QModelIndex& index );
    void setTime( const QModelIndex& index );

    QString address() const
    {
        return address_;
    }
    QString collate() const
    {
        return collate_;
    }
    QString ctype() const
    {
        return ctype_;
    }
    QString identification() const
    {
        return identification_;
    }
    QString lang() const
    {
        return lang_;
    }
    QString language() const
    {
        return language_;
    }
    QString measurement() const
    {
        return measurement_;
    }
    QString monetary() const
    {
        return monetary_;
    }
    QString messages() const
    {
        return messages_;
    }
    QString name() const
    {
        return name_;
    }
    QString numeric() const
    {
        return numeric_;
    }
    QString paper() const
    {
        return paper_;
    }
    QString telephone() const
    {
        return telephone_;
    }
    QString time() const
    {
        return time_;
    }

    bool contains( const QString& localeCode ) const;
    QStringList locales() const;

    static icu::UnicodeString qStringToUnicodeString( const QString& sourceStr );
    static QString unicodeStringToQString( const icu::UnicodeString& sourceStr );

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QStringList m_locales;
    QString address_;
    QString collate_;
    QString ctype_;
    QString identification_;
    QString lang_;
    QString language_;
    QString measurement_;
    QString monetary_;
    QString messages_;
    QString name_;
    QString numeric_;
    QString paper_;
    QString telephone_;
    QString time_;

    int findKey( const QString key ) const;

signals:

public slots:

};

#endif // ENABLEDLOCALESMODEL_H
