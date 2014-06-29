#include "page_kernel.h"
#include "ui_page_kernel.h"

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
    qDebug() << package << packageList;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  tr("Install new kernel"),
                                  tr("Do you really want install the kernel?"),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        ApplyDialog dialog(this);
        dialog.exec("pacman", QStringList() << "-S" << "--noconfirm" << package << packageList,
                    "Installing new kernel", false);
        kernelModel->update();
    }
}

void Page_Kernel::removeKernel(const QModelIndex &index)
{
    QStringList packageList = qvariant_cast<QStringList>(index.data(KernelModel::ModulesRole));
    QString package = qvariant_cast<QString>(index.data(KernelModel::PackageRole));
    qDebug() << package << packageList;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  tr("Remove kernel"),
                                  tr("Do you really want remove the kernel?"),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        ApplyDialog dialog(this);
        dialog.exec("pacman", QStringList() << "-R" << "--noconfirm" << package << packageList,
                    "Removing kernel...", false);
        kernelModel->update();
    }
}

void Page_Kernel::infoButtonClicked(const QModelIndex &index)
{
    qDebug() << "info button clicked: " << index;
    qDebug() << index.data(KernelModel::ModulesRole);
}
