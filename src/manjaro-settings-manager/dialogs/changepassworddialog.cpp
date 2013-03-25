/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
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

#include "changepassworddialog.h"
#include "ui_changepassworddialog.h"


ChangePasswordDialog::ChangePasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePasswordDialog)
{
    ui->setupUi(this);

    // Connect signals and slots
    connect(ui->buttonCancel, SIGNAL(clicked()) ,   this, SLOT(close()));
    connect(ui->buttonApply, SIGNAL(clicked()) ,   this, SLOT(buttonApply_clicked()));
    connect(ui->textBoxPassword, SIGNAL(textChanged(QString))   ,   this, SLOT(textbox_textChanged()));
    connect(ui->textBoxVerifiedPassword, SIGNAL(textChanged(QString))   ,   this, SLOT(textbox_textChanged()));
}



ChangePasswordDialog::~ChangePasswordDialog()
{
    delete ui;
}



int ChangePasswordDialog::exec(QString username) {
    this->username = username;
    ui->textBoxPassword->clear();
    ui->textBoxVerifiedPassword->clear();
    ui->buttonApply->setEnabled(false);
    ui->buttonCancel->setFocus();

    return QDialog::exec();
}



//###
//### Private
//###


void ChangePasswordDialog::textbox_textChanged() {
    if (ui->textBoxPassword->text().isEmpty() ||
            ui->textBoxVerifiedPassword->text().isEmpty())
        ui->buttonApply->setEnabled(false);
    else
        ui->buttonApply->setEnabled(true);
}



void ChangePasswordDialog::buttonApply_clicked() {
    // Check passwords
    if (ui->textBoxPassword->text() != ui->textBoxVerifiedPassword->text()) {
        QMessageBox::warning(this, tr("Error!"), tr("Your passwords do not match!"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }


    // Set password
    QString errorMessage;
    if (Global::runProcess("passwd",
                           QStringList() << username,
                           QStringList() << ui->textBoxPassword->text() << ui->textBoxPassword->text(),
                           errorMessage) != 0)
        QMessageBox::warning(this, tr("Error!"), QString(tr("Failed to set user's password!") + "\n" + errorMessage), QMessageBox::Ok, QMessageBox::Ok);

    close();
}
