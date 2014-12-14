#include "LocaleAuthHelper.h"

#include <QtCore/QProcess>
#include <QDebug>
#include <unistd.h>


ActionReply LocaleAuthHelper::save(const QVariantMap& args)
{
    return ActionReply::SuccessReply();
}

KAUTH_HELPER_MAIN("org.manjaro.msm.locale", LocaleAuthHelper)
#include "moc_LocaleAuthHelper.cpp"
