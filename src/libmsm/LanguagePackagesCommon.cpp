/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
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

#include "LanguagePackagesCommon.h"
#include "LanguagePackages.h"
#include "LanguageCommon.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtCore/QRegularExpression>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

bool
LanguagePackagesCommon::getLanguagePackages( QList<LanguagePackagesCommon::LanguagePackage>* availablePackages,
        QList<LanguagePackagesCommon::LanguagePackage>* installedPackages,
        QList<LanguagePackagesItem> lpiList )
{
    QList<LanguagePackagesCommon::LanguagePackage> languagePackages;
    QStringList parentPackages, checkLP;
    QList<LanguagePackagesCommon::LocaleSplit> locales = getAllEnabledLocalesSplit();

    if ( locales.isEmpty() )
    {
        qDebug() << "error: didn't find any enabled locale";
        return false;
    }

    for ( const auto& lpi : lpiList )
    {
        for ( const auto parentPkg : lpi.parentPackages() )
        {
            LanguagePackage lp;
            lp.parentPackage = parentPkg;
            lp.languagePackage = lpi.languagePackage();
            parentPackages.append( lp.parentPackage );
            languagePackages.append( lp );
        }

    }

    // Keep only installed parent packages
    parentPackages = getAllInstalledPackages( parentPackages );

    for ( int i = 0; i < languagePackages.size(); i++ )
    {
        LanguagePackage* lp = &languagePackages[i];

        if ( !parentPackages.contains( lp->parentPackage ) )
            continue;

        // Check if it is a global language pack
        if ( !lp->languagePackage.contains( "%" ) )
        {
            checkLP.append( lp->languagePackage );
            continue;
        }

        for ( int i = 0; i < locales.size(); i++ )
        {
            LocaleSplit* locale = &locales[i];

            // Example: firefox-i18n-% -> firefox-i18n-en-US
            checkLP.append( QString( lp->languagePackage ).replace( "%", QString( "%1-%2" ).arg( locale->language.toLower(), locale->territory ) ) );

            // Example: firefox-i18n-% -> firefox-i18n-en-us
            checkLP.append( QString( lp->languagePackage ).replace( "%", QString( "%1-%2" ).arg( locale->language.toLower(), locale->territory.toLower() ) ) );

            // Example: firefox-i18n-% -> firefox-i18n-en_US
            checkLP.append( QString( lp->languagePackage ).replace( "%", QString( "%1_%2" ).arg( locale->language.toLower(), locale->territory ) ) );

            // Example: firefox-i18n-% -> firefox-i18n-en_us
            checkLP.append( QString( lp->languagePackage ).replace( "%", QString( "%1_%2" ).arg( locale->language.toLower(), locale->territory.toLower() ) ) );

            // Example: firefox-i18n-% -> firefox-i18n-en
            checkLP.append( QString( lp->languagePackage ).replace( "%", locale->language.toLower() ) );
        }
    }


    // Keep only valid language packages
    checkLP.removeDuplicates();
    checkLP = getAllAvailableRepoPackages( checkLP );

    // Sort installed and available packages
    QStringList availableLP, installedLP = getAllInstalledPackages( checkLP );

    foreach ( QString lp, checkLP )
    {
        if ( !installedLP.contains( lp ) )
            availableLP.append( lp );
    }

    availableLP.sort();
    installedLP.sort();

    // Now add them to availablePackages and installedPackages
    for ( int i = 0; i < languagePackages.size(); i++ )
    {
        LanguagePackage* lp = &languagePackages[i];

        if ( !parentPackages.contains( lp->parentPackage ) )
            continue;

        // Check if it is a global language pack
        if ( !lp->languagePackage.contains( "%" ) )
        {
            LanguagePackage l = *lp;
            l.locale = QObject::tr( "Global" );

            if ( availableLP.contains( l.languagePackage ) )
                availablePackages->append( l );
            else if ( installedLP.contains( l.languagePackage ) )
                installedPackages->append( l );

            continue;
        }

        for ( int i = 0; i < locales.size(); i++ )
        {
            LocaleSplit* locale = &locales[i];

            checkLP.clear();

            // Example: firefox-i18n-% -> firefox-i18n-en-US
            checkLP.append( QString( lp->languagePackage ).replace( "%", QString( "%1-%2" ).arg( locale->language.toLower(), locale->territory ) ) );

            // Example: firefox-i18n-% -> firefox-i18n-en-us
            checkLP.append( QString( lp->languagePackage ).replace( "%", QString( "%1-%2" ).arg( locale->language.toLower(), locale->territory.toLower() ) ) );

            // Example: firefox-i18n-% -> firefox-i18n-en_US
            checkLP.append( QString( lp->languagePackage ).replace( "%", QString( "%1_%2" ).arg( locale->language.toLower(), locale->territory ) ) );

            // Example: firefox-i18n-% -> firefox-i18n-en_us
            checkLP.append( QString( lp->languagePackage ).replace( "%", QString( "%1_%2" ).arg( locale->language.toLower(), locale->territory.toLower() ) ) );

            // Example: firefox-i18n-% -> firefox-i18n-en
            checkLP.append( QString( lp->languagePackage ).replace( "%", locale->language.toLower() ) );


            foreach ( QString alp, availableLP )
            {
                if ( !checkLP.contains( alp ) )
                    continue;

                LanguagePackage availableLanguagePackage;
                availableLanguagePackage.languagePackage = alp;
                availableLanguagePackage.parentPackage = lp->parentPackage;
                availableLanguagePackage.locale = locale->language + "_" + locale->territory;
                availablePackages->append( availableLanguagePackage );
                break;
            }

            foreach ( QString alp, installedLP )
            {
                if ( !checkLP.contains( alp ) )
                    continue;

                LanguagePackage installedLanguagePackage;
                installedLanguagePackage.languagePackage = alp;
                installedLanguagePackage.parentPackage = lp->parentPackage;
                installedLanguagePackage.locale = locale->language + "_" + locale->territory;
                installedPackages->append( installedLanguagePackage );
                break;
            }
        }
    }

    return true;
}


QStringList
LanguagePackagesCommon::getAllInstalledPackages( const QStringList& checkPackages )
{
    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert( "LANG", "C" );
    env.insert( "LC_MESSAGES", "C" );
    process.setProcessEnvironment( env );
    process.start( "pacman", QStringList() << "-Qq" );
    if ( !process.waitForFinished() )
    {
        qDebug() << "error: failed to get installed packages (pacman)!";
        return QStringList();
    }

    if ( process.exitCode() != 0 )
    {
        qDebug() << "error: failed to get installed packages (pacman)!";
        return QStringList();
    }

    QStringList output = QString( process.readAll() ).split( "\n", QString::SkipEmptyParts );

    QStringList result;
    for ( QString package : checkPackages )
    {
        if ( output.contains( package ) )
            result.append( package );
    }

    return result;
}


QStringList
LanguagePackagesCommon::getAllAvailableRepoPackages( const QStringList& checkPackages )
{
    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert( "LANG", "C" );
    env.insert( "LC_MESSAGES", "C" );
    process.setProcessEnvironment( env );
    process.start( "pacman", QStringList() << "-Si" << checkPackages );

    if ( !process.waitForFinished() )
    {
        qDebug() << "error: failed to get informations about available packages (pacman)!";
        return QStringList();
    }

    QStringList output = QString( process.readAll() ).split( "\n", QString::SkipEmptyParts );

    QStringList result;
    for ( QString line : output )
    {
        line = line.remove( " " ).remove( "\t" );

        if ( !line.toLower().startsWith( "name:" ) )
            continue;

        line = line.mid( line.indexOf( ":" ) + 1 );

        if ( checkPackages.contains( line ) )
            result.append( line );
    }

    return result;
}


QList<LanguagePackagesCommon::LocaleSplit>
LanguagePackagesCommon::getAllEnabledLocalesSplit()
{
    QStringList localesList { LanguageCommon::enabledLocales( true ) };
    QList<LanguagePackagesCommon::LocaleSplit> locales;
    for ( const QString locale : localesList )
    {
        QStringList split = locale.split( "_", QString::SkipEmptyParts );
        if ( split.size() != 2 )
            continue;

        LocaleSplit lc;
        lc.language = split.at( 0 );
        lc.territory = split.at( 1 );

        locales.append( lc );
    }
    return locales;
}


bool
LanguagePackagesCommon::isSystemUpToDate()
{
    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert( "LANG", "C" );
    env.insert( "LC_MESSAGES", "C" );
    process.setProcessEnvironment( env );
    process.start( "pacman", QStringList() << "-Sup" );
    if ( !process.waitForFinished() )
    {
        qDebug() << "error: failed to determine if system is up-to-date (pacman)!";
        return false;
    }

    return QString( process.readAll() ).split( "\n", QString::SkipEmptyParts ) ==
           ( QStringList() << ":: Starting full system upgrade..." );
}
