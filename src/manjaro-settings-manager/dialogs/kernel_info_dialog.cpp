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

void KernelInfoDialog::setVersion(const QString &version)
{
    m_version = version;
}

int KernelInfoDialog::exec()
{
    ui->webView->load(QUrl("qrc:///kernel_changelogs/resources/linux%1.html").arg(m_version));
    ui->webView->show();
    return QDialog::exec();
}
