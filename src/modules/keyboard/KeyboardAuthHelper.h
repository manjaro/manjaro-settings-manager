#ifndef KEYBOARDAUTHHELPER_H
#define KEYBOARDAUTHHELPER_H

#include <KAuth>
 
using namespace KAuth;

class KeyboardAuthHelper : public QObject
{
  Q_OBJECT
  
  public Q_SLOTS:
  ActionReply save(const QVariantMap& args);
};

#endif // KEYBOARDAUTHHELPER_H
