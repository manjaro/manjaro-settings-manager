#include "page_kernel.h"
#include "ui_page_kernel.h"
#include "dialogs/kernel_info_dialog.h"

Page_Kernel::Page_Kernel(QWidget *parent) :
    PageWidget(parent),
    ui(new Ui::Page_Kernel),
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

Page_Kernel::~Page_Kernel()
{
    delete ui;
    delete kernelModel;
}

void Page_Kernel::installButtonClicked(const QModelIndex &index)
{
    bool isInstalled = qvariant_cast<bool>(index.data(KernelModel::IsInstalledRole));
    if (isInstalled) {
        removeKernel(index);
    } else {
        installKernel(index);
    }
}

void Page_Kernel::installKernel(const QModelIndex &index)
{
    QStringList packageList = qvariant_cast<QStringList>(index.data(KernelModel::ModulesRole));
    QString package = qvariant_cast<QString>(index.data(KernelModel::PackageRole));
    QString version = qvariant_cast<QString>(index.data(KernelModel::VersionRole));

    QString title = QString(tr("Install Linux %1")).arg(version);
    QString message = QString(tr("Do you really want install this kernel?\n"));
    QString information = QString(tr("This will remove the following packages:\n"));
    information.append(package);
    for (auto p : packageList) {
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

void Page_Kernel::removeKernel(const QModelIndex &index)
{
    QStringList packageList = qvariant_cast<QStringList>(index.data(KernelModel::ModulesRole));
    QString package = qvariant_cast<QString>(index.data(KernelModel::PackageRole));
    QString version = qvariant_cast<QString>(index.data(KernelModel::VersionRole));

    QString title = QString(tr("Remove Linux %1")).arg(version);
    QString message = QString(tr("Do you really want remove this kernel?"));
    QString information = QString(tr("This will remove the following packages:\n"));
    information.append(package);
    for (auto p : packageList) {
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

void Page_Kernel::infoButtonClicked(const QModelIndex &index)
{
    KernelInfoDialog dialog(this);

    QString package = qvariant_cast<QString>(index.data(KernelModel::VersionRole));
    QStringList packageSplit = package.split(".");
    QString title = QString(tr("Linux %1.%2 changelog")).arg(packageSplit.at(0),packageSplit.at(1));
    QString version = QString(tr("%1%2")).arg(packageSplit.at(0),packageSplit.at(1));
    dialog.setWindowTitle(title);

    dialog.exec(version);
}
