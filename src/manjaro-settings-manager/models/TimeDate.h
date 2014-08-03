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
    explicit TimeDate(QObject *parent = 0);
    ~TimeDate();

    QDateTime localDateTime();
    QDateTime utcDateTime();
    QDateTime rtcDateTime();
    QString timeZone();
    bool canNtp();
    bool isNtpEnabled();
    bool isNtpSynchronized();
    bool isRtcInLocalTimeZone();

    void setTime(const QDateTime &time);
    void setTimeZone(const QString &timeZone);
    void setLocalRtc(const bool local);
    void setNtp(const bool ntp);

signals:

public slots:

private:
    QStringListModel timeZonesList_;
    QDBusInterface *dbusInterface_;
};

#endif // TIMEDATE_H
