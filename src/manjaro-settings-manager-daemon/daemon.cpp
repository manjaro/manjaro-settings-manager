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

    connect(this, SIGNAL(timeout())   ,   this, SLOT(run()));
    connect(&trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)) ,   this, SLOT(trayIcon_clicked()));
    connect(&trayIcon, SIGNAL(messageClicked()) ,   this, SLOT(trayIcon_clicked()));
}



void Daemon::start() {
    if (isActive())
        return;

    QTimer::singleShot(20000, this, SLOT(run()));
    QTimer::start();
}



void Daemon::run() {
    // Check if language packages are available
    QList<Global::LanguagePackage> availablePackages, installedPackages, packages;
    Global::getLanguagePackages(&availablePackages, &installedPackages);

    // Check if packages should be ignored
    for (int i = 0; i < availablePackages.size(); i++) {
        const Global::LanguagePackage *l = &availablePackages.at(i);
        QString value = Global::getConfigValue("notify_count_" + l->languagePackage, QDir::homePath() + DAEMON_CONF_FILE);

        if (value.isEmpty() || value.toInt() < 2)
            packages.append(*l);
    }

    if (!packages.isEmpty()) {
        if (!trayIcon.isVisible()) {
            trayIcon.setIcon(QIcon(":/images/resources/language.png"));
            trayIcon.show();
            showMessage(tr("Additional Language Packages"),
                        tr("%1 new additional language package(s) available").arg(QString::number(packages.size())));

            // Add to Config
            for (int i = 0; i < packages.size(); i++) {
                const Global::LanguagePackage *l = &packages.at(i);
                QString value = Global::getConfigValue("notify_count_" + l->languagePackage, QDir::homePath() + DAEMON_CONF_FILE);
                int count = 0;

                if (!value.isEmpty() && value.toInt() > 0)
                    count = value.toInt();

                ++count;

                if (count < 3)
                    Global::setConfigValue("notify_count_" + l->languagePackage, QString::number(count), QDir::homePath() + DAEMON_CONF_FILE);
            }
        }
    }
    else {
        trayIcon.hide();
    }
}



void Daemon::showMessage(QString messageTitel, QString messageText) {
    this->messageTitel = messageTitel;
    this->messageText = messageText;
    QTimer::singleShot(2000, this, SLOT(trayIcon_showMessage()));
}



void Daemon::trayIcon_clicked() {
    // Restart timer
    stop();
    QTimer::start();

    // Hide tray icon
    trayIcon.hide();

    QProcess::startDetached("manjaro-settings-manager", QStringList() << "--page-language-packages");
}



void Daemon::trayIcon_showMessage() {
    trayIcon.showMessage(messageTitel, messageText, QSystemTrayIcon::Information, 30000);
}
