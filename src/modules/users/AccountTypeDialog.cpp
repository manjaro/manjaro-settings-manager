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

#include "AccountTypeDialog.h"
#include "ui_AccountTypeDialog.h"

#include <KAuth>
#include <KAuthAction>

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtWidgets/QMessageBox>

#include <QDebug>

AccountTypeDialog::AccountTypeDialog( QWidget* parent ) :
    QDialog( parent ),
    ui( new Ui::AccountTypeDialog )
{
    ui->setupUi( this );
    m_userGroupDataChanged = false;
    m_adminGroup ="wheel";

    // Hide treeWidget
    checkBoxShowGroups_toggled( false );

    ui->treeWidget->setColumnWidth( 0, 150 );
    ui->treeWidget->setColumnWidth( 1, 100 );

    // Connect signals and slots
    connect( ui->buttonCancel, SIGNAL( clicked() ) ,   this, SLOT( close() ) );
    connect( ui->buttonApply, SIGNAL( clicked() ) ,   this, SLOT( buttonApply_clicked() ) );
    connect( ui->checkBoxShowGroups, SIGNAL( toggled( bool ) ) ,   this, SLOT( checkBoxShowGroups_toggled( bool ) ) );
    connect( ui->treeWidget, SIGNAL( itemChanged( QTreeWidgetItem*,int ) )   ,   this, SLOT( treeWidget_itemChanged( QTreeWidgetItem*,int ) ) );
    connect( ui->comboBoxAccountType, SIGNAL( currentIndexChanged( int ) )   ,   this, SLOT( comboBoxAccountType_currentIndexChanged( int ) ) );
}


AccountTypeDialog::~AccountTypeDialog()
{
    delete ui;
}


bool
AccountTypeDialog::userGroupsChanged() const
{
    return m_userGroupDataChanged;
}


int
AccountTypeDialog::exec( QString username, QList<UsersCommon::Group> groups )
{
    // Block signals
    ui->treeWidget->blockSignals( true );

    m_userGroupDataChanged = false;
    this->m_username = username;
    ui->treeWidget->clear();

    foreach ( const auto group, groups )
    {
        QTreeWidgetItem* item = new QTreeWidgetItem( ui->treeWidget );
        item->setText( 0, group.name );

        if ( group.members.contains( username ) )
            item->setCheckState( 1, Qt::Checked );
        else
            item->setCheckState( 1, Qt::Unchecked );

        // Check the account type
        if ( group.name == m_adminGroup )
        {
            if ( group.members.contains( username ) )
                ui->comboBoxAccountType->setCurrentIndex( 1 );
            else
                ui->comboBoxAccountType->setCurrentIndex( 0 );
        }
    }

    // Sort tree widget
    ui->treeWidget->sortItems( 0, Qt::AscendingOrder );

    // Unblock signals
    ui->treeWidget->blockSignals( false );

    show();
    checkSudoersFile();
    return QDialog::exec();
}


void
AccountTypeDialog::checkSudoersFile()
{
    const QString sudoersFile {"/etc/sudoers"};
    QFile file( sudoersFile );
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        return;

    QTextStream in( &file );
    bool found = false;

    while ( !in.atEnd() )
    {
        QString line = in.readLine().split( "#", QString::KeepEmptyParts ).first().remove( " " );

        if ( line == "%" + m_adminGroup + "ALL=(ALL)ALL" )
        {
            found = true;
            break;
        }
    }

    file.close();

    if ( !found )
        QMessageBox::warning( this,
                              tr( "Warning!" ),
                              tr( "Admin group %1 isn't enabled in '%2'! You have to enable it to be able to set admin rights..." ).arg( m_adminGroup, sudoersFile ),
                              QMessageBox::Ok, QMessageBox::Ok );
}


void
AccountTypeDialog::buttonApply_clicked()
{
    QStringList groups;

    for ( int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i )
    {
        QTreeWidgetItem* item = ui->treeWidget->topLevelItem( i );

        if ( item->checkState( 1 ) == Qt::Checked )
            groups.append( item->text( 0 ) );
    }

    // Check if default groups have been disabled
    QStringList missingDefaultGroups;
    QStringList defaultGroups = QStringList() << "video" << "audio" << "power" << "disk" << "storage"
                                << "optical" << "network" << "lp" << "scanner";
    foreach ( QString defaultGroup, defaultGroups )
    {
        if ( !groups.contains( defaultGroup ) )
            missingDefaultGroups.append( defaultGroup );
    }

    if ( !missingDefaultGroups.isEmpty()
            && QMessageBox::No == QMessageBox::question( this,
                    tr( "Warning!" ),
                    tr( "Following default user groups have been disabled:\n%1\nIt is recommended to enable those groups. Do you really want to continue?" ).arg( missingDefaultGroups.join( ", " ) ),
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) )
        return;

    m_userGroupDataChanged = true;

    // Set groups
    KAuth::Action installAction( QLatin1String( "org.manjaro.msm.users.changeaccounttype" ) );
    installAction.setHelperId( QLatin1String( "org.manjaro.msm.users" ) );
    QVariantMap args;
    args["arguments"] = QStringList() << "-G" << groups.join( "," ) << m_username;
    installAction.setArguments( args );
    KAuth::ExecuteJob* jobAdd = installAction.execute();
    connect( jobAdd, &KAuth::ExecuteJob::newData,
             [=] ( const QVariantMap &data )
    {
        qDebug() << data;
    } );
    if ( jobAdd->exec() )
        qDebug() << "Groups set successfully";
    else
    {
        QMessageBox::warning( this, tr( "Error!" ), QString( tr( "Failed to set groups!" ) ), QMessageBox::Ok, QMessageBox::Ok );
        close();
    }
}


void
AccountTypeDialog::checkBoxShowGroups_toggled( bool toggled )
{
    if ( toggled )
        ui->treeWidget->setVisible( true );
    else
        ui->treeWidget->setVisible( false );

    adjustSize();
}


void
AccountTypeDialog::treeWidget_itemChanged( QTreeWidgetItem* item, int column )
{
    if ( item->text( 0 ) != m_adminGroup || column != 1 )
        return;

    if ( item->checkState( 1 ) == Qt::Checked )
        ui->comboBoxAccountType->setCurrentIndex( 1 );
    else
        ui->comboBoxAccountType->setCurrentIndex( 0 );
}


void
AccountTypeDialog::comboBoxAccountType_currentIndexChanged( int index )
{
    for ( int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i )
    {
        QTreeWidgetItem* item = ui->treeWidget->topLevelItem( i );
        if ( item->text( 0 ) != m_adminGroup )
            continue;

        if ( index == 0 )
            item->setCheckState( 1, Qt::Unchecked );
        else
            item->setCheckState( 1, Qt::Checked );

        break;
    }
}
