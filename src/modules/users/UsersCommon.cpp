/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Roland Singer <roland@manjaro.org>
 *  Ramon Buldó <ramon@manjaro.org>
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


#include "UsersCommon.h"

#include "AccountTypeDialog.h"
#include "AddUserDialog.h"
#include "ChangePasswordDialog.h"
#include "ListWidget.h"
#include "PreviewFileDialog.h"

#include <KAuth>

#include <QtWidgets/QMessageBox>

#include <QDebug>


QString
UsersCommon::getName()
{
    return QString( "msm_users" );
}


QString
UsersCommon::getTitle()
{
    return QString( tr ( "User Accounts" ) );
}


QString
UsersCommon::getDescription()
{
    return QString( tr ( "User accounts configuration" ) );
}


bool
UsersCommon::addUser()
{
    AddUserDialog dialog( nullptr );
    dialog.exec();

    // Refresh list if required
    if ( dialog.userDataChanged() )
        return true;
    else
        return false;
}


void
UsersCommon::changePassword( QListWidgetItem* currentItem )
{
    ListWidgetItem* item = dynamic_cast<ListWidgetItem*>( currentItem );
    if ( !item )
        return;

    ChangePasswordDialog dialog( nullptr );
    dialog.exec( item->text() );
}


bool
UsersCommon::changeAccountType( Ui::PageUsers* ui )
{
    ListWidgetItem* item = dynamic_cast<ListWidgetItem*>( ui->listWidget->currentItem() );
    if ( !item )
        return false;

    AccountTypeDialog dialog( nullptr );
    dialog.exec( item->text(), UsersCommon::Groups() );

    // Update account type if required
    if ( dialog.userGroupsChanged() )
        return true;
    else
        return false;
}


void
UsersCommon::loadUsers( ListWidget* listWidget )
{
    listWidget->clear();

    foreach ( const auto user, UsersCommon::Users() )
    {
        ListWidgetItem* item = new ListWidgetItem( listWidget );
        item->setText( user.username );
        item->user = user;

        QString faceIcon { user.homePath + "/.face" };
        QString accSerIcon { "/var/lib/AccountsService/icons/" + user.username } ;

        if ( QFile::exists( faceIcon ) )
            item->setIcon( QIcon( faceIcon ) );
        else if ( QFile::exists( accSerIcon ) )
            item->setIcon( QIcon( accSerIcon ) );
        else
            item->setIcon( QIcon( ":/icons/user.png" ) );
    }
}


void
UsersCommon::removeUser( QListWidgetItem* currentItem )
{
    ListWidgetItem* item = dynamic_cast<ListWidgetItem*>( currentItem );
    if ( !item )
        return;

    QString username = item->text();
    if ( QMessageBox::No == QMessageBox::question( nullptr,
            tr( "Continue?" ),
            tr( "Do you really want to remove the user %1?" ).arg( username ),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No ) )
        return;

    QString removeHome = "";
    if ( QMessageBox::Yes == QMessageBox::question( nullptr,
            tr( "Remove Home?" ),
            tr( "Do you want to remove the home folder of the user %1?" ).arg( username ),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No ) )
        removeHome = "-r";

    // Remove user
    KAuth::Action installAction( QLatin1String( "org.manjaro.msm.users.remove" ) );
    installAction.setHelperId( QLatin1String( "org.manjaro.msm.users" ) );
    QVariantMap args;
    args["arguments"] = QStringList() << removeHome << username;
    installAction.setArguments( args );
    KAuth::ExecuteJob* jobAdd = installAction.execute();
    connect( jobAdd, &KAuth::ExecuteJob::newData,
             [=] ( const QVariantMap &data )
    {
        qDebug() << data;
    } );
    if ( jobAdd->exec() )
        qDebug() << "Remove user job succesfull";
    else
    {
        QMessageBox::warning( nullptr,
                              tr( "Error!" ),
                              QString( tr( "Failed to remove user %1" ).arg( username ) ),
                              QMessageBox::Ok, QMessageBox::Ok );
        return;
    }
}


void
UsersCommon::setUserImage( Ui::PageUsers* ui )
{
    ListWidgetItem* item = dynamic_cast<ListWidgetItem*>( ui->listWidget->currentItem() );
    if ( !item )
        return;

    PreviewFileDialog dialog( nullptr );
    dialog.setFileMode( QFileDialog::ExistingFile );
    dialog.setNameFilter( tr( "Images (*.png *.jpg *.bmp)" ) );
    dialog.setViewMode( QFileDialog::Detail );
    dialog.setDirectory( QStringLiteral( "/usr/share/pixmaps/faces" ) );

    if ( !dialog.exec() || dialog.selectedFiles().isEmpty() )
        return;

    QString filename = dialog.selectedFiles().first();

    // Copy .face file to home dir
    QString faceDest;
    faceDest = QString( "%1/.face" ).arg( item->user.homePath );
    if ( QFile::exists( faceDest ) )
        QFile::remove( faceDest );
    QFile::copy( filename, faceDest );

    // Create symlinks to ~/.face
    QStringList symlinkHomeDest;
    symlinkHomeDest << QString( "%1/.face.icon" ).arg( item->user.homePath )
                    << QString( "%1/.icon" ).arg( item->user.homePath );
    foreach ( const QString dest, symlinkHomeDest )
    {
        if ( QFile::exists( dest ) )
            QFile::remove( dest );
        QFile::link( faceDest, dest );
    }

    // Copy face image to dirs that need admin rights
    QStringList copyDest;
    if ( QDir( "/var/lib/AccountsService/icons/" ).exists() )
        copyDest << QString( "/var/lib/AccountsService/icons/%1" ).arg( item->user.username );
    if ( QDir( "/usr/share/sddm/faces/" ).exists() )
        copyDest  << QString( "/usr/share/sddm/faces/%1.face.icon" ).arg( item->user.username );

    if ( !copyDest.isEmpty() )
    {
        KAuth::Action installAction( QLatin1String( "org.manjaro.msm.users.changeimage" ) );
        installAction.setHelperId( QLatin1String( "org.manjaro.msm.users" ) );
        QVariantMap args;
        args["copyDest"] = copyDest;
        args["filename"] = filename;
        installAction.setArguments( args );
        KAuth::ExecuteJob* jobAdd = installAction.execute();
        connect( jobAdd, &KAuth::ExecuteJob::newData,
                 [=] ( const QVariantMap &data )
        {
            qDebug() << data;
        } );
        if ( jobAdd->exec() )
            qDebug() << "Change image job succesfull";
        else
        {
            QMessageBox::warning( nullptr,
                                  tr( "Error!" ),
                                  QString( tr( "Failed to change user image" ) ),
                                  QMessageBox::Ok, QMessageBox::Ok );
            return;
        }
    }
}


void
UsersCommon::setupUserData( Ui::PageUsers* ui, QListWidgetItem* current )
{
    ui->buttonAccountType->setText( tr( "Standard" ) );

    if ( !current )
    {
        ui->buttonImage->setIcon( QIcon( ":/icons/user.png" ) );
        ui->labelUsername->setText( "" );
        ui->userWidget->setEnabled( false );
        return;
    }

    ui->buttonImage->setIcon( current->icon() );
    ui->labelUsername->setText( current->text() );
    ui->userWidget->setEnabled( true );

    foreach ( const auto group, UsersCommon::Groups() )
    {
        if ( group.name != QStringLiteral( "wheel" ) || !group.members.contains( current->text() ) )
            continue;

        ui->buttonAccountType->setText( tr( "Administrator" ) );
        break;
    }
}


QList<UsersCommon::User>
UsersCommon::Users()
{
    QList<UsersCommon::User> users;

    const QString passwdFilePath { "/etc/passwd" };
    QFile file( passwdFilePath );
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        return users;

    QTextStream in( &file );

    while ( !in.atEnd() )
    {
        QStringList split = in.readLine()
                            .split( "#", QString::KeepEmptyParts )
                            .first()
                            .split( ":", QString::KeepEmptyParts );

        if ( split.size() < 7 )
            continue;

        User user;
        user.username = split.at( 0 );
        user.homePath = split.at( 5 );
        user.uuid = split.at( 2 ).toInt();

        const int minUserUuid { 1000 };
        if ( user.uuid < minUserUuid || user.username.isEmpty() || user.homePath.isEmpty() )
            continue;

        users.append( user );
    }

    file.close();

    return users;
}


QList<UsersCommon::Group>
UsersCommon::Groups()
{
    QList<UsersCommon::Group> groups;

    const QString groupconfFilePath { "/etc/group" };
    QFile file( groupconfFilePath  );
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        return groups;

    QTextStream in( &file );

    while ( !in.atEnd() )
    {
        QStringList split = in.readLine()
                            .split( "#", QString::KeepEmptyParts )
                            .first()
                            .split( ":", QString::KeepEmptyParts );

        if ( split.size() < 4 )
            continue;

        Group group;
        group.name = split.at( 0 );
        group.members = split.at( 3 ).split( ",", QString::SkipEmptyParts );

        if ( group.name.isEmpty() )
            continue;

        groups.append( group );
    }

    file.close();

    return groups;
}
