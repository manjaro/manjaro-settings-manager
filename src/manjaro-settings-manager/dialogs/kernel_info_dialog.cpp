#include "kernel_info_dialog.h"
#include "ui_kernel_info_dialog.h"

KernelInfoDialog::KernelInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KernelInfoDialog)
{
    ui->setupUi(this);
}

KernelInfoDialog::~KernelInfoDialog()
{
    delete ui;
}

int KernelInfoDialog::exec(version)
{
    ui->webView->load(QUrl("qrc:///kernel_changelogs/resources/linux%1.html").arg(version));
    ui->webView->show();
    return QDialog::exec();
}
