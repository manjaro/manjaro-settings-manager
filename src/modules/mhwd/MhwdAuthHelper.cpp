#include "MhwdAuthHelper.h"

#include <QtCore/QProcess>


ActionReply MhwdAuthHelper::install(const QVariantMap& args)
{
    QProcess *pacman = new QProcess();
    pacman->start("/usr/bin/mhwd", args["arguments"].toStringList());
    connect(pacman, &QProcess::readyRead,
            [=] ()
    {
        QString data = QString::fromUtf8(pacman->readAll()).trimmed();
        if (!data.isEmpty()) {
            QVariantMap map;
            map.insert(QLatin1String("Data"), data);
            HelperSupport::progressStep(map);
        }
    });
    pacman->waitForStarted();
    pacman->waitForFinished(-1);
    return ActionReply::SuccessReply();
}


ActionReply MhwdAuthHelper::remove(const QVariantMap& args)
{
    QProcess *pacman = new QProcess();
    pacman->start("/usr/bin/mhwd", args["arguments"].toStringList());
    connect(pacman, &QProcess::readyRead,
            [=] ()
    {
        QString data = QString::fromUtf8(pacman->readAll()).trimmed();
        if (!data.isEmpty()) {
            QVariantMap map;
            map.insert(QLatin1String("Data"), data);
            HelperSupport::progressStep(map);
        }
    });
    pacman->waitForStarted();
    pacman->waitForFinished(-1);
    return ActionReply::SuccessReply();
}

KAUTH_HELPER_MAIN("org.manjaro.msm.mhwd", MhwdAuthHelper)
#include "moc_MhwdAuthHelper.cpp"
