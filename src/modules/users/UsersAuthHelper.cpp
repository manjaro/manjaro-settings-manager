#include "UsersAuthHelper.h"

#include <QDebug>


ActionReply UsersAuthHelper::add(const QVariantMap& args)
{
    return ActionReply::SuccessReply();
}

ActionReply UsersAuthHelper::remove(const QVariantMap& args)
{
    return ActionReply::SuccessReply();
}

ActionReply UsersAuthHelper::changePassword(const QVariantMap &args)
{
    return ActionReply::SuccessReply();
}

ActionReply UsersAuthHelper::changeAccountType(const QVariantMap &args)
{
    return ActionReply::SuccessReply();
}

ActionReply UsersAuthHelper::changeImage(const QVariantMap &args)
{
    return ActionReply::SuccessReply();
}

KAUTH_HELPER_MAIN("org.manjaro.msm.users", UsersAuthHelper)
#include "moc_UsersAuthHelper.cpp"
