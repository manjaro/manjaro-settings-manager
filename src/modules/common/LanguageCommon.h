#ifndef LANGUAGECOMMON_H
#define LANGUAGECOMMON_H

#include <QtCore/QStringList>

class LanguageCommon
{
public:
    static QStringList enabledLocales();
    static QStringList supportedLocales();
};

#endif // LANGUAGECOMMON_H
