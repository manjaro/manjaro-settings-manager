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

#ifndef KERNEL_H
#define KERNEL_H

#include <QtCore/QString>
#include <QtCore/QStringList>

class Kernel
{
public:
    Kernel();
    Kernel( const QString& package,
            const QString& version,
            const QStringList& installedModules,
            const QStringList& availableModules,
            const bool isAvailable,
            const bool isInstalled,
            const bool isLts,
            const bool isRecommended,
            const bool isRunning );
    ~Kernel();

    QString package() const;
    QString version() const;
    int majorVersion() const;
    int minorVersion() const;
    QStringList installedModules() const;
    QStringList availableModules() const;
    bool isAvailable() const;
    bool isInstalled() const;
    bool isLts() const;
    bool isRecommended() const;
    bool isRunning() const;
    bool isUnsupported() const;
    bool isRc() const;
    bool isRealtime() const;
    void setPackage( const QString& package );
    void setVersion( const QString& version );
    void setInstalledModules( const QStringList& modules );
    void setAvailableModules( const QStringList& modules );
    void setAvailable( const bool isAvailable );
    void setInstalled( const bool isInstalled );
    void setLts( const bool isLts );
    void setRecommended( const bool isRecommended );
    void setRunning( const bool isRunning );
    void setUnsupported( const bool isUnsupported );

    QString toString();

private:
    QString m_package;
    QString m_version;
    QStringList m_installedModules;
    QStringList m_availableModules;
    bool m_isAvailable;
    bool m_isInstalled;
    bool m_isLts;
    bool m_isRecommended;
    bool m_isRunning;
    bool m_isUnsupported;
};

#endif // KERNEL_H
