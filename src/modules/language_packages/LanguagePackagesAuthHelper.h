#ifndef LANGUAGEPACKAGESAUTHHELPER_H
#define LANGUAGEPACKAGESAUTHHELPER_H

#include <KAuth>
 
using namespace KAuth;

class LanguagePackagesAuthHelper : public QObject
{
  Q_OBJECT
  
  public Q_SLOTS:
  ActionReply install(const QVariantMap& args);
  ActionReply remove(const QVariantMap& args);
};

#endif // LANGUAGEPACKAGESAUTHHELPER_H
