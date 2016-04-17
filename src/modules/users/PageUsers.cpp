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

#include "PageUsers.h"
#include "ui_PageUsers.h"


PageUsers::PageUsers( QWidget* parent ) :
    PageWidget( parent ),
    ui( new Ui::PageUsers )
{
    ui->setupUi( this );
    setTitle( tr( "User Accounts" ) );
    setIcon( QPixmap( ":/images/resources/users.png" ) );

    // Connect signals and slots
    connect( ui->listWidget, SIGNAL( currentItemChanged( QListWidgetItem*,QListWidgetItem* ) )   ,   this, SLOT( setupUserData( QListWidgetItem* ) ) );
    connect( ui->buttonImage, SIGNAL( clicked() )  ,   this, SLOT( buttonImage_clicked() ) );
    connect( ui->buttonAdd, SIGNAL( clicked() )    ,   this, SLOT( buttonAddUser_clicked() ) );
    connect( ui->buttonRemove, SIGNAL( clicked() ) ,   this, SLOT( buttonRemoveUser_clicked() ) );
    connect( ui->buttonPassword, SIGNAL( clicked() )   ,   this, SLOT( buttonChangePassword_clicked() ) );
    connect( ui->buttonAccountType, SIGNAL( clicked() )    ,   this, SLOT( buttonChangeAccountType_clicked() ) );
}


PageUsers::~PageUsers()
{
    delete ui;
}


void
PageUsers::load()
{
    ui->listWidget->clear();

    QList<UsersCommon::User> users = UsersCommon::getAllUsers();

    for ( int i = 0; i < users.size(); i++ )
    {
        const UsersCommon::User* user = &users.at( i );

        ListWidgetItem* item = new ListWidgetItem( ui->listWidget );
        item->setText( user->username );
        item->user = *user;

        if ( QFile::exists( user->homePath + "/.face" ) )
            item->setIcon( QIcon( user->homePath + "/.face" ) );
        else
            item->setIcon( QIcon( ":/images/resources/user.png" ) );
    }

    setupUserData( ui->listWidget->currentItem() );
}


void
PageUsers::setupUserData( QListWidgetItem* current )
{
    if ( !current )
    {
        ui->buttonImage->setIcon( QIcon( ":/images/resources/user.png" ) );
        ui->labelUsername->setText( "" );
        ui->buttonAccountType->setText( tr( "Standard" ) );
        ui->userWidget->setEnabled( false );
        return;
    }

    ui->buttonImage->setIcon( current->icon() );
    ui->labelUsername->setText( current->text() );
    ui->buttonAccountType->setText( tr( "Standard" ) );
    ui->userWidget->setEnabled( true );

    QList<UsersCommon::Group> groups = UsersCommon::getAllGroups();

    for ( int i = 0; i < groups.size(); i++ )
    {
        const UsersCommon::Group* group = &groups.at( i );
        if ( group->name != QStringLiteral("wheel") || !group->members.contains( current->text() ) )
            continue;

        ui->buttonAccountType->setText( tr( "Administrator" ) );
        break;
    }
}


void
PageUsers::buttonImage_clicked()
{
    ListWidgetItem* item = dynamic_cast<ListWidgetItem*>( ui->listWidget->currentItem() );
    if ( !item )
        return;

    PreviewFileDialog dialog( this );
    dialog.setFileMode( QFileDialog::ExistingFile );
    dialog.setNameFilter( tr( "Images (*.png *.jpg *.bmp)" ) );
    dialog.setViewMode( QFileDialog::Detail );
    dialog.setDirectory( QStringLiteral( "/usr/share/pixmaps/faces" ) );

    if ( !dialog.exec() || dialog.selectedFiles().isEmpty() )
        return;

    QString fileName = dialog.selectedFiles().first();
    QString fileNameEnding = fileName.split( ".", QString::KeepEmptyParts ).last();

    // Set icon in our GUI
    item->setIcon( QIcon( fileName ) );
    ui->buttonImage->setIcon( item->icon() );


    // Paths to copy/symlink
    QStringList copyDest, symlinkHomeDest;

    // Fill lists
    copyDest << QString( "%1/.face" ).arg( item->user.homePath )
             << QString( "/var/lib/AccountsService/icons/%1" ).arg( item->user.username )
             << QString( "/usr/share/apps/sddm/faces/%1.face.icon" ).arg( item->user.username );

    symlinkHomeDest << QString( "%1/.face.icon" ).arg( item->user.homePath )
                    << QString( "%1/.icon" ).arg( item->user.homePath );


    // Structs to set permission
    struct passwd* pwd = getpwnam( item->user.username.toStdString().c_str() );
    struct group* grp = getgrnam( "users" );

    if ( !pwd || !grp )
    {
        QMessageBox::warning( this, tr( "Error!" ), tr( "Failed to get user permission structures!" ), QMessageBox::Ok, QMessageBox::Ok );
        return;
    }


    // Copy files
    foreach ( QString dest, copyDest )
    {
        if ( !QDir( dest.mid( 0, dest.lastIndexOf( "/" ) ) ).exists() )
            continue;

        if ( QFile::exists( dest ) )
            QFile::remove( dest );

        if ( !QFile::copy( fileName, dest ) )
        {
            QMessageBox::warning( this, tr( "Error!" ), tr( "Failed to copy image to '%1'!" ).arg( dest ), QMessageBox::Ok, QMessageBox::Ok );
            return;
        }

        // Set right permission if file is inside the home path
        if ( dest.startsWith( item->user.homePath ) && chown( dest.toStdString().c_str(), pwd->pw_uid, grp->gr_gid ) < 0 )
        {
            QMessageBox::warning( this, tr( "Error!" ), tr( "Failed to set permission of file '%1'!" ).arg( dest ), QMessageBox::Ok, QMessageBox::Ok );
            return;
        }
    }


    // Create symlinks to ~/.face
    foreach ( QString dest, symlinkHomeDest )
    {
        if ( QFile::exists( dest ) )
            QFile::remove( dest );

        if ( !QFile::link( ".face", dest ) )
        {
            QMessageBox::warning( this, tr( "Error!" ), tr( "Failed to symlink '%1' to '%2'!" ).arg( ".face", dest ), QMessageBox::Ok, QMessageBox::Ok );
            return;
        }

        // Set right permission
        if ( chown( dest.toStdString().c_str(), pwd->pw_uid, grp->gr_gid ) < 0 )
        {
            QMessageBox::warning( this, tr( "Error!" ), tr( "Failed to set permission of file '%1'!" ).arg( dest ), QMessageBox::Ok, QMessageBox::Ok );
            return;
        }
    }
}


void
PageUsers::buttonAddUser_clicked()
{
    AddUserDialog dialog( this );
    dialog.exec();

    // Refresh list if required
    if ( dialog.userDataChanged() )
        load();
}


void
PageUsers::buttonRemoveUser_clicked()
{
    ListWidgetItem* item = dynamic_cast<ListWidgetItem*>( ui->listWidget->currentItem() );
    if ( !item )
        return;

    if ( QMessageBox::No == QMessageBox::question( this, tr( "Continue?" ), tr( "Do you really want to remove the user %1?" ).arg( item->text() ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) )
        return;

    QStringList args;

    if ( QMessageBox::Yes == QMessageBox::question( this, tr( "Remove Home?" ), tr( "Do you want to remove the home folder of the user %1?" ).arg( item->text() ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) )
        args << "-r";

    // Remove user
    QString errorMessage;

    if ( UsersCommon::runProcess( "userdel",
                                  args << item->text(),
                                  QStringList(),
                                  errorMessage ) != 0 )
        QMessageBox::warning( this, tr( "Error!" ), QString( tr( "Failed to remove user %1!" ).arg( item->text() ) + "\n" + errorMessage ), QMessageBox::Ok, QMessageBox::Ok );


    // Refresh list
    load();
}


void
PageUsers::buttonChangePassword_clicked()
{
    ListWidgetItem* item = dynamic_cast<ListWidgetItem*>( ui->listWidget->currentItem() );
    if ( !item )
        return;

    ChangePasswordDialog dialog( this );
    dialog.exec( item->text() );
}


void
PageUsers::buttonChangeAccountType_clicked()
{
    ListWidgetItem* item = dynamic_cast<ListWidgetItem*>( ui->listWidget->currentItem() );
    if ( !item )
        return;

    AccountTypeDialog dialog( this );
    dialog.exec( item->text() );

    // Update account type if required
    if ( dialog.userGroupsChanged() )
        setupUserData( item );
}
