#include "MhwdAuthHelper.h"

#include <QtCore/QProcess>


ActionReply MhwdAuthHelper::install(const QVariantMap& args)
{
    QProcess *mhwd = new QProcess();
    mhwd->start("/usr/bin/mhwd", args["arguments"].toStringList());
    connect(mhwd, &QProcess::readyRead,
            [=] ()
    {
        QString data = QString::fromUtf8(mhwd->readAll()).trimmed();
        if (!data.isEmpty()) {
            QVariantMap map;
            map.insert(QLatin1String("Data"), data);
            HelperSupport::progressStep(map);
        }
    });
    mhwd->waitForStarted();
    mhwd->waitForFinished(-1);
    return ActionReply::SuccessReply();
}


ActionReply MhwdAuthHelper::remove(const QVariantMap& args)
{
    QProcess *mhwd = new QProcess();
    mhwd->start("/usr/bin/mhwd", args["arguments"].toStringList());
    connect(mhwd, &QProcess::readyRead,
            [=] ()
    {
        QString data = QString::fromUtf8(mhwd->readAll()).trimmed();
        if (!data.isEmpty()) {
            QVariantMap map;
            map.insert(QLatin1String("Data"), data);
            HelperSupport::progressStep(map);
        }
    });
    mhwd->waitForStarted();
    mhwd->waitForFinished(-1);
    return ActionReply::SuccessReply();
}

KAUTH_HELPER_MAIN("org.manjaro.msm.mhwd", MhwdAuthHelper)
#include "moc_MhwdAuthHelper.cpp"
