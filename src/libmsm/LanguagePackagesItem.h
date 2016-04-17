/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Ramon Buldó <ramon@manjaro.org>
 *
 *  Manjaro Settings Manager is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Manjaro Settings Manager is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Manjaro Settings Manager.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LANGUAGEPACKAGESITEM_H
#define LANGUAGEPACKAGESITEM_H

#include <QtCore/QByteArray>
#include <QtCore/QList>

class LanguagePackagesItem
{
public:
    LanguagePackagesItem();
    LanguagePackagesItem( const QByteArray& name,
                          const QByteArray& languagePackage,
                          const QList<QByteArray>& parentPackages,
                          const QList<QByteArray>& parentPkgInstalled,
                          const QList<QByteArray>& languagePkgInstalled,
                          const QList<QByteArray>& languagePkgAvailable );
    ~LanguagePackagesItem();

    QByteArray name() const;
    void setName( const QByteArray& name );

    QByteArray languagePackage() const;
    void setLanguagePackage( const QByteArray& languagePackage );

    QList<QByteArray> parentPackages() const;
    void setParentPackages( const QList<QByteArray>& parentPackages );

    QList<QByteArray> parentPkgInstalled() const;
    void setParentPkgInstalled( const QList<QByteArray>& parentPkgInstalled );

    QList<QByteArray> languagePkgInstalled() const;
    void setLanguagePkgInstalled( const QList<QByteArray>& languagePkgInstalled );

    QList<QByteArray> languagePkgAvailable() const;
    void setLanguagePkgAvailable( const QList<QByteArray>& languagePkgAvailable );

private:
    QByteArray m_name;
    QByteArray m_languagePackage;
    QList<QByteArray> m_parentPackages;
    QList<QByteArray> m_parentPkgInstalled;
    QList<QByteArray> m_languagePkgInstalled;
    QList<QByteArray> m_languagePkgAvailable;
};

#endif // LANGUAGEPACKAGESITEM_H

