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

    // Connect signals and slots
    connect(ui->buttonInstallFree, SIGNAL(clicked())  ,   this, SLOT(buttonInstallFree_clicked()));
    connect(ui->buttonInstallNonFree, SIGNAL(clicked())  ,   this, SLOT(buttonInstallNonFree_clicked()));
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
        for (std::vector<mhwd::Config*>::iterator ci = (*iterator)->availableConfigs.begin(); ci != (*iterator)->availableConfigs.end(); ci++) {
            QString freedriver, name;
            name = QString::fromStdString((*ci)->name);

            if (addedToList.contains(name))
                continue;

            addedToList.append(name);

            if ((*ci)->freedriver)
                freedriver = tr("free");
            else
                freedriver = tr("nonfree");

            QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
            item->setText(QString("%1 (%2)").arg(name, freedriver));
            //item->setBackgroundColor("green");

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
