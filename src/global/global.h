/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
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

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStringList>
#include <QList>
#include <QHash>
#include <QMap>
#include <QRegExp>
#include <QDebug>
#include <QProcess>
#include "const.h"


class Global
{
public:
    struct LanguagePackage {
        QString languagePackage, parentPackage, locale;
    };

    struct KeyboardInfo {
        QString description;
        QMap< QString, QString > variants;
    };

    struct Locale {
        QString description, locale;
    };

    static bool getLanguagePackages(QList<Global::LanguagePackage> *availablePackages, QList<Global::LanguagePackage> *installedPackages);
    static bool isSystemUpToDate();

    static QMap< QString, KeyboardInfo > getKeyboardLayouts();
    static QMap<QString, QString> getKeyboardModels();
    static bool getCurrentXorgKeyboardLayout(QString & layout, QString & variant);

    static QStringList getAllEnabledLocales();
    static QString getCurrentLocale();
    static QHash<QString, QHash<QString, QList<Global::Locale> > > getAllLocales();
    static QString localeToValidLocaleGenString(QString locale);


private:
    struct LocaleInfo {
        QString locale, language, territory, description;
    };

    struct LocaleSplit {
        QString language, territory;
    };

    static QStringList getAllInstalledPackages(const QStringList & checkPackages);
    static QStringList getAllAvailableRepoPackages(const QStringList & checkPackages);
    static QList<LocaleSplit> getAllEnabledLocalesSplit();

};


#endif // GLOBAL_H
