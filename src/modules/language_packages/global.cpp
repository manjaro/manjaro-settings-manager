/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
 *  Ramon Buldó <ramon@manjaro.org>
 *
 *  Copyright (C) 2007 Free Software Foundation, Inc.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "global.h"

#include <QtCore/QProcessEnvironment>
#include <QtNetwork/QNetworkInterface>

bool Global::getLanguagePackages(QList<Global::LanguagePackage> *availablePackages, QList<Global::LanguagePackage> *installedPackages) {
    QList<Global::LanguagePackage> languagePackages;
    QStringList parentPackages, checkLP;
    QList<Global::LocaleSplit> locales = getAllEnabledLocalesSplit();

    if (locales.isEmpty()) {
        qDebug() << "error: didn't find any enabled locales!";
        return false;
    }

    QFile file(LANGUAGE_PACKAGES_FILE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "error: failed to open '" << LANGUAGE_PACKAGES_FILE << "'!";
        return false;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QStringList split = in.readLine().split("#", QString::KeepEmptyParts).first().split(":", QString::SkipEmptyParts);
        if (split.size() < 2)
            continue;

        LanguagePackage lp;
        lp.parentPackage = split.at(0).trimmed();
        lp.languagePackage = split.at(1).trimmed();

        if (!lp.parentPackage.isEmpty() && !lp.languagePackage.isEmpty()) {
            parentPackages.append(lp.parentPackage);
            languagePackages.append(lp);
        }
    }

    file.close();


    // Keep only installed parent packages
    parentPackages = getAllInstalledPackages(parentPackages);

    for (int i = 0; i < languagePackages.size(); i++) {
        LanguagePackage *lp = &languagePackages[i];

        if (!parentPackages.contains(lp->parentPackage))
            continue;

        // Check if it is a global language pack
        if (!lp->languagePackage.contains("%")) {
            checkLP.append(lp->languagePackage);
            continue;
        }

        for (int i = 0; i < locales.size(); i++) {
            LocaleSplit *locale = &locales[i];

            // Example: firefox-i18n-% -> firefox-i18n-en-US
            checkLP.append(QString(lp->languagePackage).replace("%", QString("%1-%2").arg(locale->language.toLower(), locale->territory)));

            // Example: firefox-i18n-% -> firefox-i18n-en-us
            checkLP.append(QString(lp->languagePackage).replace("%", QString("%1-%2").arg(locale->language.toLower(), locale->territory.toLower())));

            // Example: firefox-i18n-% -> firefox-i18n-en_US
            checkLP.append(QString(lp->languagePackage).replace("%", QString("%1_%2").arg(locale->language.toLower(), locale->territory)));

            // Example: firefox-i18n-% -> firefox-i18n-en_us
            checkLP.append(QString(lp->languagePackage).replace("%", QString("%1_%2").arg(locale->language.toLower(), locale->territory.toLower())));

            // Example: firefox-i18n-% -> firefox-i18n-en
            checkLP.append(QString(lp->languagePackage).replace("%", locale->language.toLower()));
        }
    }


    // Keep only valid language packages
    checkLP.removeDuplicates();
    checkLP = getAllAvailableRepoPackages(checkLP);

    // Sort installed and available packages
    QStringList availableLP, installedLP = getAllInstalledPackages(checkLP);

    foreach (QString lp, checkLP) {
        if (!installedLP.contains(lp))
            availableLP.append(lp);
    }

    availableLP.sort();
    installedLP.sort();

    // Now add them to availablePackages and installedPackages
    for (int i = 0; i < languagePackages.size(); i++) {
        LanguagePackage *lp = &languagePackages[i];

        if (!parentPackages.contains(lp->parentPackage))
            continue;

        // Check if it is a global language pack
        if (!lp->languagePackage.contains("%")) {
            LanguagePackage l = *lp;
            l.locale = QObject::tr("Global");

            if (availableLP.contains(l.languagePackage))
                availablePackages->append(l);
            else if (installedLP.contains(l.languagePackage))
                installedPackages->append(l);

            continue;
        }

        for (int i = 0; i < locales.size(); i++) {
            LocaleSplit *locale = &locales[i];

            checkLP.clear();

            // Example: firefox-i18n-% -> firefox-i18n-en-US
            checkLP.append(QString(lp->languagePackage).replace("%", QString("%1-%2").arg(locale->language.toLower(), locale->territory)));

            // Example: firefox-i18n-% -> firefox-i18n-en-us
            checkLP.append(QString(lp->languagePackage).replace("%", QString("%1-%2").arg(locale->language.toLower(), locale->territory.toLower())));

            // Example: firefox-i18n-% -> firefox-i18n-en_US
            checkLP.append(QString(lp->languagePackage).replace("%", QString("%1_%2").arg(locale->language.toLower(), locale->territory)));

            // Example: firefox-i18n-% -> firefox-i18n-en_us
            checkLP.append(QString(lp->languagePackage).replace("%", QString("%1_%2").arg(locale->language.toLower(), locale->territory.toLower())));

            // Example: firefox-i18n-% -> firefox-i18n-en
            checkLP.append(QString(lp->languagePackage).replace("%", locale->language.toLower()));


            foreach (QString alp, availableLP) {
                if (!checkLP.contains(alp))
                    continue;

                LanguagePackage availableLanguagePackage;
                availableLanguagePackage.languagePackage = alp;
                availableLanguagePackage.parentPackage = lp->parentPackage;
                availableLanguagePackage.locale = locale->language + "_" + locale->territory;
                availablePackages->append(availableLanguagePackage);
                break;
            }

            foreach (QString alp, installedLP) {
                if (!checkLP.contains(alp))
                    continue;

                LanguagePackage installedLanguagePackage;
                installedLanguagePackage.languagePackage = alp;
                installedLanguagePackage.parentPackage = lp->parentPackage;
                installedLanguagePackage.locale = locale->language + "_" + locale->territory;
                installedPackages->append(installedLanguagePackage);
                break;
            }
        }
    }

    return true;
}



bool Global::isSystemUpToDate() {
    QProcess process;
    process.setEnvironment(QStringList() << "LANG=C" << "LC_MESSAGES=C");
    process.start("pacman", QStringList() << "-Sup");
    if (!process.waitForFinished()) {
        qDebug() << "error: failed to determine if system is up-to-date (pacman)!";
        return false;
    }

    return QString(process.readAll()).split("\n", QString::SkipEmptyParts) ==
               (QStringList() << ":: Starting full system upgrade...");
}


QStringList Global::getAllInstalledPackages(const QStringList & checkPackages) {
    QStringList result;

    QProcess process;
    process.setEnvironment(QStringList() << "LANG=C" << "LC_MESSAGES=C");
    process.start("pacman", QStringList() << "-Qq");
    if (!process.waitForFinished()) {
        qDebug() << "error: failed to get installed packages (pacman)!";
        return result;
    }

    if (process.exitCode() != 0) {
        qDebug() << "error: failed to get installed packages (pacman)!";
        return result;
    }

    QStringList allPackages = QString(process.readAll()).split("\n", QString::SkipEmptyParts);

    foreach (QString package, checkPackages) {
        if (allPackages.contains(package))
            result.append(package);
    }

    return result;
}



QStringList Global::getAllAvailableRepoPackages(const QStringList & checkPackages) {
    QStringList result;

    QProcess process;
    process.setEnvironment(QStringList() << "LANG=C" << "LC_MESSAGES=C");
    process.start("pacman", QStringList() << "-Si" << checkPackages);
    if (!process.waitForFinished()) {
        qDebug() << "error: failed to get informations about available packages (pacman)!";
        return result;
    }

    QStringList output = QString(process.readAll()).split("\n", QString::SkipEmptyParts);

    foreach (QString line, output) {
        line = line.remove(" ").remove("\t");

        if (!line.toLower().startsWith("name:"))
            continue;

        line = line.mid(line.indexOf(":") + 1);

        if (checkPackages.contains(line))
            result.append(line);
    }

    return result;
}


QList<Global::LocaleSplit> Global::getAllEnabledLocalesSplit() {
    QStringList strLocales;
    QList<Global::LocaleSplit> locales;

    QFile file(LOCALE_GEN);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "error: failed to open '" << LOCALE_GEN << "'!";
        return locales;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().split("#", QString::KeepEmptyParts).first().trimmed();
        if (line.isEmpty())
            continue;

        strLocales.append(line.split(" ", QString::SkipEmptyParts).first());
    }

    file.close();
    strLocales.removeDuplicates();


    foreach (QString locale, strLocales) {
        QStringList split = locale.split(QRegExp("[ .@]"), QString::SkipEmptyParts).first().split("_", QString::SkipEmptyParts);
        if (split.size() < 2)
            continue;

        LocaleSplit lc;
        lc.language = split.at(0);
        lc.territory = split.at(1);

        // Check if this locale is already in list
        bool found = false;
        for (int i = 0; i < locales.size(); i++) {
            if (locales.at(i).language == lc.language && locales.at(i).territory == lc.territory) {
                found = true;
                break;
            }
        }

        if (!found)
            locales.append(lc);
    }

    return locales;
}
