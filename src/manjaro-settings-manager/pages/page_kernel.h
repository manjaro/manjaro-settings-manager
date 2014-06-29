#ifndef PAGE_KERNEL_H
#define PAGE_KERNEL_H

#include "delegates/kernel_list_view_delegate.h"
#include "dialogs/applydialog.h"
#include "models/kernel.h"
#include "widgets/pagewidget.h"
#include <QDebug>
#include <QProcess>
#include <QSortFilterProxyModel>
#include <QMessageBox>

namespace Ui {
class Page_Kernel;
}

class Page_Kernel : public PageWidget
{
    Q_OBJECT

public:
    explicit Page_Kernel(QWidget *parent = 0);
    ~Page_Kernel();

public slots:
    void installButtonClicked(const QModelIndex &index);
    void infoButtonClicked(const QModelIndex &index);

private:
    Ui::Page_Kernel *ui;
    KernelModel *kernelModel;
    void installKernel(const QModelIndex &index);
    void removeKernel(const QModelIndex &index);
};

#endif // PAGE_KERNEL_H
