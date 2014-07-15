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
    // Set Interval to 60 minutes
    setInterval(3600000);

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
    QTimer::singleShot(60, this, SLOT(runKernel()));
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

void Daemon::cKernel() {
    QStringList installedKernels = Global::getAllInstalledKernels();
    QStringList availableKernels = Global::getAllAvailableKernels();
    QString runningKernel = Global::getRunningKernel();
    QStringList ltsKernels = Global::getLtsKernels();
    QStringList recommendedKernels = Global::getRecommendedKernels();
    QStringList unsupportedKernels;
    QStringList newKernels;
    Daemon::KernelFlags kernelFlags;

    for (QString kernel : installedKernels) {
        if (!isPackageIgnored(kernel, "unsupported_kernel") && !availableKernels.contains(kernel)) {
            unsupportedKernels << kernel;
        }
    }

    if (checkUnsupportedKernel && !unsupportedKernels.isEmpty()) {
        if (unsupportedKernels.contains(runningKernel)) {
            kernelFlags |= KernelFlag::Unsupported | KernelFlag::Running;
        }
        if (!checkUnsupportedKernelRunning) {
            kernelFlags |= KernelFlag::Unsupported;
        }
    }

    if (checkNewKernel) {
        /* Obtain the version of the latest installed kernel */
        int major = 0;
        int minor = 0;
        for (QString kernel : installedKernels) {
            QString version;
            if (availableKernels.contains(kernel)){
               version = Global::getKernelVersion(kernel, false);
            } else {
                version = Global::getKernelVersion(kernel, true);
            }
            QStringList versionStringList = version.split(".");
            int thisMajor = versionStringList.at(0).toInt();
            int thisMinor = versionStringList.at(1).left(2).toInt();

            if (thisMajor > major) {
                major = thisMajor;
                minor = thisMinor;
            } else if ((thisMajor == major) && (thisMinor > minor)){
                minor = thisMinor;
            }
        }

        // Obtain the list of new kernels
        for (QString kernel : availableKernels) {
            if (isPackageIgnored(kernel, "new_kernel")) {
                continue;
            }
            QString version = Global::getKernelVersion(kernel, false);
            QStringList versionStringList = version.split(".");
            int thisMajor = versionStringList.at(0).toInt();
            int thisMinor = versionStringList.at(1).left(2).toInt();
            qDebug() << kernel << ": " << thisMajor << "." << thisMinor;
            if (thisMajor > major) {
                newKernels << kernel;
                              qDebug() << newKernels;
            } else if ((thisMajor == major) && (thisMinor > minor)) {
                newKernels << kernel;
                qDebug() << "hi";
                qDebug() << newKernels;
            }
        }

        /* Find kernels that are lts, recommended or both */
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
            if (!newKernels.isEmpty()) {
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

    if (!messageTitle.isEmpty()) {
        if (!kernelTrayIcon.isVisible()) {
            kernelTrayIcon.setIcon(QIcon(":/images/resources/tux-manjaro.png"));
            kernelTrayIcon.show();
            kernelTrayIcon.setToolTip(messageText);
            showKernelMessage(messageTitle, messageText);

            for (QString kernel : unsupportedKernels) {
                addToConfig(kernel, "unsupported_kernel");
            }
            for (QString kernel : newKernels) {
                addToConfig(kernel, "new_kernel");
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
    this->kernelMessageTitle = messageTitle;
    this->kernelMessageText = messageText;
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
    kernelTrayIcon.showMessage(kernelMessageTitle, kernelMessageText, QSystemTrayIcon::Information, 30000);
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


bool Daemon::isPackageIgnored(const QString package, const QString group) {
    QSettings settings("manjaro", "manjaro-settings-manager-daemon");
    settings.beginGroup(group);
    int value = settings.value("notify_count_" + package, "0").toInt();
    settings.endGroup();
    return (value < 2) ? false : true;
}


void Daemon::addToConfig(const QString package, const QString group) {
    QSettings settings("manjaro", "manjaro-settings-manager-daemon");
    settings.beginGroup(group);
    int value = settings.value("notify_count_" + package, "0").toInt();
    ++value;
    if (value < 3)
        settings.setValue("notify_count_" + package, value);
    settings.endGroup();
}


