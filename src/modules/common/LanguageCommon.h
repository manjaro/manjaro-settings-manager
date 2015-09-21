#ifndef LANGUAGECOMMON_H
#define LANGUAGECOMMON_H

#include <QtCore/QStringList>

class LanguageCommon
{
public:
    static QStringList enabledLocales(bool clean = false);
    static QStringList supportedLocales(bool clean = false);
};

#endif // LANGUAGECOMMON_H
