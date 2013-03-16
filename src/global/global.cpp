/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
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


//###
//### Public
//###


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
    process.start("pacman", QStringList() << "-Qu");
    if (!process.waitForFinished()) {
        qDebug() << "error: failed to determind if system is up-to-date (pacman)!";
        return false;
    }

    return QString(process.readAll()).split("\n", QString::SkipEmptyParts).isEmpty();
}



//### Source by Georg Grabler <ggrabler@gmail.com> ###//
QMap<QString, QString> Global::getKeyboardModels() {
    QMap<QString, QString> models;

    QFile fh(XKB_FILE);
    fh.open(QIODevice::ReadOnly);

    if (!fh.isOpen()) {
        qDebug() << "X11 Keyboard model definitions not found!";
        return models;
    }

    bool modelsFound = false;
    // read the file until the end or until we break the loop
    while (!fh.atEnd()) {
        QByteArray line = fh.readLine();

        // check if we start with the model section in the file
        if (!modelsFound && line.startsWith("! model"))
            modelsFound = true;
        else if (modelsFound && line.startsWith ("!"))
            break;
        else if (!modelsFound)
            continue;

        // here we are in the model section, otherwhise we would continue or break
        QRegExp rx;
        rx.setPattern("^\\s+(\\S+)\\s+(\\w.*)\n$");

        // insert into the model map
        if (rx.indexIn(line) != -1) {
            QString modelDesc = rx.cap(2);
            QString model = rx.cap(1);

            if (model == "pc105")
                modelDesc += "  -  " + QObject::tr("Default Keyboard Model");

            models.insert(modelDesc, model);
        }
    }

    return models;
}



QMap<QString, Global::KeyboardInfo> Global::getKeyboardLayouts() {
    QMap< QString, KeyboardInfo >  layouts;

    //### Get Layouts ###//

    QFile fh(XKB_FILE);
    fh.open(QIODevice::ReadOnly);

    if (!fh.isOpen()) {
        qDebug() << "X11 Keyboard layout definitions not found!";
        return layouts;
    }

    bool layoutsFound = false;
    // read the file until the end or we break the loop
    while (!fh.atEnd()) {
        QByteArray line = fh.readLine();

        // find the layout section otherwhise continue. If the layout section is at it's end, break the loop
        if (!layoutsFound && line.startsWith("! layout"))
            layoutsFound = true;
        else if (layoutsFound && line.startsWith ("!"))
            break;
        else if (!layoutsFound)
            continue;

        QRegExp rx;
        rx.setPattern("^\\s+(\\S+)\\s+(\\w.*)\n$");

        // insert into the layout map
        if (rx.indexIn(line) != -1) {
            KeyboardInfo info;
            info.description = rx.cap(2);
            info.variants.insert(QObject::tr("Default"), "");
            layouts.insert(rx.cap(1), info);
        }
    }

    fh.reset();


    //### Get Variants ###//

    bool variantsFound = false;
    // read the file until the end or until we break
    while (!fh.atEnd()) {
        QByteArray line = fh.readLine();

        // continue until we found the variant section. If found, read until the next section is found
        if (!variantsFound && line.startsWith("! variant")) {
            variantsFound = true;
            continue;
        } else if (variantsFound && line.startsWith ("!"))
            break;
        else if (!variantsFound)
            continue;

        QRegExp rx;
        rx.setPattern("^\\s+(\\S+)\\s+(\\S+): (\\w.*)\n$");

        // insert into the variants multimap, if the pattern matches
        if (rx.indexIn(line) != -1) {
            if (layouts.find(rx.cap(2)) != layouts.end()) {
                // in this case we found an entry in the multimap, and add the values to the multimap
                layouts.find(rx.cap(2)).value().variants.insert(rx.cap(3), rx.cap(1));
            } else {
                // create a new map in the multimap - the value was not found.
                KeyboardInfo info;
                info.description = rx.cap(2);
                info.variants.insert(QObject::tr("Default"), "");
                info.variants.insert(rx.cap(3), rx.cap(1));
                layouts.insert(rx.cap(2), info);
            }
        }
    }

    return layouts;
}



bool Global::getCurrentXorgKeyboardLayout(QString & layout, QString & variant) {
    layout.clear();
    variant.clear();

    QProcess process;
    process.start("setxkbmap", QStringList() << "-print");

    if (!process.waitForFinished())
        return false;

    QStringList list = QString(process.readAll()).split("\n", QString::SkipEmptyParts);

    foreach(QString line, list) {
        line = line.trimmed();
        if (!line.startsWith("xkb_symbols"))
            continue;

        line = line.remove("}").remove("{").remove(";");
        line = line.mid(line.indexOf("\"") + 1);

        QStringList split = line.split("+", QString::SkipEmptyParts);
        if (split.size() >= 2) {
            layout = split.at(1);

            if (layout.contains("(")) {
                variant = layout.mid(layout.indexOf("(") + 1);
                variant = variant.mid(0, variant.lastIndexOf(")")).trimmed();
                layout = layout.mid(0, layout.indexOf("(")).trimmed();
            }
        }
    }

    return !layout.isEmpty();
}



QStringList Global::getAllEnabledLocales() {
    QStringList locales;

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

        locales.append(line.split(" ", QString::SkipEmptyParts).first());
    }

    file.close();

    locales.removeDuplicates();
    return locales;
}



QString Global::getCurrentLocale() {
    QString locale;

    QFile file(LOCALE_CONF);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "error: failed to open '" << LOCALE_CONF << "'!";
        return "";
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().split("#", QString::KeepEmptyParts).first().trimmed();
        if (line.isEmpty() || !QString(line).toLower().startsWith("lang="))
            continue;

        locale = line.mid(line.indexOf("=") + 1).trimmed();
    }

    file.close();

    return locale;
}




QHash<QString, QHash<QString, QList<Global::Locale> > > Global::getAllLocales() {
    QHash<QString, QHash<QString, QList<Global::Locale> > > locales;
    QList<Global::LocaleInfo> localeInfoList;

    QFile file(LOCALESINFO);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return locales;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().split("#", QString::KeepEmptyParts).first().trimmed();
        QStringList split = line.split("|", QString::SkipEmptyParts);
        if (split.size() < 4)
            continue;

        bool skip = false;
        foreach (QString str, split) {
            if (str.trimmed().isEmpty()) {
                skip = true;
                break;
            }
        }

        if (skip)
            continue;

        LocaleInfo locale;
        locale.locale = split.at(0).trimmed();
        locale.language = split.at(1).trimmed();
        locale.territory = split.at(2).trimmed();
        locale.description = split.at(3).trimmed();
        localeInfoList.append(locale);
    }

    file.close();



    file.setFileName(LOCALESSUPPORTED);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return locales;

    QStringList listLocales;
    in.setDevice(&file);
    bool found=false;

    while (!in.atEnd()) {
        QString line = in.readLine().split("#", QString::KeepEmptyParts).first().trimmed();
        if (line.startsWith("SUPPORTED-LOCALES=")) {
            found = true;
            line = line.remove("SUPPORTED-LOCALES=").trimmed();
        }

        if (found && !line.isEmpty())
            listLocales.append(line.mid(0, line.indexOf("/")).remove("\\").trimmed());
    }
    file.close();

    listLocales.removeAll("");
    listLocales.removeDuplicates();



    foreach (QString l, listLocales) {
        bool found = false;

        for (int i = 0; i < localeInfoList.size(); i++) {
            const LocaleInfo *localeInfo = &localeInfoList.at(i);
            if (l != localeInfo->locale)
                continue;

            Locale item;
            item.locale = l;
            item.description = localeInfo->description;
            locales[localeInfo->language][localeInfo->territory].append(item);

            found = true;
            break;
        }

        if (found)
            continue;

        QString lShort = l.split(QRegExp("[ .@]"), QString::KeepEmptyParts).first().trimmed();

        for (int i = 0; i < localeInfoList.size(); i++) {
            const LocaleInfo *localeInfo = &localeInfoList.at(i);
            if (lShort != localeInfo->locale)
                continue;

            Locale item;
            item.locale = l;
            item.description = localeInfo->description;
            locales[localeInfo->language][localeInfo->territory].append(item);
            break;
        }
    }

    return locales;
}



QString Global::localeToValidLocaleGenString(QString locale) {
    QFile file(LOCALESSUPPORTED);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().split("#", QString::KeepEmptyParts).first().remove("\\").trimmed();
        if (line.startsWith(locale + "/"))
            return line.replace("/", " ");
    }
    file.close();

    return "";
}





//###
//### Private
//###


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
    QStringList strLocales = getAllEnabledLocales();
    QList<Global::LocaleSplit> locales;

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
