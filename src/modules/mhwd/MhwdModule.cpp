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
#include "ActionDialog.h"

#include <KAboutData>
#include <KAuth>
#include <KAuthAction>
#include <KPluginFactory>

#include <QtCore/QTranslator>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>

#include <QtDebug>

K_PLUGIN_FACTORY( MsmMhwdFactory,
                  registerPlugin<MhwdModule>( "msm_mhwd" ); )

MhwdModule::MhwdModule( QWidget* parent, const QVariantList& args ) :
    KCModule( parent, args ),
    ui( new Ui::PageMhwd )
{
    Q_INIT_RESOURCE( translations );
    QTranslator appTranslator;
    appTranslator.load( ":/translations/msm_" + QLocale::system().name() );
    qApp->installTranslator( &appTranslator );

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
    } );
    connect( ui->buttonInstallNonFree, &QPushButton::clicked,
             [=] ( bool checked )
    {
        Q_UNUSED( checked )
        QString configuration = ui->treeWidget->currentItem()->text( 0 );
        MhwdCommon::installNonFreeConfiguration( true );
    } );
    connect( ui->installAction, &QAction::triggered,
             [=] ( bool checked )
    {
        Q_UNUSED( checked )
        QString configuration = ui->treeWidget->currentItem()->text( 0 );
        MhwdCommon::installConfiguration( configuration, true );
    } );
    connect( ui->reinstallAction, &QAction::triggered,
             [=] ( bool checked )
    {
        Q_UNUSED( checked )
        QString configuration = ui->treeWidget->currentItem()->text( 0 );
        MhwdCommon::reinstallConfiguration( configuration, true );
    } );
    connect( ui->removeAction, &QAction::triggered,
             [=] ( bool checked )
    {
        Q_UNUSED( checked )
        QString configuration = ui->treeWidget->currentItem()->text( 0 );
        MhwdCommon::removeConfiguration( configuration, true );
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
    ui->treeWidget->clear();
    ui->buttonInstallFree->setEnabled( false );
    ui->buttonInstallNonFree->setEnabled( false );
    // Create mhwd data object and fill it with hardware informations
    mhwd::Data data;
    mhwd::initData( &data );
    mhwd::fillData( &data );

    for ( std::vector<mhwd::Device*>::iterator dev_iter = data.PCIDevices.begin();
            dev_iter != data.PCIDevices.end();
            ++dev_iter )
    {
        QTreeWidgetItem* deviceItem = new QTreeWidgetItem();
        // Check if deviceClass node its already added
        QString deviceClassName = QString::fromStdString( ( *dev_iter )->className );
        QList<QTreeWidgetItem*> found = ui->treeWidget->findItems( deviceClassName, Qt::MatchCaseSensitive, 0 );
        if ( found.isEmpty() )
        {
            QTreeWidgetItem* deviceClassItem = new QTreeWidgetItem( ui->treeWidget );
            deviceClassItem->setText( 0, deviceClassName );
            deviceClassItem->addChild( deviceItem );
            if ( !ui->checkBoxShowAll->isChecked() )
                deviceClassItem->setHidden( true );
        }
        else
            found.first()->addChild( deviceItem );

        QString deviceName = QString::fromStdString( ( *dev_iter )->deviceName );
        QString vendorName = QString::fromStdString( ( *dev_iter )->vendorName );
        if ( deviceName.isEmpty() )
            deviceName = tr( "Unknown device name" );
        deviceItem->setText( 0, QString( "%1 (%2)" ).arg( deviceName, vendorName ) );


        for ( std::vector<mhwd::Config*>::iterator conf_iter = ( *dev_iter )->availableConfigs.begin();
                conf_iter != ( *dev_iter )->availableConfigs.end(); ++conf_iter )
        {
            //Always expand and show devices with configuration
            deviceItem->parent()->setHidden( false );
            deviceItem->parent()->setExpanded( true );
            deviceItem->setExpanded( true );

            QTreeWidgetItem* item = new QTreeWidgetItem( deviceItem );
            item->setFlags( Qt::ItemIsEnabled );

            QString configName = QString::fromStdString( ( *conf_iter )->name );
            item->setText( 0, configName );
            if ( ( configName.toLower().contains( "nvidia" ) || configName.toLower().contains( "nouveau" ) ) &&
                    configName.toLower().contains( "intel" ) )
                item->setIcon( 0, QIcon( ":/icons/intel-nvidia.png" ) );
            else if ( configName.toLower().contains( "intel" ) )
                item->setIcon( 0, QIcon( ":/icons/intel.png" ) );
            else if ( configName.toLower().contains( "nvidia" ) || configName.toLower().contains( "nouveau" ) )
                item->setIcon( 0, QIcon( ":/icons/nvidia.png" ) );
            else if ( configName.toLower().contains( "catalyst" ) )
                item->setIcon( 0, QIcon( ":/icons/ati.png" ) );
            else
                item->setIcon( 0, QIcon( ":/icons/gpudriver.png" ) );

            //Check if freedriver
            if ( ( *conf_iter )->freedriver )
            {
                item->setCheckState( 1, Qt::Checked );
                ui->buttonInstallFree->setEnabled( true );
            }
            else
            {
                item->setCheckState( 1, Qt::Unchecked );
                ui->buttonInstallNonFree->setEnabled( true );
            }

            //Check if installed
            mhwd::Config* installedConfig = getInstalledConfig( &data, ( *conf_iter )->name, ( *conf_iter )->type );
            if ( installedConfig == NULL )
                item->setCheckState( 2, Qt::Unchecked );
            else
                item->setCheckState( 2, Qt::Checked );
        }
    }
    // Free data object again
    mhwd::freeData( &data );
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
