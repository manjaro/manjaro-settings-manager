#include "kernel_info_dialog.h"
#include "ui_kernel_info_dialog.h"

kernel_info_dialog::kernel_info_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::kernel_info_dialog)
{
    ui->setupUi(this);
}

kernel_info_dialog::~kernel_info_dialog()
{
    delete ui;
}
