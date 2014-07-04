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

void KernelInfoDialog::setPackage(const QString &package)
{
    m_package = package;
}

int KernelInfoDialog::exec()
{
    ui->webView->load(QUrl(QString("qrc:///kernel_changelogs/resources/%1.html").arg(m_package)));
    ui->webView->show();
    return QDialog::exec();
}
