#ifndef KERNEL_INFO_DIALOG_H
#define KERNEL_INFO_DIALOG_H

#include <QDialog>
#include <QWebView>
#include <QDebug>

namespace Ui {
class KernelInfoDialog;
}

class KernelInfoDialog : public QDialog
{
    Q_OBJECT

public slots:
    int exec();

public:
    explicit KernelInfoDialog(QWidget *parent = 0);
    ~KernelInfoDialog();

private:
    Ui::KernelInfoDialog *ui;
};

#endif // KERNELINFODIALOG_H
