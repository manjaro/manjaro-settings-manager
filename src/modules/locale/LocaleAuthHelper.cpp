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

#include "LocaleAuthHelper.h"
#include "LanguageCommon.h"

#include <QtDBus/QDBusInterface>
#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtCore/QTextStream>
#include <QtCore/QSet>

#include <QDebug>

ActionReply
LocaleAuthHelper::save( const QVariantMap& args )
{
    if ( args["isLocaleListModified"].toBool() )
    {
        updateLocaleGen( args["locales"].toStringList() );
        generateLocaleGen();
    }
    if ( args["isSystemLocalesModified"].toBool() )
        setSystemLocale( args["localeList"].toStringList() );
    return ActionReply::SuccessReply();
}


bool
LocaleAuthHelper::updateLocaleGen( QStringList locales )
{

    const QString localeGen = "/etc/locale.gen";
    QFile file( localeGen );
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug() << QString( "error: failed to open file '%1'" ).arg( localeGen );
        return false;
    }

    QStringList content;

    QTextStream in( &file );
    while ( !in.atEnd() )
    {
        QString line = in.readLine();
        content.append( line );
        line = line.trimmed();

        bool found = false;

        foreach ( const QString locale, locales )
        {
            if ( line.startsWith( locale + " " ) )
            {
                found = true;
                locales.removeAll( locale );
                break;
            }

            if ( line.startsWith( "#" + locale + " " ) )
            {
                content.removeLast();
                content.append( line.remove( 0, 1 ) );
                found = true;
                locales.removeAll( locale );
                break;
            }
        }

        if ( !found && !line.split( "#", QString::KeepEmptyParts ).first()
                .trimmed().isEmpty() )
        {
            content.removeLast();
            content.append( "#" + line );
        }
    }
    file.close();

    // Add missing locales in the file
    foreach ( const QString locale, locales )
    {
        QString validLocale = LanguageCommon::localeToLocaleGenFormat( locale );
        if ( validLocale.isEmpty() )
        {
            qDebug() << QString( "warning: failed to obtain valid locale string"
                                 "for locale '%1'!" ).arg( locale );
            continue;
        }
        content.append( validLocale );
    }

    if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
        qDebug() << QString( "error: failed to open file '%1'!" ).arg( localeGen );
        return false;
    }
    QTextStream out( &file );
    out << content.join( "\n" );
    out.flush();
    file.close();

    return true;
}


bool
LocaleAuthHelper::generateLocaleGen()
{
    QProcess localeGen;
    localeGen.start( "/usr/bin/locale-gen" );
    connect( &localeGen, &QProcess::readyRead,
             [&] ()
    {
        QString data = QString::fromUtf8( localeGen.readAll() ).trimmed();
        if ( !data.isEmpty() )
        {
            QVariantMap map;
            map.insert( QLatin1String( "Data" ), data );
            HelperSupport::progressStep( map );
        }
    } );
    if ( !localeGen.waitForStarted() )
        return false;
    if ( !localeGen.waitForFinished( 60000 ) )
        return false;
    return true;
}


bool
LocaleAuthHelper::setSystemLocale( const QStringList localeList )
{
    QDBusInterface dbusInterface( "org.freedesktop.locale1",
                                  "/org/freedesktop/locale1",
                                  "org.freedesktop.locale1",
                                  QDBusConnection::systemBus() );
    /*
     * asb
     * array_string -> locale
     * boolean -> arg_ask_password
     */
    QDBusMessage reply;
    reply = dbusInterface.call( "SetLocale", localeList, true );
    if ( reply.type() == QDBusMessage::ErrorMessage )
        return false;
    return true;
}


KAUTH_HELPER_MAIN( "org.manjaro.msm.locale", LocaleAuthHelper )
#include "moc_LocaleAuthHelper.cpp"
