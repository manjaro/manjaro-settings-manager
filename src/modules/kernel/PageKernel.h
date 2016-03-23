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

#include "KernelModel.h"
#include "PageWidget.h"
#include "KernelInfoDialog.h"


namespace Ui
{
class PageKernel;
}

class PageKernel : public PageWidget
{
    Q_OBJECT

public:
    explicit PageKernel( QWidget* parent = 0 );
    ~PageKernel();
    void load();

public slots:
    void installButtonClicked( const QModelIndex& index );
    void infoButtonClicked( const QModelIndex& index );

private:
    Ui::PageKernel* ui;
    KernelModel* kernelModel;
    KernelInfoDialog* kernelInfoDialog;
    void installKernel( const QModelIndex& index );
    void removeKernel( const QModelIndex& index );
};

#endif // PAGE_KERNEL_H
