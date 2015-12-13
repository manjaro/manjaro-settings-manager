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

#ifndef NOTIFIER_H
#define NOTIFIER_H

#include "LanguagePackagesItem.h"

#include <KNotifications/KStatusNotifierItem>

#include <QTimer>

class Notifier : public QTimer
{
    Q_OBJECT

public:
    explicit Notifier( QObject* parent = 0 );
    void start();

    enum KernelFlag
    {
        Unsupported = 0x01,
        Running = 0x02,
        New = 0x04,
    };
    Q_DECLARE_FLAGS( KernelFlags, KernelFlag )


private:
    KStatusNotifierItem m_tray;
    bool m_checkLanguagePackage;
    bool m_checkKernel;
    bool m_checkUnsupportedKernel;
    bool m_checkUnsupportedKernelRunning;
    bool m_checkNewKernel;
    bool m_checkNewKernelLts;
    bool m_checkNewKernelRecommended;

    void cLanguagePackage();
    void cKernel();
    bool isPackageIgnored( const QString package, const QString group );
    void addToConfig( const QString package, const QString group );

    bool hasPacmanEverSynced();
    bool isPacmanUpdating();
    bool isSystemUpToDate();

protected slots:
    void run();
    void loadConfiguration();

};

Q_DECLARE_OPERATORS_FOR_FLAGS( Notifier::KernelFlags )

#endif // NOTIFIER_H
