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

#include "LanguagePackages.h"
#include "LanguageCommon.h"

#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include <QDebug>

LanguagePackages::LanguagePackages( QObject* parent )
    : QObject( parent )
{
}

QList<LanguagePackagesItem>
LanguagePackages::languagePackages()
{
    QList<QByteArray> installedPkg { installedPackages() };
    QList<QByteArray> availablePkg { availablePackages() };

    QFile file;
    file.setFileName( ":/language_packages.json" );
    file.open( QIODevice::ReadOnly | QIODevice::Text );
    QJsonDocument jsonDocument = QJsonDocument::fromJson( file.readAll() );
    file.close();

    if ( !jsonDocument.isObject() )
    {
        qDebug() << "Cannot read 'language_packages.json' resource";
        return QList<LanguagePackagesItem>();
    }

    QJsonObject jsonObject = jsonDocument.object();
    QJsonValue packagesValue = jsonObject.value( QString( "Packages" ) );
    QList<QVariantMap> packages;
    if ( packagesValue.isArray() )
    {
        foreach ( const auto val, packagesValue.toArray() )
            packages.append( val.toObject().toVariantMap() );
    }

    QList<LanguagePackagesItem> lpiList;
    foreach ( QVariantMap package, packages )
    {
        QByteArray name { package["name"].toByteArray() };
        QByteArray languagePackage { package["l10n_package"].toByteArray() };
        QList<QByteArray> parentPackages;
        foreach ( const auto val, package["parent_packages"].toList() )
            parentPackages << val.toByteArray();
        QList<QByteArray> parentPkgInstalled { intersect( parentPackages, installedPkg ) };
        QList<QByteArray> languagePkgInstalled { filterLanguagePackage( languagePackage, installedPkg ) };
        QList<QByteArray> languagePkgAvailable { filterLanguagePackage( languagePackage, availablePkg ) };
        LanguagePackagesItem lpi
        {
            name,
            languagePackage,
            parentPackages,
            parentPkgInstalled,
            languagePkgInstalled,
            languagePkgAvailable
        };
        lpiList.append( lpi );
    }
    return lpiList;
}


const QList<QByteArray>
LanguagePackages::intersect( const QList<QByteArray>& packages1, const QList<QByteArray>& packages2 )
{
    QSet<QByteArray> intersection = packages1.toSet().intersect( packages2.toSet() );
    return intersection.toList();
}


const QList<QByteArray>
LanguagePackages::filterLanguagePackage( const QByteArray package, const QList<QByteArray>& packages )
{
    QByteArray pkg;
    if ( package.contains( "%" ) )
        pkg = package.left( package.indexOf( '%' ) );
    else
        pkg = package;
    QList<QByteArray> result;
    foreach ( const auto i, packages )
    {
        if ( i.startsWith( pkg ) )
            result.append( i );
    }

    return result;
}


const QList<QByteArray>
LanguagePackages::availablePackages()
{
    QProcess* process = new QProcess( this );
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert( "LANG", "C" );
    env.insert( "LC_MESSAGES", "C" );
    process->setProcessEnvironment( env );
    process->start( "pacman", QStringList() << "-Sl" );

    if ( !process->waitForFinished() )
    {
        qDebug() << "error: failed to get informations about available packages (pacman)!";
        return QList<QByteArray>();
    }

    QList<QByteArray> availablePackages;
    for ( QByteArray line : process->readAll().split( '\n' ) )
    {
        if ( line.isEmpty() )
            continue;

        line = line.mid( line.indexOf( " " ) + 1 );
        line = line.mid( 0, line.indexOf( " " ) );
        availablePackages.append( line );
    }
    return availablePackages;
}


const QList<QByteArray>
LanguagePackages::installedPackages()
{
    QProcess* process = new QProcess( this );
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert( "LANG", "C" );
    env.insert( "LC_MESSAGES", "C" );
    process->setProcessEnvironment( env );
    process->start( "pacman", QStringList() << "-Qq" );
    if ( !process->waitForFinished() )
    {
        qDebug() << "error: failed to get installed packages (pacman)!";
        return QList<QByteArray>();
    }

    if ( process->exitCode() != 0 )
    {
        qDebug() << "error: failed to get installed packages (pacman)!";
        return QList<QByteArray>();
    }

    QList<QByteArray> installedPackages;
    foreach ( const QByteArray line, process->readAll().split( '\n' ) )
    {
        if ( line.isEmpty() )
            continue;

        installedPackages.append( line );
    }

    return installedPackages;
}
