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

#ifndef TIMEDATE_H
#define TIMEDATE_H

#include <QtCore/QDateTime>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringListModel>
#include <QtDBus/QDBusInterface>


class TimeDate : public QObject
{
    Q_OBJECT
public:
    explicit TimeDate( QObject* parent = 0 );
    ~TimeDate();

    QDateTime localDateTime();
    QDateTime utcDateTime();
    QDateTime rtcDateTime();
    QString timeZone();
    bool canNtp();
    bool isNtpEnabled();
    bool isNtpSynchronized();
    bool isRtcInLocalTimeZone();

    void setTime( const QDateTime& time );
    void setTimeZone( const QString& timeZone );
    void setLocalRtc( const bool local );
    void setNtp( const bool ntp );

private:
    QDBusInterface* dbusInterface_;
};

#endif // TIMEDATE_H
