#include "KernelAuthHelper.h"

#include <QtCore/QProcess>


ActionReply KernelAuthHelper::install(const QVariantMap& args)
{
    QProcess *pacman = new QProcess();
    pacman->start("/usr/bin/pacman", args["arguments"].toStringList());
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


ActionReply KernelAuthHelper::remove(const QVariantMap& args)
{
    QProcess *pacman = new QProcess();
    pacman->start("/usr/bin/pacman", args["arguments"].toStringList());
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

KAUTH_HELPER_MAIN("org.manjaro.msm.kernel", KernelAuthHelper)
#include "moc_KernelAuthHelper.cpp"
