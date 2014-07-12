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

#include "daemon.h"

Daemon::Daemon(QObject *parent) :
    QTimer(parent)
{
    // Set Interval to 30 minutes
    setInterval(1800000);

    connect(this, SIGNAL(timeout())   ,   this, SLOT(run()));
    connect(&trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)) ,   this, SLOT(trayIconClicked()));
    connect(&trayIcon, SIGNAL(messageClicked()) ,   this, SLOT(trayIconClicked()));
    connect(&kernelTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)) ,   this, SLOT(kernelTrayIconClicked()));
    connect(&kernelTrayIcon, SIGNAL(messageClicked()) ,   this, SLOT(kernelTrayIconClicked()));

}



void Daemon::start() {
    if (isActive())
        return;

    QTimer::singleShot(20000, this, SLOT(run()));
    QTimer::singleShot(60000, this, SLOT(runKernel()));
    QTimer::start();
}

/*
 * PRIVATE
 */

void Daemon::run() {
    loadConfiguration();
    if (checkLanguagePackage) {
        cLanguagePackage();
    }
}

void Daemon::runKernel() {
    loadConfiguration();
    if (checkKernel){
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

    if (packages.isEmpty()) {
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
                if (value < 3)
                    settings.setValue("notify_count_" + l->languagePackage, value);
            }
        }
    } else {
        trayIcon.hide();
    }
}

void Daemon::cKernel() {
    QStringList installedKernels = Global::getAllInstalledKernels();
    QStringList availableKernels = Global::getAllAvailableKernels();
    QString runningKernel = Global::getRunningKernel();
    QStringList ltsKernels = Global::getLtsKernels();
    QStringList recommendedKernels = Global::getRecommendedKernels();
    QStringList unsupportedKernels;


    for (QString kernel : installedKernels) {
        if (!availableKernels.contains(kernel))
            unsupportedKernels << kernel;
    }

    if (checkUnsupportedKernel && !unsupportedKernels.isEmpty()) {
        if (checkUnsupportedKernelRunning) {
            if (unsupportedKernels.contains(runningKernel)) {
                if (!kernelTrayIcon.isVisible()) {
                    kernelTrayIcon.setIcon(QIcon(":/images/resources/tux-manjaro.png"));
                    kernelTrayIcon.show();
                    showKernelMessage(tr("Running an unsupported kernel"),
                                tr("You are running a kernel which is unsupported, please update"));

                }
            }
        } else  {
            if (unsupportedKernels.contains(runningKernel)) {
                if (!kernelTrayIcon.isVisible()) {
                    kernelTrayIcon.setIcon(QIcon(":/images/resources/tux-manjaro.png"));
                    kernelTrayIcon.show();
                    showKernelMessage(tr("Running an unsupported kernel"),
                                tr("You are running a kernel which is unsupported, please update"));

                }
            } else {
                if (!kernelTrayIcon.isVisible()) {
                    kernelTrayIcon.setIcon(QIcon(":/images/resources/tux-manjaro.png"));
                    kernelTrayIcon.show();
                    showKernelMessage(tr("Running an unsupported kernel"),
                                tr("You have installed a kernel which is unsupported."));

                }
            }
        }
    }

    if (checkNewKernel) {
        QStringList newKernels; // Not done yet
        QStringList newLtsRecommendedKernels;
        QStringList newLtsKernels;
        QStringList newRecommendedKernels;


        for(QString kernel : newKernels){
            if (ltsKernels.contains(kernel) && recommendedKernels.contains(kernel)) {
                newLtsRecommendedKernels << kernel;
                newLtsKernels << kernel;
                newRecommendedKernels << kernel;
            } else if (ltsKernels.contains(kernel)) {
                newLtsKernels << kernel;
            } else if (recommendedKernels.contains(kernel)) {
                newRecommendedKernels << kernel;
            }
        }

        if (checkNewKernelLts && checkNewKernelRecommended) {
            if (!newLtsRecommendedKernels.isEmpty()) {
                qDebug() << "Newer LTS & Recommended kernel available";
            }
        } else if (checkNewKernelLts) {
            if (!newLtsKernels.isEmpty()) {
                qDebug() << "Newer LTS  kernel available";
            }
        } else if (checkNewKernelRecommended) {
            if (!newRecommendedKernels.isEmpty()) {
                qDebug() << "Newer Recommended kernel available";
            }
        } else {
            if (!newKernels.isEmpty()) {
                qDebug() << "Newer kernel available";
            }
        }
    }
}

void Daemon::showMessage(QString messageTitle, QString messageText) {
    this->messageTitle = messageTitle;
    this->messageText = messageText;
    QTimer::singleShot(2000, this, SLOT(trayIconShowMessage()));
}

void Daemon::showKernelMessage(QString messageTitle, QString messageText) {
    this->messageTitle = messageTitle;
    this->messageText = messageText;
    QTimer::singleShot(2000, this, SLOT(kernelTrayIconShowMessage()));
}


void Daemon::trayIconClicked() {

    // Restart timer
    stop();
    QTimer::start();

    // Hide tray icon
    trayIcon.hide();

    QProcess::startDetached("manjaro-settings-manager", QStringList() << "--page-language-packages");
}

void Daemon::kernelTrayIconClicked() {

    // Restart timer
    stop();
    QTimer::start();

    // Hide tray icon
    kernelTrayIcon.hide();

    QProcess::startDetached("manjaro-settings-manager", QStringList() << "--page-kernel");
}



void Daemon::trayIconShowMessage() {
    trayIcon.showMessage(messageTitle, messageText, QSystemTrayIcon::Information, 30000);
}

void Daemon::kernelTrayIconShowMessage() {
    kernelTrayIcon.showMessage(messageTitle, messageText, QSystemTrayIcon::Information, 30000);
}


void Daemon::loadConfiguration() {
    QSettings settings("manjaro", "manjaro-settings-manager");
    this->checkLanguagePackage = settings.value("notifications/checkLanguagePackages", true).toBool();
    this->checkKernel = settings.value("notifications/checkKernel", true).toBool();
    this->checkUnsupportedKernel = settings.value("notifications/checkUnsupportedKernel", true).toBool();
    this->checkUnsupportedKernelRunning = settings.value("notifications/checkUnsupportedKernelRunning", false).toBool();
    this->checkNewKernel = settings.value("notifications/checkNewKernel", true).toBool();
    this->checkNewKernelLts = settings.value("notifications/checkNewKernelLts", false).toBool();
    this->checkNewKernelRecommended = settings.value("notifications/checkNewKernelRecommended", false).toBool();
    this->checkKernel = checkUnsupportedKernel | checkNewKernel;
}
