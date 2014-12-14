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

#include "PageNotifications.h"
#include "ui_PageNotifications.h"

#include <QtCore/QSettings>
#include <QDebug>

#include <KAboutData>
#include <KPluginFactory>
K_PLUGIN_FACTORY(MsmNotificationsFactory,
                 registerPlugin<PageNotifications>("msm_notifications");)

PageNotifications::PageNotifications(QWidget *parent, const QVariantList &args) :
    KCModule(parent, args),
    ui(new Ui::PageNotifications)
{
    KAboutData *aboutData = new KAboutData("msm_notifications",
                                           i18nc("@title", "Notifications"),
                                           PROJECT_VERSION,
                                           QStringLiteral(""),
                                           KAboutLicense::LicenseKey::GPL_V3,
                                           i18nc("@info:credit", "Copyright 2014 Ramon Buldó"));

    aboutData->addAuthor(i18nc("@info:credit", "Ramon Buldó"),
                         i18nc("@info:credit", "Author"),
                         QStringLiteral("ramon@manjaro.org"));

    setAboutData(aboutData);
    
    ui->setupUi(this);

    connect(ui->checkUnsupportedKernelBox, &QCheckBox::stateChanged,
            this, &PageNotifications::unsupportedKernelStateBoxChanged);
    connect(ui->checkNewKernelBox, &QCheckBox::stateChanged,
            this, &PageNotifications::newKernelStateBoxChanged);
}

PageNotifications::~PageNotifications()
{
    delete ui;
}


/***********
 * PRIVATE
 **********/
void PageNotifications::load()
{
    QSettings settings("manjaro", "manjaro-settings-manager");
    bool checkLanguagePackage = settings.value("notifications/checkLanguagePackages", true).toBool();
    bool checkUnsupportedKernel = settings.value("notifications/checkUnsupportedKernel", true).toBool();
    bool checkUnsupportedKernelRunning = settings.value("notifications/checkUnsupportedKernelRunning", false).toBool();
    bool checkNewKernel = settings.value("notifications/checkNewKernel", true).toBool();
    bool checkNewKernelLts = settings.value("notifications/checkNewKernelLts", false).toBool();
    bool checkNewKernelRecommended = settings.value("notifications/checkNewKernelRecommended", true).toBool();

    ui->checkLanguagePackage->setChecked(checkLanguagePackage);
    ui->checkUnsupportedKernelBox->setChecked(checkUnsupportedKernel);
    ui->checkUnsupportedKernelRunningBox->setChecked(checkUnsupportedKernelRunning);
    ui->checkNewKernelBox->setChecked(checkNewKernel);
    ui->checkNewKernelLtsBox->setChecked(checkNewKernelLts);
    ui->checkNewKernelRecommendedBox->setChecked(checkNewKernelRecommended);

    if(!checkUnsupportedKernel) {
        ui->checkUnsupportedKernelRunningBox->setEnabled(false);
    }
    if(!checkNewKernel) {
        ui->checkNewKernelLtsBox->setEnabled(false);
        ui->checkNewKernelRecommendedBox->setEnabled(false);
    }
}


void PageNotifications::save()
{
    bool checkLanguagePackage = ui->checkLanguagePackage->isChecked();
    bool checkUnsupportedKernel = ui->checkUnsupportedKernelBox->isChecked();
    bool checkUnsupportedKernelRunning = ui->checkUnsupportedKernelRunningBox->isChecked();
    bool checkNewKernel = ui->checkNewKernelBox->isChecked();
    bool checkNewKernelLts = ui->checkNewKernelLtsBox->isChecked();
    bool checkNewKernelRecommended = ui->checkNewKernelRecommendedBox->isChecked();

    QSettings settings("manjaro", "manjaro-settings-manager");
    settings.setValue("notifications/checkLanguagePackages", checkLanguagePackage);
    settings.setValue("notifications/checkUnsupportedKernel", checkUnsupportedKernel);
    settings.setValue("notifications/checkUnsupportedKernelRunning", checkUnsupportedKernelRunning);
    settings.setValue("notifications/checkNewKernel", checkNewKernel);
    settings.setValue("notifications/checkNewKernelLts", checkNewKernelLts);
    settings.setValue("notifications/checkNewKernelRecommended", checkNewKernelRecommended);
    settings.sync();

    switch (settings.status()) {
    case QSettings::NoError :
        qDebug() << "Your notifications settings have been saved";
        break;
    case QSettings::FormatError :
        qDebug() << "Format error when saving your notifications settings";
        break;
    case QSettings::AccessError :
        qDebug() << "Access error when saving your notifications settings";
        break;
    }
}


void PageNotifications::defaults()
{
    this->load();
}


void PageNotifications::unsupportedKernelStateBoxChanged(int checkState)
{
    switch (checkState) {
    case Qt::Unchecked :
        ui->checkUnsupportedKernelRunningBox->setEnabled(false);
        break;
    case Qt::Checked   :
        ui->checkUnsupportedKernelRunningBox->setEnabled(true);
        break;
    }
}


void PageNotifications::newKernelStateBoxChanged(int checkState)
{
    switch (checkState) {
    case Qt::Unchecked :
        ui->checkNewKernelLtsBox->setEnabled(false);
        ui->checkNewKernelRecommendedBox->setEnabled(false);
        break;
    case Qt::Checked   :
        ui->checkNewKernelLtsBox->setEnabled(true);
        ui->checkNewKernelRecommendedBox->setEnabled(true);
    }
}

#include "PageNotifications.moc"
