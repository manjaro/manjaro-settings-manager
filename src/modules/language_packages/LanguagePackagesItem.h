/*
 *  Manjaro Settings Manager
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

#ifndef LANGUAGEPACKAGESITEM_H
#define LANGUAGEPACKAGESITEM_H

#include <QtCore/QString>
#include <QtCore/QStringList>

class LanguagePackagesItem
{
public:
    LanguagePackagesItem();
    LanguagePackagesItem(const QString &name,
           const QString &languagePackage,
           const QStringList &parentPackages,
           const QStringList &parentPkgInstalled,
           const QStringList &languagePkgInstalled,
           const QStringList &languagePkgAvailable);
    ~LanguagePackagesItem();

    QString name() const { return m_name; }
    QString languagePackage() const { return m_languagePackage; }
    QStringList parentPackages() const { return m_parentPackages; }
    QStringList parentPkgInstalled() const { return m_parentPkgInstalled; }
    QStringList languagePkgInstalled() const { return m_languagePkgInstalled; }
    QStringList languagePkgAvailable() const { return m_languagePkgAvailable; }
    void setName(const QString &package);
    void setLanguagePackage(const QString &version);
    void setParentPackages(const QStringList &modules);
    void setParentPkgInstalled(const QStringList &modules);
    void setLanguagePkgInstalled(const QStringList &modules);
    void setLanguagePkgAvailable(const QStringList &modules);

private:
    QString m_name;
    QString m_languagePackage;
    QStringList m_parentPackages;
    QStringList m_parentPkgInstalled;
    QStringList m_languagePkgInstalled;
    QStringList m_languagePkgAvailable;
};

#endif // LANGUAGEPACKAGESITEM_H

