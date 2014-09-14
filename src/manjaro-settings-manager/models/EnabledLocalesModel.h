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

#ifndef ENABLEDLOCALESMODEL_H
#define ENABLEDLOCALESMODEL_H

#include "LocaleItem.h"

#include <QtCore/QAbstractTableModel>
#include <QtCore/QObject>
#include <QtCore/QVariant>

#include "unicode/locid.h"
#include "unicode/brkiter.h"

class EnabledLocalesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum EnabledLocalesRoles {
        KeyRole = Qt::UserRole + 1,
        CountryRole,
        LanguageRole,
        IsSystemLang
    };

    struct SystemLocales {
        QString lang;
        QString language;
        QString ctype;
        QString numeric;
        QString time;
        QString collate;
        QString monetary;
        QString messages;
        QString paper;
        QString name;
        QString address;
        QString telephone;
        QString measurement;
        QString identification;
    };

    explicit EnabledLocalesModel(QObject *parent = 0);
    void init();
    void updateSystemLocales();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool insertLocale(int row, int count, const LocaleItem &localeItem);
    bool removeLocale(int row, int count);
    bool setLang(const QModelIndex &index);

    bool contains(const LocaleItem &localeItem) const;
    QStringList locales() const;

    static icu::UnicodeString qStringToUnicodeString(const QString &sourceStr);
    static QString unicodeStringToQString(const icu::UnicodeString &sourceStr);

    SystemLocales systemLocales;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<LocaleItem> locales_;

signals:

public slots:

};

#endif // ENABLEDLOCALESMODEL_H
