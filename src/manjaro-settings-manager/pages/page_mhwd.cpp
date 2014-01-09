/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
 *  Ramon Buldó <rbuldo@gmail.com>
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

#include "page_mhwd.h"
#include "ui_page_mhwd.h"


Page_MHWD::Page_MHWD(QWidget *parent) :
    PageWidget(parent),
    ui(new Ui::Page_MHWD)
{
    ui->setupUi(this);
    setTitel(tr("Hardware Detection"));
    setIcon(QPixmap(":/images/resources/gpudriver.png"));
    setShowApplyButton(false);

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeWidget->setColumnWidth(0, 450);
    ui->treeWidget->setColumnWidth(1, 75);
    ui->treeWidget->setColumnWidth(2, 75);

    // Context menu actions
    installAction = new QAction(tr("Install"), ui->treeWidget);
    installAction->setIcon(QPixmap(":/images/resources/add.png"));
    removeAction = new QAction(tr("Remove"), ui->treeWidget);
    removeAction->setIcon(QPixmap(":/images/resources/remove.png"));
    forceReinstallationAction = new QAction(tr("Force Reinstallation"), ui->treeWidget);
    forceReinstallationAction->setIcon(QPixmap(":/images/resources/restore.png"));

    // Connect signals and slots
    connect(ui->buttonInstallFree, SIGNAL(clicked()),
            this, SLOT(buttonInstallFree_clicked()));
    connect(ui->buttonInstallNonFree, SIGNAL(clicked()),
            this, SLOT(buttonInstallNonFree_clicked()));
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            SLOT(showContextMenuForListWidget(const QPoint &)));
    connect(installAction, SIGNAL(triggered()),
            this, SLOT(installAction_triggered()));
    connect(removeAction, SIGNAL(triggered()),
            this, SLOT(removeAction_triggered()));
    connect(forceReinstallationAction, SIGNAL(triggered()),
            this, SLOT(forceReinstallationAction_triggered()));
    connect(ui->checkBoxShowAll, SIGNAL(toggled(bool)),
            this, SLOT(checkBoxShowAll_toggled()));
}



Page_MHWD::~Page_MHWD()
{
    delete ui;
}



void Page_MHWD::activated()
{
    ui->treeWidget->clear();
    // Create mhwd data object and fill it with hardware informations
    mhwd::Data data;
    mhwd::initData(&data);
    mhwd::fillData(&data);

    for (std::vector<mhwd::Device*>::iterator iterator = data.PCIDevices.begin();
         iterator != data.PCIDevices.end();
         iterator++)
    {
        QTreeWidgetItem *deviceItem = new QTreeWidgetItem();
        // Check if deviceClass node its already added
        QString deviceClassName = QString::fromStdString((*iterator)->className);
        QList<QTreeWidgetItem*> found = ui->treeWidget->findItems(deviceClassName, Qt::MatchCaseSensitive, 0);
        if (found.isEmpty())
        {
            QTreeWidgetItem *deviceClassItem = new QTreeWidgetItem(ui->treeWidget);
            deviceClassItem->setText(0, deviceClassName);
            deviceClassItem->addChild(deviceItem);
            if (!ui->checkBoxShowAll->isChecked())
                deviceClassItem->setHidden(true);
        }
        else
            found.first()->addChild(deviceItem);

        QString deviceName = QString::fromStdString((*iterator)->deviceName);
        QString vendorName = QString::fromStdString((*iterator)->vendorName);
        if (deviceName.isEmpty())
            deviceName = tr("Unknown device name");
        deviceItem->setText(0, QString("%1 (%2)").arg(deviceName, vendorName));


        for (std::vector<mhwd::Config*>::iterator config = (*iterator)->availableConfigs.begin();
             config != (*iterator)->availableConfigs.end();
             config++)
        {
            //Always expand and show devices with configuration
            deviceItem->parent()->setHidden(false);
            deviceItem->parent()->setExpanded(true);
            deviceItem->setExpanded(true);

            QTreeWidgetItem *item = new QTreeWidgetItem(deviceItem);
            item->setFlags(Qt::ItemIsEnabled);

            QString configName = QString::fromStdString((*config)->name);
            item->setText(0, configName);
            if ((configName.toLower().contains("nvidia") || configName.toLower().contains("nouveau")) &&
                    configName.toLower().contains("intel"))
                item->setIcon(0, QIcon(":/images/resources/intel-nvidia.png"));
            else if (configName.toLower().contains("intel"))
                item->setIcon(0, QIcon(":/images/resources/intel.png"));
            else if (configName.toLower().contains("nvidia") || configName.toLower().contains("nouveau"))
                item->setIcon(0, QIcon(":/images/resources/nvidia.png"));
            else if (configName.toLower().contains("catalyst"))
                item->setIcon(0, QIcon(":/images/resources/ati.png"));
            else
                item->setIcon(0, QIcon(":/images/resources/gpudriver.png"));

            //Check if freedriver
            if ((*config)->freedriver)
                item->setCheckState(1, Qt::Checked);
            else
                item->setCheckState(1, Qt::Unchecked);

            //Check if installed
            mhwd::Config *installedConfig = getInstalledConfig(&data, (*config)->name, (*config)->type);
            if (installedConfig == NULL)
                item->setCheckState(2, Qt::Unchecked);
            else
                item->setCheckState(2, Qt::Checked);
        }
    }

    // Free data object again
    mhwd::freeData(&data);
}



void Page_MHWD::buttonInstallFree_clicked()
{
    ApplyDialog dialog(this);
    dialog.exec("mhwd", QStringList() << "-a" << "pci" << "free" << "0300", tr("Installing free driver..."), false);
    activated();
}



void Page_MHWD::buttonInstallNonFree_clicked()
{
    ApplyDialog dialog(this);
    dialog.exec("mhwd", QStringList() << "-a" << "pci" << "nonfree" << "0300", tr("Installing non-free driver..."), false);
    activated();
}



void Page_MHWD::showContextMenuForListWidget(const QPoint &pos)
{
    QMenu contextMenu(this);
    QTreeWidgetItem* temp = ui->treeWidget->itemAt(pos);
    if (temp != NULL && (temp->text(0).contains("video-") || temp->text(0).contains("network-")) )
    {
        if(temp->checkState(2))
        {
            contextMenu.addAction(removeAction);
            contextMenu.addAction(forceReinstallationAction);
        }
        else
        {
            contextMenu.addAction(installAction);
        }
        contextMenu.exec(mapToGlobal(pos));
    }
}



void Page_MHWD::installAction_triggered()
{
    QTreeWidgetItem* temp = ui->treeWidget->currentItem();
    QString configuration = temp->text(0);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  tr("Install Configuration"),
                                  tr("Do you really want to install\n%1?").arg(configuration),
                                  QMessageBox::Ok | QMessageBox::Cancel);
    if (reply == QMessageBox::Ok)
    {
        ApplyDialog dialog(this);
        dialog.exec("mhwd", QStringList() << "-i" << "pci" << configuration, tr("Installing driver..."), false);
    }
    activated();
}



void Page_MHWD::removeAction_triggered()
{
    QTreeWidgetItem* temp = ui->treeWidget->currentItem();
    QString configuration = temp->text(0);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  tr("Remove Configuration"),
                                  tr("Do you really want to remove\n%1?").arg(configuration),
                                  QMessageBox::Ok | QMessageBox::Cancel);
    if (reply == QMessageBox::Ok)
    {
        ApplyDialog dialog(this);
        dialog.exec("mhwd", QStringList() << "-r" << "pci" << configuration, tr("Removing driver..."), false);
    }
    activated();
}



void Page_MHWD::forceReinstallationAction_triggered()
{
    QTreeWidgetItem* temp = ui->treeWidget->currentItem();
    QString configuration = temp->text(0);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  tr("Force Reinstallation"),
                                  tr("Do you really want to force the reinstallation of\n%1?").arg(configuration),
                                  QMessageBox::Ok | QMessageBox::Cancel);
    if (reply == QMessageBox::Ok)
    {
        ApplyDialog dialog(this);
        dialog.exec("mhwd", QStringList() << "-f" << "-i" << "pci" << configuration, tr("Reinstalling driver..."), false);
    }
    activated();
}

void Page_MHWD::checkBoxShowAll_toggled()
{
    activated();
}
