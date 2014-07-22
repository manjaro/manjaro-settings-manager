#include "TimeDate.h"

#include <QDebug>

TimeDate::TimeDate(QObject *parent) :
    QObject(parent)
{
    dbusInterface_ = new QDBusInterface( "org.freedesktop.timedate1",
                                         "/org/freedesktop/timedate1",
                                         "org.freedesktop.timedate1",
                                         QDBusConnection::systemBus(),
                                         0 );
}

TimeDate::~TimeDate()
{
    delete dbusInterface_;
}

QDateTime TimeDate::localDateTime()
{
    return QDateTime::fromTime_t( (dbusInterface_->property("TimeUSec")).toString().left(10).toUInt() );
}

QDateTime TimeDate::utcDateTime()
{
    QDateTime aux;
    aux.setTime_t( (dbusInterface_->property("TimeUSec")).toString().left(10).toUInt() );
    aux.setTimeSpec(Qt::LocalTime);
    return aux.toUTC();
}

QDateTime TimeDate::rtcDateTime()
{
    return QDateTime::fromTime_t( (dbusInterface_->property("RTCTimeUSec")).toString().left(10).toUInt() );
}

QString TimeDate::timeZone()
{
    return dbusInterface_->property("Timezone").toString();
}

bool TimeDate::canNtp()
{
    return dbusInterface_->property("CanNTP").toBool();
}

bool TimeDate::isNtpEnabled()
{
    return dbusInterface_->property("NTP").toBool();
}

bool TimeDate::isNtpSynchronized()
{
    return dbusInterface_->property("NTPSynchronized").toBool();
}

bool TimeDate::isRtcInLocalTimeZone()
{
    return dbusInterface_->property("LocalRTC").toBool();
}

bool TimeDate::isDstActive()
{
    return false;
}

QDateTime TimeDate::lastDstChange()
{
    return QDateTime();
}

QDateTime TimeDate::nextDstChange()
{
    return QDateTime();
}

void TimeDate::updateTimeZonesList()
{

}

void TimeDate::setTime(const QDateTime &time)
{

}

void TimeDate::setTimeZone(const QString &timeZone)
{

}

void TimeDate::setLocalRtc(const bool &local)
{

}

void TimeDate::setNtp(const bool &ntp)
{

}
