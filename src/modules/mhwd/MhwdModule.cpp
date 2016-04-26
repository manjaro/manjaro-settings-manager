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
                  registerPlugin<PageMhwd>( "msm_mhwd" ); )

PageMhwd::PageMhwd( QWidget* parent, const QVariantList& args ) :
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
    m_installAction = new QAction( tr( "Install" ), ui->treeWidget );
    m_installAction->setIcon(QIcon::fromTheme("list-add", QPixmap( ":/icons/add.png") ));
    m_removeAction = new QAction( tr( "Remove" ), ui->treeWidget );
    m_removeAction->setIcon(QIcon::fromTheme("list-remove", QPixmap( ":/icons/remove.png") ));
    m_forceReinstallationAction = new QAction( tr( "Force Reinstallation" ), ui->treeWidget );
    m_forceReinstallationAction->setIcon( QIcon::fromTheme("view-refresh",  QPixmap( ":/icons/restore.png")) );

    // Connect signals and slots
    connect( ui->buttonInstallFree, &QPushButton::clicked,
             this, &PageMhwd::installFreeConfiguration );
    connect( ui->buttonInstallNonFree, &QPushButton::clicked,
             this, &PageMhwd::installNonFreeConfiguration );
    connect( ui->treeWidget, &QTreeWidget::customContextMenuRequested,
             this, &PageMhwd::showContextMenuForTreeWidget );
    connect( m_installAction, &QAction::triggered,
             this, &PageMhwd::installConfiguration );
    connect( m_removeAction, &QAction::triggered,
             this, &PageMhwd::removeConfiguration );
    connect( m_forceReinstallationAction, &QAction::triggered,
             this, &PageMhwd::reinstallConfiguration );
    connect( ui->checkBoxShowAll, &QCheckBox::toggled,
             this, &PageMhwd::load );
}


PageMhwd::~PageMhwd()
{
    delete ui;
}


void
PageMhwd::load()
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
PageMhwd::save()
{
}


void
PageMhwd::defaults()
{
}


void
PageMhwd::installFreeConfiguration()
{
    QString title = QString( tr( "Install open-source graphic driver" ) );
    QString message = QString( tr( "MHWD will autodetect your open-source graphic drivers and install it" ) );

    QVariantMap args;
    args["arguments"] = QStringList() << "-a" << "pci" << "free" << "0300";
    KAuth::Action installAction( QLatin1String( "org.manjaro.msm.mhwd.install" ) );
    installAction.setHelperId( QLatin1String( "org.manjaro.msm.mhwd" ) );
    installAction.setArguments( args );

    ActionDialog actionDialog;
    actionDialog.setInstallAction( installAction );
    actionDialog.setWindowTitle( title );
    actionDialog.setMessage( message );
    actionDialog.exec();
    if ( actionDialog.isJobSuccesful() )
        load();
}


void
PageMhwd::installNonFreeConfiguration()
{
    QString title = QString( tr( "Install proprietary graphic driver" ) );
    QString message = QString( tr( "MHWD will autodetect your proprietary graphic drivers and install it" ) );

    QVariantMap args;
    args["arguments"] = QStringList() << "-a" << "pci" << "nonfree" << "0300";
    KAuth::Action installAction( QLatin1String( "org.manjaro.msm.mhwd.install" ) );
    installAction.setHelperId( QLatin1String( "org.manjaro.msm.mhwd" ) );
    installAction.setArguments( args );

    ActionDialog actionDialog;
    actionDialog.setInstallAction( installAction );
    actionDialog.setWindowTitle( title );
    actionDialog.setMessage( message );
    actionDialog.exec();
    if ( actionDialog.isJobSuccesful() )
        load();
}


void
PageMhwd::showContextMenuForTreeWidget( const QPoint& pos )
{
    QMenu contextMenu( this );
    QTreeWidgetItem* temp = ui->treeWidget->itemAt( pos );
    if ( ( temp != NULL ) && ( temp->text( 0 ).contains( "video-" ) || temp->text( 0 ).contains( "network-" ) ) )
    {
        if ( temp->checkState( 2 ) )
        {
            contextMenu.addAction( m_removeAction );
            contextMenu.addAction( m_forceReinstallationAction );
        }
        else
            contextMenu.addAction( m_installAction );
        contextMenu.exec( ui->treeWidget->viewport()->mapToGlobal( pos ) );
    }
}


void
PageMhwd::installConfiguration()
{
    QString configuration = ui->treeWidget->currentItem()->text( 0 );

    QString title = QString( tr( "Install configuration" ) );
    QString message = QString( tr( "MHWD will install the '%1' configuration" ) )
                      .arg( configuration );

    QVariantMap args;
    args["arguments"] = QStringList() << "-i" << "pci" << configuration;
    KAuth::Action installAction( QLatin1String( "org.manjaro.msm.mhwd.install" ) );
    installAction.setHelperId( QLatin1String( "org.manjaro.msm.mhwd" ) );
    installAction.setArguments( args );

    ActionDialog actionDialog;
    actionDialog.setInstallAction( installAction );
    actionDialog.setWindowTitle( title );
    actionDialog.setMessage( message );
    actionDialog.exec();
    if ( actionDialog.isJobSuccesful() )
        load();
}


void
PageMhwd::removeConfiguration()
{
    QString configuration = ui->treeWidget->currentItem()->text( 0 );

    QString title = QString( tr( "Remove configuration" ) );
    QString message = QString( tr( "MHWD will remove the '%1' configuration" ) )
                      .arg( configuration );

    QVariantMap args;
    args["arguments"] = QStringList() << "-r" << "pci" << configuration;
    KAuth::Action installAction( QLatin1String( "org.manjaro.msm.mhwd.remove" ) );
    installAction.setHelperId( QLatin1String( "org.manjaro.msm.mhwd" ) );
    installAction.setArguments( args );

    ActionDialog actionDialog;
    actionDialog.setInstallAction( installAction );
    actionDialog.setWindowTitle( title );
    actionDialog.setMessage( message );
    actionDialog.exec();
    if ( actionDialog.isJobSuccesful() )
        load();
}


void
PageMhwd::reinstallConfiguration()
{
    QString configuration = ui->treeWidget->currentItem()->text( 0 );

    QString title = QString( tr( "Reinstall configuration" ) );
    QString message = QString( tr( "MHWD will reinstall the '%1' configuration" ) )
                      .arg( configuration );

    QVariantMap args;
    args["arguments"] = QStringList() << "-f" << "-i" << "pci" << configuration;
    KAuth::Action installAction( QLatin1String( "org.manjaro.msm.mhwd.install" ) );
    installAction.setHelperId( QLatin1String( "org.manjaro.msm.mhwd" ) );
    installAction.setArguments( args );

    ActionDialog actionDialog;
    actionDialog.setInstallAction( installAction );
    actionDialog.setWindowTitle( title );
    actionDialog.setMessage( message );
    actionDialog.exec();
    if ( actionDialog.isJobSuccesful() )
        load();
}

#include "MhwdModule.moc"
