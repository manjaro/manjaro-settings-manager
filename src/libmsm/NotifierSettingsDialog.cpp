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

#include "NotifierSettingsDialog.h"
#include "ui_NotifierSettingsDialog.h"

#include <QDebug>

NotifierSettingsDialog::NotifierSettingsDialog( QWidget* parent ) :
    QDialog( parent ),
    ui( new Ui::NotifierSettingsDialog )
{
    ui->setupUi( this );


    this->setWindowTitle(tr( "Notifications settings" ));
    this->setWindowIcon(QIcon::fromTheme("gtk-preferences"));
    ui->buttonApply->setIcon(QIcon::fromTheme("dialog-ok-apply"));
    ui->buttonQuit->setIcon(QIcon::fromTheme("gtk-quit"));

    connect( ui->checkUnsupportedKernelBox, &QCheckBox::stateChanged,
             this, &NotifierSettingsDialog::unsupportedKernelStateBoxChanged );
    connect( ui->checkNewKernelBox, &QCheckBox::stateChanged,
             this, &NotifierSettingsDialog::newKernelStateBoxChanged );
    connect( ui->buttonApply, &QPushButton::clicked,
             this, &NotifierSettingsDialog::buttonApply_clicked );
    connect( ui->buttonQuit, &QPushButton::clicked,
             this, &NotifierSettingsDialog::buttonQuit_clicked );

    connect( ui->checkLanguagePackage, &QCheckBox::stateChanged,
             [=] ( )
    {
        ui->buttonApply->setEnabled(true);
    } );
    connect( ui->checkUnsupportedKernelBox, &QCheckBox::stateChanged,
             [=] ( )
    {
        ui->buttonApply->setEnabled(true);
    } );
    connect( ui->checkUnsupportedKernelRunningBox, &QCheckBox::stateChanged,
             [=] ( )
    {
        ui->buttonApply->setEnabled(true);
    } );
    connect( ui->checkNewKernelBox, &QCheckBox::stateChanged,
             [=] ( )
    {
        ui->buttonApply->setEnabled(true);
    } );
    connect( ui->checkNewKernelLtsBox, &QCheckBox::stateChanged,
             [=] ( )
    {
        ui->buttonApply->setEnabled(true);
    } );
    connect( ui->checkNewKernelRecommendedBox, &QCheckBox::stateChanged,
             [=] ( )
    {
        ui->buttonApply->setEnabled(true);
    } );

    this->load();

    ui->buttonApply->setEnabled(false);
}


NotifierSettingsDialog::~NotifierSettingsDialog()
{
    delete ui;
}

void
NotifierSettingsDialog::buttonQuit_clicked()
{
    close();
}

void
NotifierSettingsDialog::buttonApply_clicked()
{
    QMessageBox msgBox;

    QSettings::Status result = this->save();
    switch ( result )
    {
    case QSettings::NoError :
        qDebug() << "Your notifications settings have been saved";
        break;
    case QSettings::FormatError :
        qDebug() << "Format error when saving your notifications settings";
        msgBox.setText( tr("Format error when saving your notifications settings") );
        break;
    case QSettings::AccessError :
        qDebug() << "Access error when saving your notifications settings";
        msgBox.setText( tr("Access error when saving your notifications settings") );
        break;
    }
    if( result == QSettings::NoError )
        ui->buttonApply->setEnabled( false );
    else
        msgBox.exec();
}

void
NotifierSettingsDialog::load()
{
    QSettings settings( "manjaro", "manjaro-settings-manager" );
    bool checkLanguagePackage = settings.value( "notifications/checkLanguagePackages", true ).toBool();
    bool checkUnsupportedKernel = settings.value( "notifications/checkUnsupportedKernel", true ).toBool();
    bool checkUnsupportedKernelRunning = settings.value( "notifications/checkUnsupportedKernelRunning", false ).toBool();
    bool checkNewKernel = settings.value( "notifications/checkNewKernel", true ).toBool();
    bool checkNewKernelLts = settings.value( "notifications/checkNewKernelLts", false ).toBool();
    bool checkNewKernelRecommended = settings.value( "notifications/checkNewKernelRecommended", true ).toBool();

    ui->checkLanguagePackage->setChecked( checkLanguagePackage );
    ui->checkUnsupportedKernelBox->setChecked( checkUnsupportedKernel );
    ui->checkUnsupportedKernelRunningBox->setChecked( checkUnsupportedKernelRunning );
    ui->checkNewKernelBox->setChecked( checkNewKernel );
    ui->checkNewKernelLtsBox->setChecked( checkNewKernelLts );
    ui->checkNewKernelRecommendedBox->setChecked( checkNewKernelRecommended );

    if ( !checkUnsupportedKernel )
        ui->checkUnsupportedKernelRunningBox->setEnabled( false );
    if ( !checkNewKernel )
    {
        ui->checkNewKernelLtsBox->setEnabled( false );
        ui->checkNewKernelRecommendedBox->setEnabled( false );
    }
}


QSettings::Status
NotifierSettingsDialog::save()
{
    bool checkLanguagePackage = ui->checkLanguagePackage->isChecked();
    bool checkUnsupportedKernel = ui->checkUnsupportedKernelBox->isChecked();
    bool checkUnsupportedKernelRunning = ui->checkUnsupportedKernelRunningBox->isChecked();
    bool checkNewKernel = ui->checkNewKernelBox->isChecked();
    bool checkNewKernelLts = ui->checkNewKernelLtsBox->isChecked();
    bool checkNewKernelRecommended = ui->checkNewKernelRecommendedBox->isChecked();

    QSettings settings( "manjaro", "manjaro-settings-manager" );
    settings.setValue( "notifications/checkLanguagePackages", checkLanguagePackage );
    settings.setValue( "notifications/checkUnsupportedKernel", checkUnsupportedKernel );
    settings.setValue( "notifications/checkUnsupportedKernelRunning", checkUnsupportedKernelRunning );
    settings.setValue( "notifications/checkNewKernel", checkNewKernel );
    settings.setValue( "notifications/checkNewKernelLts", checkNewKernelLts );
    settings.setValue( "notifications/checkNewKernelRecommended", checkNewKernelRecommended );
    settings.sync();
    return settings.status();
}


void
NotifierSettingsDialog::unsupportedKernelStateBoxChanged( int checkState )
{
    switch ( checkState )
    {
    case Qt::Unchecked :
        ui->checkUnsupportedKernelRunningBox->setEnabled( false );
        break;
    case Qt::Checked   :
        ui->checkUnsupportedKernelRunningBox->setEnabled( true );
        break;
    }
}


void
NotifierSettingsDialog::newKernelStateBoxChanged( int checkState )
{
    switch ( checkState )
    {
    case Qt::Unchecked :
        ui->checkNewKernelLtsBox->setEnabled( false );
        ui->checkNewKernelRecommendedBox->setEnabled( false );
        break;
    case Qt::Checked   :
        ui->checkNewKernelLtsBox->setEnabled( true );
        ui->checkNewKernelRecommendedBox->setEnabled( true );
    }
}
