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

#ifndef TIMEDATESERVICE_H
#define TIMEDATESERVICE_H

#include <QtCore/QDateTime>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringListModel>
#include <QtDBus/QDBusInterface>

class TimeDateService : public QObject
{
    Q_OBJECT
public:
    explicit TimeDateService( QObject* parent = 0 );
    ~TimeDateService();

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
    QDBusInterface* m_dbusInterface;
};

#endif // TIMEDATESERVICE_H
