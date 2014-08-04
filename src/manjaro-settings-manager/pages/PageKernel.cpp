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

#include "PageKernel.h"
#include "ui_PageKernel.h"

#include "delegates/KernelListViewDelegate.h"
#include "dialogs/KernelInfoDialog.h"
#include "dialogs/applydialog.h"

#include <QtCore/QProcess>
#include <QtWidgets/QMessageBox>

PageKernel::PageKernel(QWidget *parent) :
    PageWidget(parent),
    ui(new Ui::PageKernel),
    kernelModel(new KernelModel)
{
    ui->setupUi(this);
    setTitel(tr("Kernel"));
    setIcon(QPixmap(":/images/resources/tux-manjaro.png"));

    KernelSortFilterProxyModel *proxyKernelModel = new KernelSortFilterProxyModel(this);
    proxyKernelModel->setSourceModel(kernelModel);
    proxyKernelModel->setSortRole(KernelModel::VersionRole);
    proxyKernelModel->sort(0, Qt::DescendingOrder);
    ui->kernelListView->setModel(proxyKernelModel);

    KernelListViewDelegate *kernelListViewDelegate = new KernelListViewDelegate;
    ui->kernelListView->setItemDelegate(kernelListViewDelegate);
    connect(kernelListViewDelegate, SIGNAL(installButtonClicked(QModelIndex)),
            this, SLOT(installButtonClicked(QModelIndex)));
    connect(kernelListViewDelegate, SIGNAL(infoButtonClicked(QModelIndex)),
            this, SLOT(infoButtonClicked(QModelIndex)));
}

PageKernel::~PageKernel()
{
    delete ui;
    delete kernelModel;
}

void PageKernel::activated()
{
    kernelModel->update();
}

void PageKernel::installButtonClicked(const QModelIndex &index)
{
    bool isInstalled = qvariant_cast<bool>(index.data(KernelModel::IsInstalledRole));
    if (isInstalled) {
        removeKernel(index);
    } else {
        installKernel(index);
    }
}

void PageKernel::installKernel(const QModelIndex &index)
{
    QStringList packageList = qvariant_cast<QStringList>(index.data(KernelModel::InstalledModulesRole));
    QString package = qvariant_cast<QString>(index.data(KernelModel::PackageRole));
    QString version = qvariant_cast<QString>(index.data(KernelModel::VersionRole));

    QString title = QString(tr("Install Linux %1")).arg(version);
    QString message = QString(tr("Do you really want to install this kernel?\n"));
    QString information = QString(tr("This will install the following packages:\n"));
    information.append(package);
    for (QString p : packageList) {
        information.append("\n");
        information.append(p);
    }

    QMessageBox messageBox;
    messageBox.setIcon(QMessageBox::Question);
    messageBox.setWindowTitle(title);
    messageBox.setText(message);
    messageBox.setDetailedText(information);
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::No);

    int reply = messageBox.exec();
    if (reply == QMessageBox::Yes)
    {
        ApplyDialog dialog(this);
        QString infoText = QString(tr("Installing new kernel."));
        dialog.exec("pacman", QStringList() << "-S" << "--noconfirm" << package << packageList,
                    infoText, false);
        kernelModel->update();
    }
}

void PageKernel::removeKernel(const QModelIndex &index)
{
    QStringList packageList = qvariant_cast<QStringList>(index.data(KernelModel::InstalledModulesRole));
    QString package = qvariant_cast<QString>(index.data(KernelModel::PackageRole));
    QString version = qvariant_cast<QString>(index.data(KernelModel::VersionRole));

    QString title = QString(tr("Remove Linux %1")).arg(version);
    QString message = QString(tr("Do you really want to remove this kernel?"));
    QString information = QString(tr("This will remove the following packages:\n"));
    information.append(package);
    for (QString p : packageList) {
        information.append("\n");
        information.append(p);
    }

    QMessageBox messageBox;
    messageBox.setIcon(QMessageBox::Question);
    messageBox.setWindowTitle(title);
    messageBox.setText(message);
    messageBox.setDetailedText(information);
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::No);
    int reply = messageBox.exec();

    if (reply == QMessageBox::Yes)
    {
        ApplyDialog dialog(this);
        QString infoText = QString(tr("Removing kernel..."));
        dialog.exec("pacman", QStringList() << "-R" << "--noconfirm" << package << packageList,
                    infoText, false);
        kernelModel->update();
    }
}

void PageKernel::infoButtonClicked(const QModelIndex &index)
{
    KernelInfoDialog dialog(this);

    QString package = qvariant_cast<QString>(index.data(KernelModel::PackageRole));
    QString version = qvariant_cast<QString>(index.data(KernelModel::VersionRole));
    QStringList versionSplit = version.split(".");
    QString title = QString(tr("Linux %1.%2 changelog")).arg(versionSplit.at(0), versionSplit.at(1));
    dialog.setWindowTitle(title);
    dialog.setPackage(package);
    dialog.exec();
}
