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
