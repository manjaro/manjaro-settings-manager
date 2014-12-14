#ifndef LOCALEAUTHHELPER_H
#define LOCALEAUTHHELPER_H

#include <KAuth>
 
using namespace KAuth;

class LocaleAuthHelper : public QObject
{
  Q_OBJECT
  
  public Q_SLOTS:
  ActionReply save(const QVariantMap& args);
};

#endif // LOCALEAUTHHELPER_H
