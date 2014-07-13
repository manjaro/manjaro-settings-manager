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

#ifndef DAEMON_H
#define DAEMON_H

#include <QDir>
#include <QFlags>
#include <QIcon>
#include <QLabel>
#include <QProcess>
#include <QSettings>
#include <QStringList>
#include <QTimer>
#include <QSystemTrayIcon>
#include <global.h>

class Daemon : public QTimer
{
    Q_OBJECT
public:
    explicit Daemon(QObject *parent = 0);
    void start();

    enum KernelFlag {
        Unsupported = 0x01,
        Running = 0x02,
        New = 0x04,
    };
    Q_DECLARE_FLAGS(KernelFlags, KernelFlag)


private:
    QSystemTrayIcon trayIcon, kernelTrayIcon;
    QString messageTitle, messageText;
    bool checkLanguagePackage;
    bool checkKernel, checkUnsupportedKernel, checkUnsupportedKernelRunning;
    bool checkNewKernel, checkNewKernelLts, checkNewKernelRecommended;

    void cLanguagePackage();
    void cKernel();
    void showMessage(QString messageTitle, QString messageText);
    void showKernelMessage(QString messageTitle, QString messageText);
    bool isPackageIgnored(const QString package);
    void addToConfig(const QString package);

protected slots:
    void run();
    void runKernel();
    void trayIconClicked();
    void trayIconShowMessage();
    void kernelTrayIconClicked();
    void kernelTrayIconShowMessage();
    void loadConfiguration();

};

Q_DECLARE_OPERATORS_FOR_FLAGS(Daemon::KernelFlags)

#endif // DAEMON_H
