#ifndef MHWDAUTHHELPER_H
#define MHWDAUTHHELPER_H

#include <KAuth>
 
using namespace KAuth;

class MhwdAuthHelper : public QObject
{
  Q_OBJECT
  
  public Q_SLOTS:
  ActionReply install(const QVariantMap& args);
  ActionReply remove(const QVariantMap& args);
};

#endif // MHWDAUTHHELPER_H
