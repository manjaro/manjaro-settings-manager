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

#include "LanguageCommon.h"

#include <QtCore/QFile>
#include <QtCore/QProcessEnvironment>
#include <QtCore/QRegularExpression>
#include <QtCore/QStringList>
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

    QByteArray in = localeGen.readAll();
    foreach ( QByteArray line, in.split( '\n' ) )
    {
        line = line.simplified();

        if ( line.startsWith( "#" ) ||
                line.isEmpty() )
            continue;

        // Remove UTF-8, ISO-8895-15, etc
        QString lineString = QString::fromUtf8( line );
        lineString =  lineString.split( " ", QString::SkipEmptyParts )
                      .first()
                      .trimmed();

        if ( clean )
        {
            // Remove .UTF-8, @euro ...
            locales << lineString.split( QRegularExpression( "[ .@]" ),
                                         QString::SkipEmptyParts )
                    .first()
                    .trimmed();
        }
        else
            locales << lineString;
    }

    locales.removeDuplicates();
    return locales;
}


QStringList
LanguageCommon::supportedLocales( bool clean )
{
    QFile localeGen( "/etc/locale.gen" );
    QByteArray in;
    if ( localeGen.open( QIODevice::ReadOnly | QIODevice::Text ) )
        in.append( localeGen.readAll() );
    else
        qDebug() << "error: failed to open '/etc/locale.gen'";

    QFile localeGenPacnew( "/etc/locale.gen.pacnew" );
    if ( localeGenPacnew.open( QIODevice::ReadOnly | QIODevice::Text ) )
        in.append( localeGen.readAll() );

    QStringList locales;
    foreach ( QByteArray line, in.split( '\n' ) )
    {
        line = line.simplified();

        if ( line.startsWith( "# " ) ||
                line == "#" ||
                line.isEmpty() )
            continue;

        qDebug() << line;

        if ( line.startsWith( "#" ) )
            line.remove( 0, 1 );

        qDebug() << line;

        // Remove UTF-8, ISO-8895-15, etc
        QString lineString = QString::fromUtf8( line );
        lineString = lineString.split( " ", QString::SkipEmptyParts )
                     .first()
                     .trimmed();

        qDebug() << lineString;

        if ( clean )
        {
            // Remove .UTF-8, @euro ...
            locales << lineString.split( QRegularExpression( "[ .@]" ),
                                         QString::SkipEmptyParts )
                    .first()
                    .trimmed();
        }
        else
            locales << lineString;
    }

    locales.removeDuplicates();
    return locales;
}


QString
LanguageCommon::localeToLocaleGenFormat( const QString locale )
{
    QSet<QByteArray> localeList;

    QFile localeGen( "/etc/locale.gen" );
    QByteArray in;
    if ( localeGen.open( QIODevice::ReadOnly | QIODevice::Text ) )
        in.append( localeGen.readAll() );
    else
        qDebug() << "error: failed to open '/etc/locale.gen'";

    QFile localeGenPacnew( "/etc/locale.gen.pacnew" );
    if ( localeGenPacnew.open( QIODevice::ReadOnly | QIODevice::Text ) )
        in.append( localeGen.readAll() );

    foreach ( QByteArray line, in.split( '\n' ) )
    {
        line = line.simplified();

        if ( line.startsWith( "# " ) ||
                line == "#"||
                line.isEmpty() )
            continue;

        if ( line.startsWith( "#" ) )
            line.remove( 0, 1 );

        if ( locale.toUtf8() == line )
            return line;
    }

    return QString();
}
