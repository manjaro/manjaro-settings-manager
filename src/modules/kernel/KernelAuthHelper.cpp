/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Ramon Buldó <ramon@manjaro.org>
 *  Kacper Piwiński
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

#include "KernelAuthHelper.h"

#include <QtCore/QProcess>

ActionReply
KernelAuthHelper::install( const QVariantMap& args )
{
    return runPacman( args );
}


ActionReply
KernelAuthHelper::remove( const QVariantMap& args )
{
    return runPacman( args );
}


ActionReply
KernelAuthHelper::runPacman( const QVariantMap& args )
{
    QProcess* pacman = new QProcess();
    pacman->start( "/usr/bin/pacman", args["arguments"].toStringList() );
    actionReply = ActionReply::SuccessType;
    connect( pacman, &QProcess::readyRead,
             [&] ()
    {
        QString data = QString::fromUtf8( pacman->readAllStandardOutput() ).trimmed();
        QString dataErr = QString::fromUtf8( pacman->readAllStandardError() ).trimmed();
        if ( !dataErr.isEmpty() )
        {
            actionReply = ActionReply::HelperErrorType;
            data.append( "\n" ).append( dataErr );
        }
        if ( !data.isEmpty() )
        {
            QVariantMap map;
            map.insert( QLatin1String( "Data" ), data );
            HelperSupport::progressStep( map );
        }
    } );
    pacman->waitForStarted();
    pacman->waitForFinished( -1 );
    actionReply.setError( pacman->exitCode() );
    return actionReply;
}

KAUTH_HELPER_MAIN( "org.manjaro.msm.kernel", KernelAuthHelper )
#include "moc_KernelAuthHelper.cpp"
