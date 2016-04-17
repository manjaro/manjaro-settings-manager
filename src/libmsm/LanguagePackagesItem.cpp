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

#include "LanguagePackagesItem.h"

LanguagePackagesItem::LanguagePackagesItem() :
    m_name( QByteArray() ),
    m_languagePackage( QByteArray() ),
    m_parentPackages( QList<QByteArray>() ),
    m_parentPkgInstalled( QList<QByteArray>() ),
    m_languagePkgInstalled( QList<QByteArray>() ),
    m_languagePkgAvailable( QList<QByteArray>() )
{
}

LanguagePackagesItem::LanguagePackagesItem(
    const QByteArray& name,
    const QByteArray& languagePackage,
    const QList<QByteArray>& parentPackages,
    const QList<QByteArray>& parentPkgInstalled,
    const QList<QByteArray>& languagePkgInstalled,
    const QList<QByteArray>& languagePkgAvailable )
    : m_name( name ),
      m_languagePackage( languagePackage ),
      m_parentPackages( parentPackages ),
      m_parentPkgInstalled( parentPkgInstalled ),
      m_languagePkgInstalled( languagePkgInstalled ),
      m_languagePkgAvailable( languagePkgAvailable )
{
}


LanguagePackagesItem::~LanguagePackagesItem()
{
}


QByteArray
LanguagePackagesItem::name() const
{
    return m_name;
}


void
LanguagePackagesItem::setName( const QByteArray& name )
{
    m_name = name;
}


QByteArray
LanguagePackagesItem::languagePackage() const
{
    return m_languagePackage;
}


void
LanguagePackagesItem::setLanguagePackage( const QByteArray& languagePackage )
{
    m_languagePackage = languagePackage;
}


QList<QByteArray>
LanguagePackagesItem::parentPackages() const
{
    return m_parentPackages;
}


void
LanguagePackagesItem::setParentPackages( const QList<QByteArray>& parentPackages )
{
    m_parentPackages = parentPackages;
}


QList<QByteArray>
LanguagePackagesItem::parentPkgInstalled() const
{
    return m_parentPkgInstalled;
}


void
LanguagePackagesItem::setParentPkgInstalled( const QList<QByteArray>& parentPkgInstalled )
{
    m_parentPkgInstalled = parentPkgInstalled;
}


QList<QByteArray>
LanguagePackagesItem::languagePkgInstalled() const
{
    return m_languagePkgInstalled;
}


void
LanguagePackagesItem::setLanguagePkgInstalled( const QList<QByteArray>& languagePkgInstalled )
{
    m_languagePkgInstalled = languagePkgInstalled;
}


QList<QByteArray>
LanguagePackagesItem::languagePkgAvailable() const
{
    return m_languagePkgAvailable;
}


void
LanguagePackagesItem::setLanguagePkgAvailable( const QList<QByteArray>& languagePkgAvailable )
{
    m_languagePkgAvailable = languagePkgAvailable;
}
