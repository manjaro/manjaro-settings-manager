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

#include "LanguagePackagesAuthHelper.h"

#include <QtCore/QProcess>

ActionReply
LanguagePackagesAuthHelper::install( const QVariantMap& args )
{
    QProcess* pacman = new QProcess();
    pacman->start( "/usr/bin/pacman", args["arguments"].toStringList() );
    connect( pacman, &QProcess::readyRead,
             [=] ()
    {
        QString data = QString::fromUtf8( pacman->readAll() ).trimmed();
        if ( !data.isEmpty() )
        {
            QVariantMap map;
            map.insert( QLatin1String( "Data" ), data );
            HelperSupport::progressStep( map );
        }
    } );
    pacman->waitForStarted();
    pacman->waitForFinished( -1 );
    return ActionReply::SuccessReply();
}


ActionReply
LanguagePackagesAuthHelper::remove( const QVariantMap& args )
{
    QProcess* pacman = new QProcess();
    pacman->start( "/usr/bin/pacman", args["arguments"].toStringList() );
    connect( pacman, &QProcess::readyRead,
             [=] ()
    {
        QString data = QString::fromUtf8( pacman->readAll() ).trimmed();
        if ( !data.isEmpty() )
        {
            QVariantMap map;
            map.insert( QLatin1String( "Data" ), data );
            HelperSupport::progressStep( map );
        }
    } );
    pacman->waitForStarted();
    pacman->waitForFinished( -1 );
    return ActionReply::SuccessReply();
}

KAUTH_HELPER_MAIN( "org.manjaro.msm.languagepackages", LanguagePackagesAuthHelper )
#include "moc_LanguagePackagesAuthHelper.cpp"
