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
    LanguagePackagesItem( const QString& name,
                          const QString& languagePackage,
                          const QStringList& parentPackages,
                          const QStringList& parentPkgInstalled,
                          const QStringList& languagePkgInstalled,
                          const QStringList& languagePkgAvailable );
    ~LanguagePackagesItem();

    QString name() const;
    void setName( const QString& name );

    QString languagePackage() const;
    void setLanguagePackage( const QString& languagePackage );

    QStringList parentPackages() const;
    void setParentPackages( const QStringList& parentPackages );

    QStringList parentPkgInstalled() const;
    void setParentPkgInstalled( const QStringList& parentPkgInstalled );

    QStringList languagePkgInstalled() const;
    void setLanguagePkgInstalled( const QStringList& languagePkgInstalled );

    QStringList languagePkgAvailable() const;
    void setLanguagePkgAvailable( const QStringList& languagePkgAvailable );

private:
    QString m_name;
    QString m_languagePackage;
    QStringList m_parentPackages;
    QStringList m_parentPkgInstalled;
    QStringList m_languagePkgInstalled;
    QStringList m_languagePkgAvailable;
};

#endif // LANGUAGEPACKAGESITEM_H

