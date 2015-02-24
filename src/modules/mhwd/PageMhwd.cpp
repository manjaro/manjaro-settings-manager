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

#include "PageMhwd.h"
#include "ui_PageMhwd.h"
#include "libmhwd/mhwd.h"

#include <KAboutData>
#include <KAuth>
#include <KAuthAction>

#include <QtWidgets/QMessageBox>

#include <KPluginFactory>
K_PLUGIN_FACTORY(MsmMhwdFactory,
                 registerPlugin<PageMhwd>("msm_mhwd");)

PageMhwd::PageMhwd(QWidget *parent, const QVariantList &args) :
    KCModule(parent, args),
    ui(new Ui::PageMhwd)
{
    KAboutData *aboutData = new KAboutData("msm_mhwd",
                                           i18nc("@title", "Hardware Configuration"),
                                           PROJECT_VERSION,
                                           QStringLiteral(""),
                                           KAboutLicense::LicenseKey::GPL_V3,
                                           i18nc("@info:credit", "Copyright 2014 Ramon Buldó"));

    aboutData->addAuthor(i18nc("@info:credit", "Ramon Buldó"),
                         i18nc("@info:credit", "Author"),
                         QStringLiteral("ramon@manjaro.org"));

    setAboutData(aboutData);

    ui->setupUi(this);

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeWidget->setColumnWidth(0, 450);
    ui->treeWidget->setColumnWidth(1, 100);
    ui->treeWidget->setColumnWidth(2, 100);

    // Context menu actions
    installAction = new QAction(tr("Install"), ui->treeWidget);
    installAction->setIcon(QPixmap(":/icons/add.png"));
    removeAction = new QAction(tr("Remove"), ui->treeWidget);
    removeAction->setIcon(QPixmap(":/icons/remove.png"));
    forceReinstallationAction = new QAction(tr("Force Reinstallation"), ui->treeWidget);
    forceReinstallationAction->setIcon(QPixmap(":/icons/restore.png"));

    // Connect signals and slots
    connect(ui->buttonInstallFree, &QPushButton::clicked,
            this, &PageMhwd::buttonInstallFree_clicked);
    connect(ui->buttonInstallNonFree, &QPushButton::clicked,
            this, &PageMhwd::buttonInstallNonFree_clicked);
    connect(ui->treeWidget, &QTreeWidget::customContextMenuRequested,
            this, &PageMhwd::showContextMenuForTreeWidget);
    connect(installAction, &QAction::triggered,
            this, &PageMhwd::installAction_triggered);
    connect(removeAction, &QAction::triggered,
            this, &PageMhwd::removeAction_triggered);
    connect(forceReinstallationAction, &QAction::triggered,
            this, &PageMhwd::forceReinstallationAction_triggered);
    connect(ui->checkBoxShowAll, &QCheckBox::toggled,
            this, &PageMhwd::checkBoxShowAll_toggled);
}



PageMhwd::~PageMhwd()
{
    delete ui;
}



void PageMhwd::load()
{
    ui->treeWidget->clear();
    ui->buttonInstallFree->setEnabled(false);
    ui->buttonInstallNonFree->setEnabled(false);
    // Create mhwd data object and fill it with hardware informations
    mhwd::Data data;
    mhwd::initData(&data);
    mhwd::fillData(&data);

    for (std::vector<mhwd::Device*>::iterator dev_iter = data.PCIDevices.begin();
         dev_iter != data.PCIDevices.end();
         dev_iter++) {
        QTreeWidgetItem *deviceItem = new QTreeWidgetItem();
        // Check if deviceClass node its already added
        QString deviceClassName = QString::fromStdString((*dev_iter)->className);
        QList<QTreeWidgetItem*> found = ui->treeWidget->findItems(deviceClassName, Qt::MatchCaseSensitive, 0);
        if (found.isEmpty()) {
            QTreeWidgetItem *deviceClassItem = new QTreeWidgetItem(ui->treeWidget);
            deviceClassItem->setText(0, deviceClassName);
            deviceClassItem->addChild(deviceItem);
            if (!ui->checkBoxShowAll->isChecked()) {
                deviceClassItem->setHidden(true);
            }
        }
        else
            found.first()->addChild(deviceItem);

        QString deviceName = QString::fromStdString((*dev_iter)->deviceName);
        QString vendorName = QString::fromStdString((*dev_iter)->vendorName);
        if (deviceName.isEmpty()) {
            deviceName = tr("Unknown device name");
        }
        deviceItem->setText(0, QString("%1 (%2)").arg(deviceName, vendorName));


        for (std::vector<mhwd::Config*>::iterator conf_iter = (*dev_iter)->availableConfigs.begin();
             conf_iter != (*dev_iter)->availableConfigs.end(); conf_iter++) {
            //Always expand and show devices with configuration
            deviceItem->parent()->setHidden(false);
            deviceItem->parent()->setExpanded(true);
            deviceItem->setExpanded(true);

            QTreeWidgetItem *item = new QTreeWidgetItem(deviceItem);
            item->setFlags(Qt::ItemIsEnabled);

            QString configName = QString::fromStdString((*conf_iter)->name);
            item->setText(0, configName);
            if ((configName.toLower().contains("nvidia") || configName.toLower().contains("nouveau")) &&
                    configName.toLower().contains("intel")) {
                item->setIcon(0, QIcon(":/icons/intel-nvidia.png"));
            } else if (configName.toLower().contains("intel")) {
                item->setIcon(0, QIcon(":/icons/intel.png"));
            } else if (configName.toLower().contains("nvidia") || configName.toLower().contains("nouveau")) {
                item->setIcon(0, QIcon(":/icons/nvidia.png"));
            } else if (configName.toLower().contains("catalyst")) {
                item->setIcon(0, QIcon(":/icons/ati.png"));
            } else {
                item->setIcon(0, QIcon(":/icons/gpudriver.png"));
            }

            //Check if freedriver
            if ((*conf_iter)->freedriver) {
                item->setCheckState(1, Qt::Checked);
                ui->buttonInstallFree->setEnabled(true);
            }
            else {
                item->setCheckState(1, Qt::Unchecked);
                ui->buttonInstallNonFree->setEnabled(true);
            }

            //Check if installed
            mhwd::Config *installedConfig = getInstalledConfig(&data, (*conf_iter)->name, (*conf_iter)->type);
            if (installedConfig == NULL) {
                item->setCheckState(2, Qt::Unchecked);
            }
            else {
                item->setCheckState(2, Qt::Checked);
            }
        }
    }
    // Free data object again
    mhwd::freeData(&data);
}

void PageMhwd::save()
{

}

void PageMhwd::defaults()
{
    this->load();
}



void PageMhwd::buttonInstallFree_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  tr("Auto Install Configuration"),
                                  tr("Do you really want to auto install\n the open-source graphic driver?"),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        KAuth::Action installAction(QLatin1String("org.manjaro.msm.mhwd.remove"));
        installAction.setHelperId(QLatin1String("org.manjaro.msm.mhwd"));
        QVariantMap args;
        args["arguments"] = QStringList() << "-a" << "pci" << "free" << "0300";
        installAction.setArguments(args);
        KAuth::ExecuteJob *job = installAction.execute();
        connect(job, &KAuth::ExecuteJob::newData,
                [=] (const QVariantMap &data)
        {
            qDebug() << data;
        });
        if (job->exec()) {
            qDebug() << "Mwhd install Job Succesfull";
        } else {
            qDebug() << "Mwhd install Job Failed";
        }
        //ApplyDialog dialog(this);
        //dialog.exec("mhwd", QStringList() << "-a" << "pci" << "free" << "0300", tr("Installing open-source graphic driver..."), false);
    }
    load();
}



void PageMhwd::buttonInstallNonFree_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  tr("Auto Install Configuration"),
                                  tr("Do you really want to auto install\n the proprietary graphic driver?"),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        KAuth::Action installAction(QLatin1String("org.manjaro.msm.mhwd.remove"));
        installAction.setHelperId(QLatin1String("org.manjaro.msm.mhwd"));
        QVariantMap args;
        args["arguments"] = QStringList() << "-a" << "pci" << "nonfree" << "0300";
        installAction.setArguments(args);
        KAuth::ExecuteJob *job = installAction.execute();
        connect(job, &KAuth::ExecuteJob::newData,
                [=] (const QVariantMap &data)
        {
            qDebug() << data;
        });
        if (job->exec()) {
            qDebug() << "Mwhd install Job Succesfull";
        } else {
            qDebug() << "Mwhd install Job Failed";
        }
        //ApplyDialog dialog(this);
        //dialog.exec("mhwd", QStringList() << "-a" << "pci" << "nonfree" << "0300", tr("Installing proprietary graphic driver..."), false);
    }
    load();
}



void PageMhwd::showContextMenuForTreeWidget(const QPoint &pos)
{
    QMenu contextMenu(this);
    QTreeWidgetItem* temp = ui->treeWidget->itemAt(pos);
    if ((temp != NULL) && (temp->text(0).contains("video-") || temp->text(0).contains("network-"))) {
        if(temp->checkState(2)) {
            contextMenu.addAction(removeAction);
            contextMenu.addAction(forceReinstallationAction);
        }
        else {
            contextMenu.addAction(installAction);
        }
        contextMenu.exec(ui->treeWidget->viewport()->mapToGlobal(pos));
    }
}



void PageMhwd::installAction_triggered()
{
    QTreeWidgetItem* temp = ui->treeWidget->currentItem();
    QString configuration = temp->text(0);

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  tr("Install Configuration"),
                                  tr("Do you really want to install\n%1?").arg(configuration),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        KAuth::Action installAction(QLatin1String("org.manjaro.msm.mhwd.install"));
        installAction.setHelperId(QLatin1String("org.manjaro.msm.mhwd"));
        QVariantMap args;
        args["arguments"] = QStringList() << "-i" << "pci" << configuration;
        installAction.setArguments(args);
        KAuth::ExecuteJob *job = installAction.execute();
        connect(job, &KAuth::ExecuteJob::newData,
                [=] (const QVariantMap &data)
        {
            qDebug() << data;
        });
        if (job->exec()) {
            qDebug() << "Mwhd install Job Succesfull";
        } else {
            qDebug() << "Mwhd install Job Failed";
        }
        //ApplyDialog dialog(this);
        //dialog.exec("mhwd", QStringList() << "-i" << "pci" << configuration, tr("Installing driver..."), false);
    }
    load();
}



void PageMhwd::removeAction_triggered()
{
    QTreeWidgetItem* temp = ui->treeWidget->currentItem();
    QString configuration = temp->text(0);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  tr("Remove Configuration"),
                                  tr("Do you really want to remove\n%1?").arg(configuration),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        KAuth::Action installAction(QLatin1String("org.manjaro.msm.mhwd.remove"));
        installAction.setHelperId(QLatin1String("org.manjaro.msm.mhwd"));
        QVariantMap args;
        args["arguments"] = QStringList() << "-r" << "pci" << configuration;
        installAction.setArguments(args);
        KAuth::ExecuteJob *job = installAction.execute();
        connect(job, &KAuth::ExecuteJob::newData,
                [=] (const QVariantMap &data)
        {
            qDebug() << data;
        });
        if (job->exec()) {
            qDebug() << "Mwhd install Job Succesfull";
        } else {
            qDebug() << "Mwhd install Job Failed";
        }
        //ApplyDialog dialog(this);
        //dialog.exec("mhwd", QStringList() << "-r" << "pci" << configuration, tr("Removing driver..."), false);
    }
    load();
}



void PageMhwd::forceReinstallationAction_triggered()
{
    QTreeWidgetItem* temp = ui->treeWidget->currentItem();
    QString configuration = temp->text(0);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  tr("Force Reinstallation"),
                                  tr("Do you really want to force the reinstallation of\n%1?").arg(configuration),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        KAuth::Action installAction(QLatin1String("org.manjaro.msm.mhwd.install"));
        installAction.setHelperId(QLatin1String("org.manjaro.msm.mhwd"));
        QVariantMap args;
        args["arguments"] = QStringList() << "-f" << "-i" << "pci" << configuration;
        installAction.setArguments(args);
        KAuth::ExecuteJob *job = installAction.execute();
        connect(job, &KAuth::ExecuteJob::newData,
                [=] (const QVariantMap &data)
        {
            qDebug() << data;
        });
        if (job->exec()) {
            qDebug() << "Mwhd install Job Succesfull";
        } else {
            qDebug() << "Mwhd install Job Failed";
        }
        //ApplyDialog dialog(this);
        //dialog.exec("mhwd", QStringList() << "-f" << "-i" << "pci" << configuration, tr("Reinstalling driver..."), false);
    }
    load();
}

void PageMhwd::checkBoxShowAll_toggled()
{
    load();
}

#include "PageMhwd.moc"
