#ifndef USERSAUTHHELPER_H
#define USERSAUTHHELPER_H

#include <KAuth>
 
using namespace KAuth;

class UsersAuthHelper : public QObject
{
  Q_OBJECT
  
  public Q_SLOTS:
  ActionReply add(const QVariantMap& args);
  ActionReply remove(const QVariantMap& args);
  ActionReply changePassword(const QVariantMap& args);
  ActionReply changeAccountType(const QVariantMap& args);
  ActionReply changeImage(const QVariantMap& args);
};

#endif // USERSAUTHHELPER_H
