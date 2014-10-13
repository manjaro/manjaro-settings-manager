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

//###
//### Public
//###



int Global::runProcess(QString cmd, QStringList args, QStringList writeArgs, QString & error) {
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.start(cmd, args);

    if (!process.waitForStarted(5000))
        return -1;

    foreach (QString arg, writeArgs)
        process.write(QString(arg + "\n").toUtf8());

    process.closeWriteChannel();

    if (!process.waitForFinished(15000))
        return -1;

    error = QString::fromUtf8(process.readAll());
    return process.exitCode();
}



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



QList<Global::LocaleInfo> Global::getAllEnabledLocales() {
    QStringList localeList;
    QList<Global::LocaleInfo> locales, localeInfoList = getLocaleInfoList();

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

        localeList.append(line.split(" ", QString::SkipEmptyParts).first());
    }

    file.close();
    localeList.removeDuplicates();


    foreach (QString l, localeList) {
        bool found = false;

        for (int i = 0; i < localeInfoList.size(); i++) {
            const LocaleInfo *localeInfo = &localeInfoList.at(i);
            if (l != localeInfo->locale)
                continue;

            locales.append(*localeInfo);
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

            LocaleInfo lInfo = *localeInfo;
            lInfo.locale = l;
            locales.append(lInfo);
            found = true;
            break;
        }

        if (found)
            continue;

        // Just in case...
        LocaleInfo lInfo;
        lInfo.locale = l;
        locales.append(lInfo);
    }


    return locales;
}



QHash<QString, QHash<QString, QList<Global::Locale> > > Global::getAllLocales() {
    QHash<QString, QHash<QString, QList<Global::Locale> > > locales;
    QList<Global::LocaleInfo> localeInfoList = getLocaleInfoList();


    QFile file(LOCALESSUPPORTED);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return locales;

    QStringList listLocales;
    QTextStream in(&file);
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



QList<Global::User> Global::getAllUsers() {
    QList<Global::User> users;

    QFile file(PASSWD);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return users;

    QTextStream in(&file);

    while (!in.atEnd()) {
        QStringList split = in.readLine().split("#", QString::KeepEmptyParts).first().split(":", QString::KeepEmptyParts);
        if (split.size() < 7)
            continue;

        User user;
        user.username = split.at(0);
        user.homePath = split.at(5);
        user.uuid = split.at(2).toInt();

        if (user.uuid < MIN_USER_UUID || user.username.isEmpty() || user.homePath.isEmpty())
            continue;

        users.append(user);
    }

    file.close();

    return users;
}



QList<Global::Group> Global::getAllGroups() {
    QList<Global::Group> groups;

    QFile file(GROUPCONF);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return groups;

    QTextStream in(&file);

    while (!in.atEnd()) {
        QStringList split = in.readLine().split("#", QString::KeepEmptyParts).first().split(":", QString::KeepEmptyParts);
        if (split.size() < 4)
            continue;

        Group group;
        group.name = split.at(0);
        group.members = split.at(3).split(",", QString::SkipEmptyParts);

        if (group.name.isEmpty())
            continue;

        groups.append(group);
    }

    file.close();

    return groups;
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



QList<Global::LocaleInfo> Global::getLocaleInfoList() {
    QList<Global::LocaleInfo> localeInfoList;

    QFile file(LOCALESINFO);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return localeInfoList;

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

    return localeInfoList;
}


/*
 * Get the current running kernel using uname -r
 */
QString Global::getRunningKernel()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LANG", "C");
    env.insert("LC_MESSAGES", "C");
    env.insert("LC_ALL", "C");

    QProcess uname;
    uname.setProcessEnvironment(env);

    uname.start("uname", QStringList() << "-r");
    uname.waitForFinished();
    QString result = uname.readAllStandardOutput();
    uname.close();
    QStringList aux = result.split(".", QString::SkipEmptyParts);
    return QString("linux%1%2").arg(aux.at(0)).arg(aux.at(1));
}


QStringList Global::getLtsKernels()
{
    return QStringList() << "linux310" << "linux312" << "linux314";
}


QStringList Global::getRecommendedKernels()
{
    return QStringList() << "linux314";
}


/*
 * This function was copied from Octopi project
 *
 * Performs a pacman query
 */
QByteArray Global::performQuery(const QStringList args)
{
  QByteArray result("");
  QProcess pacman;

  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  env.insert("LC_ALL", "C");
  pacman.setProcessEnvironment(env);

  pacman.start("pacman", args);
  pacman.waitForFinished();
  result = pacman.readAllStandardOutput();
  pacman.close();

  return result;
}

/*
 * This function was copied from Octopi project
 *
 * Performs a pacman query
 * Overloaded with QString parameter
 */
QByteArray Global::performQuery(const QString &args)
{
  QByteArray result("");
  QProcess pacman;

  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  env.insert("LC_ALL", "C");
  pacman.setProcessEnvironment(env);

  pacman.start("pacman " + args);
  pacman.waitForFinished();
  result = pacman.readAllStandardOutput();
  pacman.close();
  return result;
}


/*
 * This function was copied from Octopi project
 *
 * Given a package name and if it is default to the official repositories,
 * returns a string containing all of its information fields
 * (ex: name, description, version, dependsOn...)
 */
QByteArray Global::packageInformation(const QString &pkgName, bool foreignPackage = false)
{
  QStringList args;

  if(foreignPackage)
    args << "-Qi";
  else
    args << "-Si";

  if (pkgName.isEmpty() == false) // enables get for all ("")
    args << pkgName;

  QByteArray result = performQuery(args);
  return result;
}


/*
 * This function was copied from Octopi project
 *
 * Given a QString containing the output of pacman -Si/Qi (pkgInfo),
 * this method returns the contents of the given field (ex: description)
 */
QString Global::extractFieldFromInfo(const QString &field, const QString &pkgInfo)
{
  int fieldPos = pkgInfo.indexOf(field);
  int fieldEnd, fieldEnd2;
  QString aux;

  if (fieldPos > 0)
  {
    if(field == "Optional Deps")
    {
      fieldPos = pkgInfo.indexOf(":", fieldPos+1);
      fieldPos+=2;
      aux = pkgInfo.mid(fieldPos);

      fieldEnd = aux.indexOf("Conflicts With");
      fieldEnd2 = aux.indexOf("Required By");

      if(fieldEnd > fieldEnd2 && fieldEnd2 != -1) fieldEnd = fieldEnd2;

      aux = aux.left(fieldEnd).trimmed();
      aux = aux.replace("\n", "<br>");
    }
    else
    {
      fieldPos = pkgInfo.indexOf(":", fieldPos+1);
      fieldPos+=2;
      aux = pkgInfo.mid(fieldPos);
      fieldEnd = aux.indexOf('\n');
      aux = aux.left(fieldEnd).trimmed();
    }
  }

  return aux;
}


/*
 * This function was copied from Octopi project
 *
 * Retrieves "Version" field of the given package information string represented by pkgInfo
 */
QString Global::packageVersion(const QString &pkgInfo)
{
  return extractFieldFromInfo("Version", pkgInfo);
}
