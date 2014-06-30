#ifndef KERNEL_INFO_DIALOG_H
#define KERNEL_INFO_DIALOG_H

#include <QDialog>

namespace Ui {
class kernel_info_dialog;
}

class kernel_info_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit kernel_info_dialog(QWidget *parent = 0);
    ~kernel_info_dialog();

private:
    Ui::kernel_info_dialog *ui;
};

#endif // KERNEL_INFO_DIALOG_H
