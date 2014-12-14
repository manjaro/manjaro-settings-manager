#include "MhwdAuthHelper.h"

#include <QDebug>


ActionReply MhwdAuthHelper::install(const QVariantMap& args)
{
    return ActionReply::SuccessReply();
}


ActionReply MhwdAuthHelper::remove(const QVariantMap& args)
{
    return ActionReply::SuccessReply();
}

KAUTH_HELPER_MAIN("org.manjaro.msm.mhwd", MhwdAuthHelper)
#include "moc_MhwdAuthHelper.cpp"
