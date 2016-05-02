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

#include "PacmanUtils.h"

#include <QtCore/QFile>
#include <QtCore/QProcessEnvironment>
#include <QtCore/QStringList>

#include <QDebug>


bool
PacmanUtils::hasPacmanEverSynced()
{
    QString path( "/var/lib/pacman/sync/" );
    QStringList files = QStringList() << "core.db" << "community.db" << "extra.db";
    foreach ( QString f, files )
    {
        if ( !QFile::exists( path + f ) )
            return false;
    }
    return true;
}


bool
PacmanUtils::isPacmanUpdating()
{
    return QFile::exists( "/var/lib/pacman/db.lck" );
}


bool
PacmanUtils::isSystemUpToDate()
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
