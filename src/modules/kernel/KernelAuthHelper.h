#ifndef KERNELAUTHHELPER_H
#define KERNELAUTHHELPER_H

#include <KAuth>
 
using namespace KAuth;

class KernelAuthHelper : public QObject
{
  Q_OBJECT
  
  public Q_SLOTS:
  ActionReply install(const QVariantMap& args);
  ActionReply remove(const QVariantMap& args);
};

#endif // KERNELAUTHHELPER_H
