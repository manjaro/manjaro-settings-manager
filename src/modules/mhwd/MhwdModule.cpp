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

#include "MhwdCommon.h"
#include "MhwdModule.h"
#include "ui_PageMhwd.h"
#include "libmhwd/mhwd.h"

#include <KAboutData>
#include <KPluginFactory>

#include <QtCore/QTranslator>

#include <QtDebug>

K_PLUGIN_FACTORY( MsmMhwdFactory,
                  registerPlugin<MhwdModule>( "msm_mhwd" ); )

MhwdModule::MhwdModule( QWidget* parent, const QVariantList& args ) :
    KCModule( parent, args ),
    ui( new Ui::PageMhwd )
{
    Q_INIT_RESOURCE( translations );
    QTranslator *appTranslator = new QTranslator;
    appTranslator->load( ":/translations/msm_" + QLocale::system().name() );
    qApp->installTranslator( appTranslator );

    KAboutData* aboutData = new KAboutData( "msm_mhwd",
                                            tr( "Hardware Configuration", "@title" ),
                                            PROJECT_VERSION,
                                            QStringLiteral( "" ),
                                            KAboutLicense::LicenseKey::GPL_V3,
                                            "Copyright 2014-15 Ramon Buldó" );
    aboutData->addAuthor( "Ramon Buldó",
                          tr( "Author", "@info:credit" ),
                          QStringLiteral( "ramon@manjaro.org" ) );
    setAboutData( aboutData );
    setButtons( KCModule::NoAdditionalButton );

    ui->setupUi( this );

    ui->treeWidget->setContextMenuPolicy( Qt::CustomContextMenu );
    ui->treeWidget->setColumnWidth( 0, 450 );
    ui->treeWidget->setColumnWidth( 1, 100 );
    ui->treeWidget->setColumnWidth( 2, 100 );

    // Context menu actions and icons
    ui->installAction->setIcon( QIcon::fromTheme( "list-add", QIcon( ":/icons/add.png" ) ) );
    ui->removeAction->setIcon( QIcon::fromTheme( "list-remove", QIcon( ":/icons/remove.png" ) ) );
    ui->reinstallAction->setIcon( QIcon::fromTheme( "view-refresh",  QIcon( ":/icons/restore.png" ) ) );

    // Connect signals and slots
    connect( ui->buttonInstallFree, &QPushButton::clicked,
             [=] ( bool checked )
    {
        Q_UNUSED( checked )
        QString configuration = ui->treeWidget->currentItem()->text( 0 );
        MhwdCommon::installFreeConfiguration( true );
        load();
    } );
    connect( ui->buttonInstallNonFree, &QPushButton::clicked,
             [=] ( bool checked )
    {
        Q_UNUSED( checked )
        QString configuration = ui->treeWidget->currentItem()->text( 0 );
        MhwdCommon::installNonFreeConfiguration( true );
        load();
    } );
    connect( ui->installAction, &QAction::triggered,
             [=] ( bool checked )
    {
        Q_UNUSED( checked )
        QString configuration = ui->treeWidget->currentItem()->text( 0 );
        MhwdCommon::installConfiguration( configuration, true );
        load();
    } );
    connect( ui->reinstallAction, &QAction::triggered,
             [=] ( bool checked )
    {
        Q_UNUSED( checked )
        QString configuration = ui->treeWidget->currentItem()->text( 0 );
        MhwdCommon::reinstallConfiguration( configuration, true );
        load();
    } );
    connect( ui->removeAction, &QAction::triggered,
             [=] ( bool checked )
    {
        Q_UNUSED( checked )
        QString configuration = ui->treeWidget->currentItem()->text( 0 );
        MhwdCommon::removeConfiguration( configuration, true );
        load();
    } );
    connect( ui->treeWidget, &QTreeWidget::customContextMenuRequested,
             [=] ( const QPoint &pos )
    {
        MhwdCommon::showItemContextMenu( ui, pos );
    } );
    connect( ui->checkBoxShowAll, &QCheckBox::toggled,
             this, &MhwdModule::load );
}


MhwdModule::~MhwdModule()
{
    delete ui;
}


void
MhwdModule::load()
{
    MhwdCommon::load( ui );
}


void
MhwdModule::save()
{
}


void
MhwdModule::defaults()
{
}

#include "MhwdModule.moc"
