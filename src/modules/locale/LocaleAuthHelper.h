#ifndef LOCALEAUTHHELPER_H
#define LOCALEAUTHHELPER_H

#include <KAuth>

using namespace KAuth;

class LocaleAuthHelper : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    ActionReply save(const QVariantMap& args);

private:
    bool updateLocaleGen(const QStringList locales);
    bool generateLocaleGen();
    bool setSystemLocale(const QStringList locale);
    QString localeToValidLocaleGenString(const QString locale);
};

#endif // LOCALEAUTHHELPER_H
