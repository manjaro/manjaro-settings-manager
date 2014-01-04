/*
 *  Manjaro Installation Framework
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

#include "page_gpudriver.h"
#include "ui_page_gpudriver.h"


Page_GPUDriver::Page_GPUDriver(QWidget *parent) :
    PageWidget(parent),
    ui(new Ui::Page_GPUDriver)
{
    ui->setupUi(this);
    setTitel(tr("Graphics Driver"));
    setIcon(QPixmap(":/images/resources/gpudriver.png"));
    setShowApplyButton(true);
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Connect signals and slots
    connect(ui->buttonInstallFree, SIGNAL(clicked()),
            this, SLOT(buttonInstallFree_clicked()));
    connect(ui->buttonInstallNonFree, SIGNAL(clicked()),
            this, SLOT(buttonInstallNonFree_clicked()));
    connect(ui->listWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            SLOT(showContextMenuForListWidget(const QPoint &)));
}



Page_GPUDriver::~Page_GPUDriver()
{
    delete ui;
}



void Page_GPUDriver::activated() {

    ui->listWidget->clear();
    // Create mhwd data object and fill it with hardware informations
    mhwd::Data data;
    mhwd::initData(&data);
    mhwd::fillData(&data);

    QStringList addedToList;

    for (std::vector<mhwd::Device*>::iterator iterator = data.PCIDevices.begin(); iterator != data.PCIDevices.end(); iterator++) {
        for (std::vector<mhwd::Config*>::iterator config = (*iterator)->availableConfigs.begin(); config != (*iterator)->availableConfigs.end(); config++) {
            QString freedriver, name, installed;
            name = QString::fromStdString((*config)->name);

            //Check if already in the list
            if (addedToList.contains(name))
                continue;
            addedToList.append(name);

            //Check if installed
            installed = ", ";
            mhwd::Config *installedConfig = getInstalledConfig(&data, (*config)->name, (*config)->type);
            if (installedConfig == NULL)
                installed = "";
            else if (installedConfig->basePath != (*config)->basePath) {
                installed.append(tr("custom installed"));
            }
            else
                installed.append(tr("installed"));

            //Check if freedriver
            if ((*config)->freedriver)
                freedriver = tr("free");
            else
                freedriver = tr("nonfree");

            QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
            item->setText(QString("%1 (%2%3)").arg(name, freedriver, installed));

            if (name.toLower().contains("nvidia") && name.toLower().contains("intel"))
                item->setIcon(QIcon(":/images/resources/intel-nvidia.png"));
            else if (name.toLower().contains("intel"))
                item->setIcon(QIcon(":/images/resources/intel.png"));
            else if (name.toLower().contains("nvidia"))
                item->setIcon(QIcon(":/images/resources/nvidia.png"));
            else if (name.toLower().contains("catalyst"))
                item->setIcon(QIcon(":/images/resources/ati.png"));
            else
                item->setIcon(QIcon(":/images/resources/gpudriver.png"));
       }
    }
    // Free data object again
    mhwd::freeData(&data);
}

void Page_GPUDriver::buttonInstallFree_clicked() {
    ApplyDialog dialog(this);
    dialog.exec("mhwd", QStringList() << "-a" << "pci" << "free" << "0300", tr("Installing free driver..."), false);
}

void Page_GPUDriver::buttonInstallNonFree_clicked() {
    ApplyDialog dialog(this);
    dialog.exec("mhwd", QStringList() << "-a" << "pci" << "nonfree" << "0300", tr("Installing non-free driver..."), false);
}

void Page_GPUDriver::showContextMenuForListWidget(const QPoint &pos)
{
    QMenu contextMenu(this);
    QListWidgetItem* temp = ui->listWidget->itemAt(pos);
    if (temp != NULL)
    {
        if(temp->text().contains("installed"))
        {
            contextMenu.addAction(new QAction(tr("Remove"), this));
            contextMenu.addAction(new QAction(tr("Force Reinstall"), this));
        }
        else
            contextMenu.addAction(new QAction(tr("Install"), this));
        contextMenu.exec(mapToGlobal(pos));
    }
}
