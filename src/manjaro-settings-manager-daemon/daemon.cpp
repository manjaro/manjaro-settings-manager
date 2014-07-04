/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
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

#include "daemon.h"

Daemon::Daemon(QObject *parent) :
    QTimer(parent)
{
    // Set Interval to 30 minutes
    setInterval(1800000);
    loadConfiguration();

    connect(this, SIGNAL(timeout())   ,   this, SLOT(run()));
    connect(&trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)) ,   this, SLOT(trayIconClicked()));
    connect(&trayIcon, SIGNAL(messageClicked()) ,   this, SLOT(trayIconClicked()));

}



void Daemon::start() {
    if (isActive())
        return;

    QTimer::singleShot(2000, this, SLOT(run()));
    QTimer::start();
}

/*
 * PRIVATE
 */

void Daemon::run() {
    if (checkLanguagePackage) {
        qDebug() << "Checking Language Package";
        cLanguagePackage();
    }
    if (checkKernel){
        qDebug() << "Checking Kernel";
        cKernel();
    }
}



void Daemon::cLanguagePackage() {
    // Check if language packages are available
    QList<Global::LanguagePackage> availablePackages, installedPackages, packages;
    Global::getLanguagePackages(&availablePackages, &installedPackages);

    QSettings settings("manjaro", "manjaro-settings-manager-daemon");

    // Check if packages should be ignored
    for (int i = 0; i < availablePackages.size(); i++) {
        const Global::LanguagePackage *l = &availablePackages.at(i);
        int value = settings.value("notify_count_" + l->languagePackage, "0").toInt();
        if (value < 2)
            packages.append(*l);
    }

    if (!packages.isEmpty()) {
        if (!trayIcon.isVisible()) {
            trayIcon.setIcon(QIcon(":/images/resources/language.png"));
            trayIcon.show();
            int packagesCount = packages.size();
            showMessage(tr("Additional Language Package(s)", "", packagesCount),
                        tr("%n new additional language package(s) available", "", packagesCount));

            // Add to Config
            for (int i = 0; i < packages.size(); i++) {
                const Global::LanguagePackage *l = &packages.at(i);
                int value = settings.value("notify_count_" + l->languagePackage, "0").toInt();
                ++value;
                //if (value < 3)
                settings.setValue("notify_count_" + l->languagePackage, value);
            }
        }
    } else {
        trayIcon.hide();
    }
}

void Daemon::cKernel() {
    if (checkOutdatedKernel) {
        //get installed kernels not available in pacman (outdated)
        if (checkOutdatedKernelRunning) {
            qDebug() << "Notify that running is outdated";
            // only if running = outdated, notify
        } else {
            qDebug() << "Notify that some installed kernel is outdated";
            // notify outdated kernels installed
        }
    }
    if (checkNewKernel) {
        // get latest kernel installed
        // get newer kernels > installed
        if (checkNewKernelLts && checkNewKernelRecommended) {
            // notify if one of the new kernels is lts AND recommended
        } else if (checkNewKernelLts) {
            // notify if one of the new kernels is LTS
        } else if (checkNewKernelRecommended) {
            // notify if one of the new kernels is recommended
        } else {
            // notify if there is a newer kernel
        }
    }
}

void Daemon::showMessage(QString messageTitle, QString messageText) {
    this->messageTitle = messageTitle;
    this->messageText = messageText;
    QTimer::singleShot(2000, this, SLOT(trayIconShowMessage()));
}



void Daemon::trayIconClicked() {

    // Restart timer
    stop();
    QTimer::start();

    // Hide tray icon
    trayIcon.hide();

    QProcess::startDetached("manjaro-settings-manager", QStringList() << "--page-language-packages");
}



void Daemon::trayIconShowMessage() {
    trayIcon.showMessage(messageTitle, messageText, QSystemTrayIcon::Information, 30000);
}


void Daemon::loadConfiguration() {
    QSettings settings("manjaro", "manjaro-settings-manager");
    this->checkLanguagePackage = settings.value("notifications/checkLanguagePackages", true).toBool();
    this->checkKernel = settings.value("notifications/checkOutdatedKernel", true).toBool();
    this->checkOutdatedKernel = settings.value("notifications/checkOutdatedKernel", true).toBool();
    this->checkOutdatedKernelRunning = settings.value("notifications/checkOutdatedKernelRunning", false).toBool();
    this->checkNewKernel = settings.value("notifications/checkNewKernel", true).toBool();
    this->checkNewKernelLts = settings.value("notifications/checkNewKernelLts", false).toBool();
    this->checkNewKernelRecommended = settings.value("notifications/checkNewKernelRecommended", false).toBool();
}
