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
#include "MhwdCommon.h"
#include "MhwdModule.h"
#include "ui_PageMhwd.h"
#include "libmhwd/mhwd.h"

#include <KAboutData>
#include <KPluginFactory>

#include <QtCore/QTranslator>

#include <QtDebug>

K_PLUGIN_FACTORY( MsmMhwdFactory,
                  registerPlugin<MhwdModule>( MhwdCommon::getName() ); )

MhwdModule::MhwdModule( QWidget* parent, const QVariantList& args ) :
    KCModule( parent, args ),
    ui( new Ui::PageMhwd )
{
    Q_INIT_RESOURCE( translations );
    QTranslator* appTranslator = new QTranslator;
    appTranslator->load( ":/translations/msm_" + QLocale::system().name() );
    qApp->installTranslator( appTranslator );

    KAboutData* aboutData = new KAboutData( MhwdCommon::getName(),
                                            MhwdCommon::getTitle(),
                                            PROJECT_VERSION,
                                            MhwdCommon::getDescription(),
                                            KAboutLicense::LicenseKey::GPL_V3,
                                            "(c) 2014 - 2017 Manjaro Settings Manager developers" );
    aboutData->addAuthor( "Ramon Buldó",
                          QStringLiteral( "ramon@manjaro.org" ) );
    aboutData->setCustomAuthorText( QString(), MsmCommon::getBugReportLink() );
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
    connect( ui->buttonInstallFree, &QLabel::linkActivated,
             [=] ( const QString& link )
    {
        Q_UNUSED( link )
        MhwdCommon::installFreeConfiguration( );
        load();
    } );
    connect( ui->buttonInstallNonFree, &QLabel::linkActivated,
             [=] ( const QString& link )
    {
        Q_UNUSED( link )
        MhwdCommon::installNonFreeConfiguration( );
        load();
    } );
    connect( ui->installAction, &QAction::triggered,
             [=] ( bool checked )
    {
        Q_UNUSED( checked )
        QString configuration = ui->treeWidget->currentItem()->text( 0 );
        MhwdCommon::installConfiguration( configuration );
        load();
    } );
    connect( ui->reinstallAction, &QAction::triggered,
             [=] ( bool checked )
    {
        Q_UNUSED( checked )
        QString configuration = ui->treeWidget->currentItem()->text( 0 );
        MhwdCommon::reinstallConfiguration( configuration );
        load();
    } );
    connect( ui->removeAction, &QAction::triggered,
             [=] ( bool checked )
    {
        Q_UNUSED( checked )
        QString configuration = ui->treeWidget->currentItem()->text( 0 );
        MhwdCommon::removeConfiguration( configuration );
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
