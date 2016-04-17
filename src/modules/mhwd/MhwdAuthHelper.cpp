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

#include "MhwdAuthHelper.h"

#include <QtCore/QProcess>


ActionReply
MhwdAuthHelper::install( const QVariantMap& args )
{
    QProcess mhwd;
    mhwd.start( "/usr/bin/mhwd", args["arguments"].toStringList() );
    connect( &mhwd, &QProcess::readyRead,
             [&] ()
    {
        QString data = QString::fromUtf8( mhwd.readAll() ).trimmed();
        if ( !data.isEmpty() )
        {
            QVariantMap map;
            map.insert( QLatin1String( "Data" ), data );
            HelperSupport::progressStep( map );
        }
    } );

    if ( !mhwd.waitForStarted() )
        return ActionReply::HelperErrorReply();

    if ( !mhwd.waitForFinished( 600000 ) )
        return ActionReply::HelperErrorReply();

    return ActionReply::SuccessReply();
}


ActionReply
MhwdAuthHelper::remove( const QVariantMap& args )
{
    return install( args );
}

KAUTH_HELPER_MAIN( "org.manjaro.msm.mhwd", MhwdAuthHelper )
#include "moc_MhwdAuthHelper.cpp"
