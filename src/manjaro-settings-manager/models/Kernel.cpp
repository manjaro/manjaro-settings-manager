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

#include "Kernel.h"

/* Kernel Class */
Kernel::Kernel() : package_ (QString()), version_ (QString()), installedModules_ (QStringList()),
                   availableModules_(QStringList()), isAvailable_(false), isInstalled_(false),
                   isLts_(false), isRecommended_(false), isRunning_(false)
{
}

Kernel::Kernel(const QString &package,
               const QString &version,
               const QStringList &installedModules,
               const QStringList &availableModules,
               const bool isAvailable,
               const bool isInstalled,
               const bool isLts,
               const bool isRecommended,
               const bool isRunning)
    : package_( package ),
      version_( version ),
      installedModules_( installedModules ),
      availableModules_( availableModules ),
      isAvailable_( isAvailable ),
      isInstalled_( isInstalled ),
      isLts_( isLts ),
      isRecommended_( isRecommended ),
      isRunning_( isRunning )
{
}

Kernel::~Kernel()
{
}

QString Kernel::package() const { return package_; }
QString Kernel::version() const { return version_; }

int Kernel::majorVersion() const
{
    QStringList splitVersion = version_.split(".");
    return splitVersion.value(0).toInt();
}

int Kernel::minorVersion() const
{
    QStringList splitVersion = version_.split(".");
    QString aux = splitVersion.value(1);
    if ( aux.length() > 1 ) {
        if ( aux.left(2).toInt() == 0 ) {
            return aux.left(1).toInt(); /* return single number minor version rc 3.1rc -> 1 */
        }
        return aux.left(2).toInt(); /* return double number minor version 3.12 or 3.12rc -> 12 */
    }
    return aux.toInt(); /* return single number minor version 3.1 -> 1 */
}

QStringList Kernel::installedModules() const { return installedModules_; }
QStringList Kernel::availableModules() const { return availableModules_; }
bool Kernel::isAvailable() const { return isAvailable_; }
bool Kernel::isInstalled() const { return isInstalled_; }
bool Kernel::isLts() const {return isLts_; }
bool Kernel::isRecommended() const { return isRecommended_; }
bool Kernel::isRunning() const { return isRunning_; }

bool Kernel::isUnsupported() const
{
    if ( !isAvailable_ ) {
        return isInstalled_;
    }
    return false;
}

void Kernel::setPackage(const QString &package)
{
    package_ = package;
}

void Kernel::setVersion(const QString &version)
{
    version_ = version;
}

void Kernel::setInstalledModules(const QStringList &modules)
{
    installedModules_ = modules;
}

void Kernel::setAvailableModules(const QStringList &modules)
{
    availableModules_ = modules;
}

void Kernel::setAvailable(const bool isAvailable)
{
    isAvailable_ = isAvailable;
}

void Kernel::setInstalled(const bool isInstalled)
{
    isInstalled_ = isInstalled;
}

void Kernel::setLts(const bool isLts)
{
    isLts_ = isLts;
}

void Kernel::setRecommended(const bool isRecommended)
{
    isRecommended_ = isRecommended;
}

void Kernel::setRunning(const bool isRunning)
{
    isRunning_ = isRunning;
}
