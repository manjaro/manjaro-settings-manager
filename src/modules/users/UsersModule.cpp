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

#include "MsmCommon.h"
#include "UsersModule.h"
#include "ui_PageUsers.h"

#include <KAboutData>
#include <KPluginFactory>

#include <QTranslator>

K_PLUGIN_FACTORY( MsmUsersFactory,
                  registerPlugin<UsersModule>( UsersCommon::getName() ); )

UsersModule::UsersModule( QWidget* parent, const QVariantList& args ) :
    KCModule( parent, args ),
    ui( new Ui::PageUsers )
{
    Q_INIT_RESOURCE( translations );
    QTranslator* appTranslator = new QTranslator;
    appTranslator->load( ":/translations/msm_" + QLocale::system().name() );
    qApp->installTranslator( appTranslator );

    KAboutData* aboutData = new KAboutData( UsersCommon::getName(),
                                            UsersCommon::getTitle(),
                                            PROJECT_VERSION,
                                            UsersCommon::getDescription(),
                                            KAboutLicense::LicenseKey::GPL_V3,
                                            "(c) 2014 - 2017 Manjaro Settings Manager developers" );
    aboutData->addAuthor( "Ramon Buldó",
                          QStringLiteral( "ramon@manjaro.org" ) );
    aboutData->addAuthor( "Roland Singer",
                          QStringLiteral( "roland@manjaro.org" ) );
    aboutData->setCustomAuthorText( QString(), MsmCommon::getBugReportLink() );
    setAboutData( aboutData );
    setButtons( KCModule::NoAdditionalButton );

    ui->setupUi( this );
    ui->buttonAdd->setIcon( QIcon::fromTheme( "list-add", QIcon( ":/icons/add.png" ) ) );
    ui->buttonRemove->setIcon( QIcon::fromTheme( "list-remove", QIcon( ":/icons/remove.png" ) ) );

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


UsersModule::~UsersModule()
{
    delete ui;
}


void
UsersModule::load()
{
    UsersCommon::loadUsers( ui->listWidget );
    UsersCommon::setupUserData( ui, ui->listWidget->currentItem() );
}


void
UsersModule::save()
{
    return;
}


void
UsersModule::defaults()
{
    this->load();
}

#include "UsersModule.moc"
