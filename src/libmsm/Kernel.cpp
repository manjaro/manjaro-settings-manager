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

#include "Kernel.h"

Kernel::Kernel() : m_package ( QString() ), m_version ( QString() ),
    m_installedModules ( QStringList() ), m_availableModules( QStringList() ),
    m_isAvailable( false ), m_isInstalled( false ), m_isLts( false ),
    m_isRecommended( false ), m_isRunning( false )
{
}


Kernel::Kernel( const QString& package,
                const QString& version,
                const QStringList& installedModules,
                const QStringList& availableModules,
                const bool isAvailable,
                const bool isInstalled,
                const bool isLts,
                const bool isRecommended,
                const bool isRunning )
    : m_package( package ),
      m_version( version ),
      m_installedModules( installedModules ),
      m_availableModules( availableModules ),
      m_isAvailable( isAvailable ),
      m_isInstalled( isInstalled ),
      m_isLts( isLts ),
      m_isRecommended( isRecommended ),
      m_isRunning( isRunning )
{
}


Kernel::~Kernel()
{
}


QString
Kernel::package() const
{
    return m_package;
}


QString
Kernel::version() const
{
    return m_version;
}


int
Kernel::majorVersion() const
{
    if ( m_version.isEmpty() )
        return 0;
    QStringList splitVersion = m_version.split( "." );
    return splitVersion.value( 0 ).toInt();
}


int
Kernel::minorVersion() const
{
    if ( m_version.isEmpty() )
        return 0;
    QStringList splitVersion = m_version.split( "." );
    QString aux = splitVersion.value( 1 );
    if ( aux.length() > 1 )
    {
        if ( aux.left( 2 ).toInt() == 0 )
        {
            return aux.left( 1 ).toInt(); /* return single number minor version rc 3.1rc -> 1 */
        }
        return aux.left( 2 ).toInt(); /* return double number minor version 3.12 or 3.12rc -> 12 */
    }
    return aux.toInt(); /* return single number minor version 3.1 -> 1 */
}


QStringList
Kernel::installedModules() const
{
    return m_installedModules;
}


QStringList
Kernel::availableModules() const
{
    return m_availableModules;
}


bool
Kernel::isAvailable() const
{
    return m_isAvailable;
}


bool
Kernel::isInstalled() const
{
    return m_isInstalled;
}


bool
Kernel::isLts() const
{
    return m_isLts;
}


bool
Kernel::isRecommended() const
{
    return m_isRecommended;
}


bool
Kernel::isRunning() const
{
    return m_isRunning;
}


bool
Kernel::isUnsupported() const
{
    return m_isUnsupported;
}

bool
Kernel::isRc() const
{
    if ( m_version.contains( "rc" ) )
        return true;
    return false;
}

bool

Kernel::isRealtime() const
{
    if ( m_version.contains( "rt" ) )
        return true;
    return false;
}

void
Kernel::setPackage( const QString& package )
{
    m_package = package;
}


void
Kernel::setVersion( const QString& version )
{
    m_version = version;
}


void
Kernel::setInstalledModules( const QStringList& modules )
{
    m_installedModules = modules;
}


void
Kernel::setAvailableModules( const QStringList& modules )
{
    m_availableModules = modules;
}


void
Kernel::setAvailable( const bool isAvailable )
{
    m_isAvailable = isAvailable;
}


void
Kernel::setInstalled( const bool isInstalled )
{
    m_isInstalled = isInstalled;
}


void
Kernel::setLts( const bool isLts )
{
    m_isLts = isLts;
}


void
Kernel::setRecommended( const bool isRecommended )
{
    m_isRecommended = isRecommended;
}


void
Kernel::setRunning( const bool isRunning )
{
    m_isRunning = isRunning;
}


void
Kernel::setUnsupported( const bool isUnsupported )
{
    m_isUnsupported = isUnsupported;
}


QString Kernel::toString()
{
    return QString( "{"
                    "package: '%1', "
                    "version: '%2', "
                    "majorVersion: '%3', "
                    "minorVersion: '%4', "
                    "installedModules: '%5', "
                    "availableModules: '%6', "
                    "isAvailable: '%7', "
                    "isInstalled: '%8', "
                    "isLts: '%9', "
                    "isRecommended: '%10', "
                    "isRunning: '%11', "
                    "isRc: '%12', "
                    "isRt: '%13', "
                    "}" )
           .arg( m_package )
           .arg( m_version )
           .arg( majorVersion() )
           .arg( minorVersion() )
           .arg( m_installedModules.join( ", " ) )
           .arg( m_availableModules.join( ", " ) )
           .arg( m_isAvailable )
           .arg( m_isInstalled )
           .arg( m_isLts )
           .arg( m_isRecommended )
           .arg( m_isRunning )
           .arg( isRc() )
           .arg( isRealtime() );
}
