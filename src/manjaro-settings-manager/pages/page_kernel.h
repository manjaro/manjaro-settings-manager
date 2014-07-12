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
