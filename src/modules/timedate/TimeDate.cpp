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

#include "TimeDate.h"

#include <QtCore/QDebug>

TimeDate::TimeDate( QObject* parent ) :
    QObject( parent )
{
    m_dbusInterface = new QDBusInterface( "org.freedesktop.timedate1",
                                         "/org/freedesktop/timedate1",
                                         "org.freedesktop.timedate1",
                                         QDBusConnection::systemBus(),
                                         0 );
}

TimeDate::~TimeDate()
{
    delete m_dbusInterface;
}


QDateTime
TimeDate::localDateTime()
{
    return QDateTime::fromMSecsSinceEpoch( m_dbusInterface->property( "TimeUSec" ).toLongLong() / 1000 );
}


QDateTime
TimeDate::utcDateTime()
{
    QDateTime aux;
    aux.setMSecsSinceEpoch( ( m_dbusInterface->property( "TimeUSec" ) ).toLongLong() / 1000 );
    aux.setTimeSpec( Qt::LocalTime );
    return aux.toUTC();
}


QDateTime
TimeDate::rtcDateTime()
{
    QDateTime aux;
    aux.setMSecsSinceEpoch( ( m_dbusInterface->property( "RTCTimeUSec" ) ).toLongLong() / 1000 );
    aux.setTimeSpec( Qt::LocalTime );
    return aux.toUTC();
}


QString
TimeDate::timeZone()
{
    return m_dbusInterface->property( "Timezone" ).toString();
}


bool
TimeDate::canNtp()
{
    return m_dbusInterface->property( "CanNTP" ).toBool();
}


bool
TimeDate::isNtpEnabled()
{
    return m_dbusInterface->property( "NTP" ).toBool();
}


bool
TimeDate::isNtpSynchronized()
{
    return m_dbusInterface->property( "NTPSynchronized" ).toBool();
}


bool
TimeDate::isRtcInLocalTimeZone()
{
    return m_dbusInterface->property( "LocalRTC" ).toBool();
}


void
TimeDate::setTime( const QDateTime& time )
{
    /*
     * xbb
     * int64_t -> time_t
     * boolean -> relative
     * boolean -> arg_ask_password
     */
    qint64 timeUSec = time.toMSecsSinceEpoch() * 1000;
    QDBusMessage reply;
    reply = m_dbusInterface->call( "SetTime", timeUSec, false, true );
    qDebug() << reply;
}


void
TimeDate::setTimeZone( const QString& timeZone )
{
    /*
     * sb
     * string -> timezone id
     * boolean -> arg_ask_password
     */
    QDBusMessage reply;
    reply = m_dbusInterface->call( "SetTimezone", timeZone, true );
    qDebug() << reply;
}


void
TimeDate::setLocalRtc( const bool local )
{
    /*
     * bbb
     * boolean -> local rtc
     * boolean -> adjust_system_clock
     * boolean -> arg_ask_password
     */
    QDBusMessage reply;
    reply = m_dbusInterface->call( "SetLocalRTC", local, false, true );
    qDebug() << reply;
}


void
TimeDate::setNtp( const bool ntp )
{
    /*
     * bb
     * boolean -> ntp
     * boolean -> arg_ask_password
     */
    QDBusMessage reply;
    reply = m_dbusInterface->call( "SetNTP", ntp, true );
    qDebug() << reply;
}
