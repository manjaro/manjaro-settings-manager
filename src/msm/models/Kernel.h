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
            const bool isRunning,
            const QString& ltsSTring );
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
    QString ltsString() const;
    void setPackage( const QString& package );
    void setVersion( const QString& version );
    void setInstalledModules( const QStringList& modules );
    void setAvailableModules( const QStringList& modules );
    void setAvailable( const bool isAvailable );
    void setInstalled( const bool isInstalled );
    void setLts( const bool isLts );
    void setRecommended( const bool isRecommended );
    void setRunning( const bool isRunning );
    void setLtsString( const QString& ltsString );

private:
    QString package_;
    QString version_;
    QStringList installedModules_;
    QStringList availableModules_;
    bool isAvailable_;
    bool isInstalled_;
    bool isLts_;
    bool isRecommended_;
    bool isRunning_;
    QString ltsString_;
};

#endif // KERNEL_H
