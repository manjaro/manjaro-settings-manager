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

#include "Const.h"

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
#include <QDir>
#include <QtCore/QByteArray>


class Global
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

    struct User
    {
        QString username, homePath;
        int uuid;
    };

    struct Group
    {
        QString name;
        QStringList members;
    };

    static int runProcess( QString cmd, QStringList args, QStringList writeArgs, QString& error );

    static QString getConfigValue( QString value, QString config );
    static bool setConfigValue( QString value, QString text, QString config );

    static bool getLanguagePackages( QList<Global::LanguagePackage>* availablePackages, QList<Global::LanguagePackage>* installedPackages );
    static bool isSystemUpToDate();

    static QString localeToValidLocaleGenString( QString locale );

    static QList<Global::User> getAllUsers();
    static QList<Global::Group> getAllGroups();

    static QString getRunningKernel();
    static QStringList getLtsKernels();
    static QStringList getSesKernels();
    static QStringList getRecommendedKernels();

    static QByteArray performQuery( const QStringList args );
    static QByteArray performQuery( const QString& args );
    static QByteArray packageInformation( const QString& pkgName, bool foreignPackage );
    static QString extractFieldFromInfo( const QString& field, const QString& pkgInfo );
    static QString packageVersion( const QString& pkgInfo );

private:
    struct LocaleSplit
    {
        QString language, territory;
    };

    static QStringList getAllInstalledPackages( const QStringList& checkPackages );
    static QStringList getAllAvailableRepoPackages( const QStringList& checkPackages );
    static QList<LocaleSplit> getAllEnabledLocalesSplit();
};


#endif // GLOBAL_H
