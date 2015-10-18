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
#include "models/Kernel.h"
#include "models/KernelModel.h"

#include <QtCore/QFile>

Daemon::Daemon(QObject *parent) :
    QTimer(parent)
{
    // Set Interval to 60 minutes
    setInterval(3600000);

    connect(this, &Daemon::timeout,
            this, &Daemon::run);
    connect(&trayIcon, &QSystemTrayIcon::activated,
            this, &Daemon::trayIconClicked);
    connect(&trayIcon, &QSystemTrayIcon::messageClicked,
            this, &Daemon::trayIconClicked);
    connect(&kernelTrayIcon, &QSystemTrayIcon::activated,
            this, &Daemon::kernelTrayIconClicked);
    connect(&kernelTrayIcon, &QSystemTrayIcon::messageClicked,
            this, &Daemon::kernelTrayIconClicked);
}



void Daemon::start()
{
    if (isActive()) {
        return;
    }
    QTimer::singleShot(20000, this, SLOT(run()));
    QTimer::singleShot(40000, this, SLOT(runKernel()));
    QTimer::start();
}


/*
 * PRIVATE
 */


void Daemon::run()
{
    loadConfiguration();
    if (checkLanguagePackage && Global::isSystemUpToDate() &&
        !isPacmanUpdating() && hasPacmanEverSynced()) {
        cLanguagePackage();
    }
}


void Daemon::runKernel()
{
    loadConfiguration();
    if (checkKernel && Global::isSystemUpToDate() &&
        !isPacmanUpdating() && hasPacmanEverSynced()) {
        cKernel();
    }
}


void Daemon::cLanguagePackage()
{
    // Check if language packages are available
    QList<Global::LanguagePackage> availablePackages, installedPackages, packages;
    Global::getLanguagePackages(&availablePackages, &installedPackages);

    // Check if packages should be ignored
    for (int i = 0; i < availablePackages.size(); i++) {
        const Global::LanguagePackage *l = &availablePackages.at(i);
        if (!isPackageIgnored(l->languagePackage, "language_package")) {
            packages.append(*l);
        }
    }

    if (!packages.isEmpty()) {
        if (!trayIcon.isVisible()) {
            trayIcon.setIcon(QIcon(":/images/resources/language.png"));
            trayIcon.show();
            int packagesCount = packages.size();
            QString messageText = QString(tr("%n new additional language package(s) available", "", packagesCount));
            trayIcon.setToolTip(messageText);
            showMessage(tr("Additional Language Package(s)", "", packagesCount),
                        messageText);
            // Add to Config
            for (int i = 0; i < packages.size(); i++) {
                const Global::LanguagePackage *l = &packages.at(i);
                addToConfig(l->languagePackage, "language_package");
            }
        }
    } else {
        trayIcon.hide();
    }
}


void Daemon::cKernel()
{
    Daemon::KernelFlags kernelFlags;
    KernelModel kernelModel;
    kernelModel.update();

    QList< Kernel > unsupportedKernels = kernelModel.unsupportedKernels();
    if (checkUnsupportedKernel && !unsupportedKernels.isEmpty()) {
        for (Kernel kernel : unsupportedKernels) {
            if ( isPackageIgnored(kernel.package(), "unsupported_kernel") ) {
                qDebug() << "Ignored unsupported kernel: " << kernel.version();
                continue;
            }
            qDebug() << "Unsupported kernel: " << kernel.version();
            kernelFlags |= KernelFlag::Unsupported;
            if ( checkUnsupportedKernelRunning && kernel.isRunning() ) {
                kernelFlags |= KernelFlag::Running;
                qDebug() << "Unsupported kernel running: " << kernel.version();
            }
        }
    }

    qDebug() << "Latest installed kernel: " << kernelModel.latestInstalledKernel().version();
    QList<Kernel> newKernels = kernelModel.newerKernels(kernelModel.latestInstalledKernel());
    QList<Kernel> newLtsRecommendedKernels;
    QList<Kernel> newLtsKernels;
    QList<Kernel> newRecommendedKernels;
    QList<Kernel> newNotIgnoredKernels;
    if (checkNewKernel) {
        for (Kernel kernel : newKernels) {
            if (isPackageIgnored(kernel.package(), "new_kernel")) {
                qDebug() << "Ignored new kernel: " << kernel.version();
                continue;
            }
            newNotIgnoredKernels << kernel;
            qDebug() << "Newer kernel " << kernel.version();
            if (kernel.isRecommended() && kernel.isLts()) {
                qDebug() << "Newer kernel LTS & Recommended: " << kernel.version();
                newLtsRecommendedKernels << kernel;
                newLtsKernels << kernel;
                newRecommendedKernels << kernel;
            } else if (kernel.isLts()) {
                qDebug() << "Newer kernel LTS: " << kernel.version();
                newLtsKernels << kernel;
            } else if (kernel.isRecommended()) {
                qDebug() << "Newer kernel Recommended: " << kernel.version();
                newRecommendedKernels << kernel;
            }
        }


        if (checkNewKernelLts && checkNewKernelRecommended) {
            if (!newLtsRecommendedKernels.isEmpty()) {
                kernelFlags |= KernelFlag::New;
            }
        } else if (checkNewKernelLts) {
            if (!newLtsKernels.isEmpty()) {
                kernelFlags |= KernelFlag::New;
            }
        } else if (checkNewKernelRecommended) {
            if (!newRecommendedKernels.isEmpty()) {
                kernelFlags |= KernelFlag::New;
            }
        } else {
            if (!newNotIgnoredKernels.isEmpty()) {
                kernelFlags |= KernelFlag::New;
            }
        }
    }

    QString messageTitle;
    QString messageText;
    if (kernelFlags.testFlag(KernelFlag::Unsupported) && kernelFlags.testFlag(KernelFlag::Running)) {
        messageText = QString(tr("Running an unsupported kernel, please update"));
    } else if (kernelFlags.testFlag(KernelFlag::Unsupported)) {
        messageText = QString(tr("Unsupported kernel installed in your system."));
    }

    if (kernelFlags.testFlag(KernelFlag::Unsupported) && kernelFlags.testFlag(KernelFlag::New)) {
        messageTitle = QString(tr("Your kernels need attention."));
        messageText.append("\n");
    } else if (kernelFlags.testFlag(KernelFlag::Unsupported)) {
        messageTitle = QString(tr("Unsupported Kernel Found."));
    } else if (kernelFlags.testFlag(KernelFlag::New)) {
        messageTitle = QString(tr("New Kernel Available."));
    }

    if (kernelFlags.testFlag(KernelFlag::New)) {
        messageText.append(QString(tr("A kernel newer than the latest installed is available.")));
    }

    if (kernelFlags.testFlag(KernelFlag::Unsupported) || kernelFlags.testFlag(KernelFlag::New)) {
        if (!kernelTrayIcon.isVisible() ) {
            kernelTrayIcon.setIcon(QIcon(":/images/resources/tux-manjaro.png"));
            kernelTrayIcon.show();
            kernelTrayIcon.setToolTip(messageText);
            showKernelMessage(messageTitle, messageText);
            for (Kernel kernel : unsupportedKernels) {
                addToConfig(kernel.package(), "unsupported_kernel");
            }
            for (Kernel kernel : newNotIgnoredKernels) {
                addToConfig(kernel.package(), "new_kernel");
            }
        }
    }
}


void Daemon::showMessage(QString messageTitle, QString messageText)
{
    this->messageTitle = messageTitle;
    this->messageText = messageText;
    QTimer::singleShot(2000, this, SLOT(trayIconShowMessage()));
}


void Daemon::showKernelMessage(QString messageTitle, QString messageText)
{
    this->kernelMessageTitle = messageTitle;
    this->kernelMessageText = messageText;
    QTimer::singleShot(2000, this, SLOT(kernelTrayIconShowMessage()));
}


void Daemon::trayIconClicked()
{
    // Restart timer
    stop();
    QTimer::start();

    // Hide tray icon
    trayIcon.hide();

    QProcess::startDetached("manjaro-settings-manager", QStringList() << "--page-language-packages");
}


void Daemon::kernelTrayIconClicked()
{
    // Restart timer
    stop();
    QTimer::start();

    // Hide tray icon
    kernelTrayIcon.hide();

    QProcess::startDetached("manjaro-settings-manager", QStringList() << "--page-kernel");
}


void Daemon::trayIconShowMessage()
{
    trayIcon.showMessage(messageTitle, messageText, QSystemTrayIcon::Information, 30000);
}


void Daemon::kernelTrayIconShowMessage()
{
    kernelTrayIcon.showMessage(kernelMessageTitle, kernelMessageText, QSystemTrayIcon::Information, 30000);
}


void Daemon::loadConfiguration()
{
    QSettings settings("/root/.config/manjaro/manjaro-settings-manager.conf",
                        QSettings::IniFormat);
    this->checkLanguagePackage = settings.value("notifications/checkLanguagePackages", true).toBool();
    this->checkUnsupportedKernel = settings.value("notifications/checkUnsupportedKernel", true).toBool();
    this->checkUnsupportedKernelRunning = settings.value("notifications/checkUnsupportedKernelRunning", false).toBool();
    this->checkNewKernel = settings.value("notifications/checkNewKernel", true).toBool();
    this->checkNewKernelLts = settings.value("notifications/checkNewKernelLts", false).toBool();
    this->checkNewKernelRecommended = settings.value("notifications/checkNewKernelRecommended", true).toBool();
    this->checkKernel = checkUnsupportedKernel | checkNewKernel;
}


bool Daemon::isPackageIgnored(const QString package, const QString group)
{
    QSettings settings("manjaro", "manjaro-settings-manager-daemon");
    settings.beginGroup(group);
    int value = settings.value("notify_count_" + package, "0").toInt();
    settings.endGroup();
    return (value < 2) ? false : true;
}


void Daemon::addToConfig(const QString package, const QString group)
{
    QSettings settings("manjaro", "manjaro-settings-manager-daemon");
    settings.beginGroup(group);
    int value = settings.value("notify_count_" + package, "0").toInt();
    ++value;
    if (value < 3) {
        settings.setValue("notify_count_" + package, value);
    }
    settings.endGroup();
}


bool Daemon::hasPacmanEverSynced()
{
    QString path( "/var/lib/pacman/sync/" );
    QStringList files = QStringList() << "core.db" << "community.db" << "extra.db";
    for (QString f : files) {
        if (!QFile::exists(path + f)) {
             return false;
        }
    }
    return true;
}


bool Daemon::isPacmanUpdating()
{
    return QFile::exists( "/var/lib/pacman/db.lck" );
}

