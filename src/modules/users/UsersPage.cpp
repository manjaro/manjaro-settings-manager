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

#include "UsersPage.h"
#include "ui_PageUsers.h"


UsersPage::UsersPage( QWidget* parent ) :
    PageWidget( parent ),
    ui( new Ui::PageUsers )
{
    ui->setupUi( this );
    ui->buttonAdd->setIcon( QIcon::fromTheme( "list-add", QIcon( ":/icons/add.png" ) ) );
    ui->buttonRemove->setIcon( QIcon::fromTheme( "list-remove", QIcon( ":/icons/remove.png" ) ) );

    setTitle( UsersCommon::getTitle() );
    setIcon( QPixmap( ":/images/resources/users.png" ) );
    setName( UsersCommon::getName() );

    // Connect signals and slots
    connect( ui->listWidget, &ListWidget::currentItemChanged,
             [=]( QListWidgetItem* current, QListWidgetItem *previous )
    {
        Q_UNUSED( previous );
        UsersCommon::setupUserData( ui, current );
    } );
    connect( ui->buttonImage, &FlatButton::clicked,
             [=]( bool checked )
    {
        Q_UNUSED( checked );
        UsersCommon::setUserImage( ui );
    } );
    connect( ui->buttonAdd, &FlatButton::clicked,
             [=]( bool checked )
    {
        Q_UNUSED( checked );
        if ( UsersCommon::addUser() )
            load();
    } );
    connect( ui->buttonRemove, &FlatButton::clicked,
             [=]( bool checked )
    {
        Q_UNUSED( checked );
        UsersCommon::removeUser( ui->listWidget->currentItem() );
        load();
    } );
    connect( ui->buttonPassword, &FlatButton::clicked,
             [=]( bool checked )
    {
        Q_UNUSED( checked );
        UsersCommon::changePassword( ui->listWidget->currentItem() );
        load();
    } );
    connect( ui->buttonAccountType, &FlatButton::clicked,
             [=]( bool checked )
    {
        Q_UNUSED( checked );
        if ( UsersCommon::changeAccountType( ui ) )
            UsersCommon::setupUserData( ui, ui->listWidget->currentItem() );
    } );
}


UsersPage::~UsersPage()
{
    delete ui;
}


void
UsersPage::load()
{
    UsersCommon::loadUsers( ui->listWidget );
    UsersCommon::setupUserData( ui, ui->listWidget->currentItem() );
    QApplication::restoreOverrideCursor();
}
