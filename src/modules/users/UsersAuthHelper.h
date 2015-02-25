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
  ActionReply changepassword(const QVariantMap& args);
  ActionReply changeaccounttype(const QVariantMap& args);
  ActionReply changeimage(const QVariantMap& args);
};

#endif // USERSAUTHHELPER_H
