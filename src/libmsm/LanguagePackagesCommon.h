/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
 *  Ramon Buldo <ramon@manjaro.org>
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

#include "LanguagePackagesItem.h"

#include <QtCore/QString>

class LanguagePackagesCommon
{
public:
    struct LanguagePackage
    {
        QString languagePackage, parentPackage, locale;
    };

    struct Locale
    {
        QString description, locale;
    };

    struct LocaleInfo
    {
        QString locale, language, territory, description;
    };

    static bool getLanguagePackages( QList<LanguagePackagesCommon::LanguagePackage>* availablePackages,
                                     QList<LanguagePackagesCommon::LanguagePackage>* installedPackages,
                                     QList<LanguagePackagesItem> lpiList );
    static QList<LanguagePackagesCommon::LocaleInfo> getAllEnabledLocales();
    static QList<LanguagePackagesItem> getLanguagePackages();

private:
    struct LocaleSplit
    {
        QString language, territory;
    };

    static QStringList getAllInstalledPackages( const QStringList& checkPackages );
    static QStringList getAllAvailableRepoPackages( const QStringList& checkPackages );
    static QList<LocaleSplit> getAllEnabledLocalesSplit();

    static QStringList checkInstalled( const QStringList& packages, const QStringList& installedPackages );
    static QStringList checkInstalledLanguagePackages( QString package, const QStringList& installedPackages );
    static QStringList checkAvailableLanguagePackages( QString package, const QStringList& availablePackages );
    static QStringList getInstalledPackages();
    static QStringList getAvailablePackages();
};


#endif // GLOBAL_H
