/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Roland Singer <roland@manjaro.org>
 *
 *  Manjaro Settings Manager is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Manjaro Settings Manager is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Manjaro Settings Manager.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ChangePasswordDialog.h"
#include "ui_ChangePasswordDialog.h"

#include <KAuth>
#include <KAuthAction>

#include <QDebug>

ChangePasswordDialog::ChangePasswordDialog( QWidget* parent ) :
    QDialog( parent ),
    ui( new Ui::ChangePasswordDialog )
{
    ui->setupUi( this );

    // Connect signals and slots
    connect( ui->buttonCancel, SIGNAL( clicked() ) ,   this, SLOT( close() ) );
    connect( ui->buttonApply, SIGNAL( clicked() ) ,   this, SLOT( buttonApply_clicked() ) );
    connect( ui->textBoxPassword, SIGNAL( textChanged( QString ) )   ,   this, SLOT( textbox_textChanged() ) );
    connect( ui->textBoxVerifiedPassword, SIGNAL( textChanged( QString ) )   ,   this, SLOT( textbox_textChanged() ) );
}


ChangePasswordDialog::~ChangePasswordDialog()
{
    delete ui;
}


int ChangePasswordDialog::exec( QString username )
{
    this->username = username;
    ui->textBoxPassword->clear();
    ui->textBoxVerifiedPassword->clear();
    ui->buttonApply->setEnabled( false );
    ui->buttonCancel->setFocus();

    return QDialog::exec();
}


void ChangePasswordDialog::textbox_textChanged()
{
    if ( ui->textBoxPassword->text().isEmpty() ||
            ui->textBoxVerifiedPassword->text().isEmpty() )
        ui->buttonApply->setEnabled( false );
    else
        ui->buttonApply->setEnabled( true );
}


void ChangePasswordDialog::buttonApply_clicked()
{
    // Check passwords
    QString password = ui->textBoxVerifiedPassword->text();
    if ( ui->textBoxPassword->text() != password )
    {
        QMessageBox::warning( this, tr( "Error!" ), tr( "Your passwords do not match!" ), QMessageBox::Ok, QMessageBox::Ok );
        return;
    }

    // Set password
    KAuth::Action installAction( QLatin1String( "org.manjaro.msm.users.changepassword" ) );
    installAction.setHelperId( QLatin1String( "org.manjaro.msm.users" ) );
    QVariantMap args;
    args["arguments"] = QStringList() << username;
    args["writeArgs"] = QStringList() << password << password;
    installAction.setArguments( args );
    KAuth::ExecuteJob* job = installAction.execute();
    connect( job, &KAuth::ExecuteJob::newData,
             [=] ( const QVariantMap &data )
    {
        qDebug() << data;
    } );
    if ( job->exec() )
        qDebug() << "Add User Job Succesfull";
    else
    {
        QMessageBox::warning( this, tr( "Error!" ), QString( tr( "Failed to set user's password!" ) ), QMessageBox::Ok, QMessageBox::Ok );
        close();
        return;
    }
    close();
}
