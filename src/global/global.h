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

#include <QList>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include "const.h"


class Global
{
public:
    struct LanguagePackage {
        QString languagePackage, parentPackage, locale;
    };

    static bool getLanguagePackages(QList<Global::LanguagePackage> *availablePackages, QList<Global::LanguagePackage> *installedPackages);
    static bool isSystemUpToDate();

private:
    struct Locale {
        QString language, territory;
    };

    static QStringList getAllInstalledPackages(const QStringList & checkPackages);
    static QStringList getAllAvailableRepoPackages(const QStringList & checkPackages);
    static QList<Locale> getAllEnabledLocales();

};


#endif // GLOBAL_H
