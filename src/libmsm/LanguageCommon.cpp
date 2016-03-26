/*
 *  Manjaro Settings Manager
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

#include "LanguageCommon.h"

#include <QtCore/QRegularExpression>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QDebug>

QStringList
LanguageCommon::enabledLocales( bool clean )
{
    QStringList locales;
    QFile localeGen( "/etc/locale.gen" );
    if ( !localeGen.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug() << "error: failed to open '/etc/locale.gen'";
        return QStringList();
    }

    QTextStream in( &localeGen );
    while ( !in.atEnd() )
    {
        QString line = in.readLine();
        if ( line.isEmpty() || line.startsWith( "#" ) )
            continue;
        line = QString( line.split( " ", QString::SkipEmptyParts )
                        .first()
                        .trimmed() );
        // Remove .UTF-8, @euro ...
        if ( clean )
        {
            line = line.split( QRegularExpression( "[ .@]" ),
                               QString::SkipEmptyParts )
                   .first();
        }
        locales << line;
    }
    localeGen.close();
    return locales;
}


QStringList
LanguageCommon::supportedLocales( bool clean )
{
    QFile localeGen( "/etc/locale.gen" );
    QString lines;
    if ( localeGen.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        QTextStream in( &localeGen );
        lines.append( in.readAll() );
    }
    else
        qDebug() << "error: failed to open '/etc/locale.gen'";

    QFile localeGenPacnew( "/etc/locale.gen.pacnew" );
    if ( localeGenPacnew.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        QTextStream in( &localeGenPacnew );
        lines.append( in.readAll() );
    }

    QSet<QString> localeList;
    for ( QString line : lines.split( '\n' ) )
    {
        if ( line.startsWith( "# " ) || line.simplified() == "#" ||
                line.isEmpty() )
            continue;

        line = line.simplified();
        if ( line.startsWith( "#" ) )
            line.remove( '#' );

        line = line.split( " ", QString::SkipEmptyParts )
               .first()
               .trimmed();
        // Remove .UTF-8, @euro ...
        if ( clean )
        {
            line = line.split( QRegularExpression( "[ .@]" ),
                               QString::SkipEmptyParts )
                   .first();
        }
        localeList.insert( line );
    }
    return localeList.toList();
}


QString
LanguageCommon::localeToLocaleGenFormat( const QString locale )
{
    QSet<QString> localeList;

    QFile localeGen( "/etc/locale.gen" );
    QString lines;
    if ( localeGen.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        QTextStream in( &localeGen );
        lines.append( in.readAll() );
    }

    QFile localeGenPacnew( "/etc/locale.gen.pacnew" );
    if ( localeGenPacnew.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        QTextStream in( &localeGenPacnew );
        lines.append( in.readAll() );
    }

    for ( const QString line : lines.split( '\n' ) )
    {
        if ( line.startsWith( "# " ) || line.simplified() == "#"
                || line.isEmpty() )
            continue;

        if ( line.simplified().startsWith( "#" ) )
            localeList.insert( line.simplified().remove( '#' ) );
        else
            localeList.insert( line.simplified() );
    }

    for ( const QString line : localeList )
    {
        if ( line.startsWith( locale + " " ) )
            return line;
    }

    return "";
}
