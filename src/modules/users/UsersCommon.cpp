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

#include "UsersCommon.h"

#include <QtCore/QProcessEnvironment>
#include <QtNetwork/QNetworkInterface>

int
UsersCommon::runProcess( QString cmd, QStringList args, QStringList writeArgs, QString& error )
{
    QProcess process;
    process.setProcessChannelMode( QProcess::MergedChannels );
    process.start( cmd, args );

    if ( !process.waitForStarted( 5000 ) )
        return -1;

    foreach ( QString arg, writeArgs )
        process.write( QString( arg + "\n" ).toUtf8() );

    process.closeWriteChannel();

    if ( !process.waitForFinished( 15000 ) )
        return -1;

    error = QString::fromUtf8( process.readAll() );
    return process.exitCode();
}


QList<UsersCommon::User>
UsersCommon::getAllUsers()
{
    QList<UsersCommon::User> users;

    const QString passwdFilePath { "/etc/passwd" };
    QFile file( passwdFilePath );
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        return users;

    QTextStream in( &file );

    while ( !in.atEnd() )
    {
        QStringList split = in.readLine().split( "#", QString::KeepEmptyParts ).first().split( ":", QString::KeepEmptyParts );
        if ( split.size() < 7 )
            continue;

        User user;
        user.username = split.at( 0 );
        user.homePath = split.at( 5 );
        user.uuid = split.at( 2 ).toInt();

        const int minUserUuid { 1000 };
        if ( user.uuid < minUserUuid || user.username.isEmpty() || user.homePath.isEmpty() )
            continue;

        users.append( user );
    }

    file.close();

    return users;
}


QList<UsersCommon::Group>
UsersCommon::getAllGroups()
{
    QList<UsersCommon::Group> groups;

    const QString groupconfFilePath { "/etc/group" };
    QFile file( groupconfFilePath  );
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        return groups;

    QTextStream in( &file );

    while ( !in.atEnd() )
    {
        QStringList split = in.readLine().split( "#", QString::KeepEmptyParts ).first().split( ":", QString::KeepEmptyParts );
        if ( split.size() < 4 )
            continue;

        Group group;
        group.name = split.at( 0 );
        group.members = split.at( 3 ).split( ",", QString::SkipEmptyParts );

        if ( group.name.isEmpty() )
            continue;

        groups.append( group );
    }

    file.close();

    return groups;
}
