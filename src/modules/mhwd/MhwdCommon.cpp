/*
 *  This file is part of Manjaro Settings Manager.
 *
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
#include <limits>
#include "ActionDialog.h"
#include "ApplyDialog.h"
#include "MhwdCommon.h"
#include "libmhwd/mhwd.h"

#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QDebug>


void
MhwdCommon::load( Ui::PageMhwd* ui )
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
            dev_iter++ )
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
                conf_iter != ( *dev_iter )->availableConfigs.end(); conf_iter++ )
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


bool
MhwdCommon::installConfiguration( QString configuration, bool useKAuth )
{
    QString title = QString( tr( "Install configuration" ) );
    QString message = QString( tr( "MHWD will install the '%1' configuration" ) )
                      .arg( configuration );

    if ( useKAuth )
    {
        QVariantMap args;
        args["arguments"] = QStringList() << "-i" << "pci" << configuration;
        KAuth::Action installAction( QLatin1String( "org.manjaro.msm.mhwd.install" ) );
        installAction.setHelperId( QLatin1String( "org.manjaro.msm.mhwd" ) );
        installAction.setArguments( args );
        installAction.setTimeout( std::numeric_limits<int>::max() );

        ActionDialog actionDialog;
        actionDialog.setInstallAction( installAction );
        actionDialog.setWindowTitle( title );
        actionDialog.setMessage( message );
        actionDialog.exec();
        return actionDialog.isJobSuccesful();
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question( nullptr,
                                       title,
                                       message,
                                       QMessageBox::Yes | QMessageBox::No );
        if ( reply == QMessageBox::Yes )
        {
            ApplyDialog dialog( nullptr );
            dialog.exec( "mhwd", QStringList() << "-i" << "pci" << configuration,
                         tr( "Installing driver..." ),
                         false );
            return dialog.processSuccess();
        }
        else
            return false;
    }
}


bool
MhwdCommon::installFreeConfiguration( bool useKAuth )
{
    QString title = QString( tr( "Install open-source graphic driver" ) );
    QString message = QString( tr( "MHWD will autodetect your open-source graphic drivers and install it" ) );

    if ( useKAuth )
    {
        QVariantMap args;
        args["arguments"] = QStringList() << "-a" << "pci" << "free" << "0300";
        KAuth::Action installAction( QLatin1String( "org.manjaro.msm.mhwd.install" ) );
        installAction.setHelperId( QLatin1String( "org.manjaro.msm.mhwd" ) );
        installAction.setArguments( args );
        installAction.setTimeout( std::numeric_limits<int>::max() );

        ActionDialog actionDialog;
        actionDialog.setInstallAction( installAction );
        actionDialog.setWindowTitle( title );
        actionDialog.setMessage( message );
        actionDialog.exec();
        return actionDialog.isJobSuccesful();
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question( nullptr,
                                       title,
                                       message,
                                       QMessageBox::Yes | QMessageBox::No );
        if ( reply == QMessageBox::Yes )
        {
            ApplyDialog dialog( nullptr );
            dialog.exec( "mhwd",
                         QStringList() << "-a" << "pci" << "free" << "0300",
                         tr( "Installing driver..." ),
                         false );
            return dialog.processSuccess();
        }
        else
            return false;
    }
}


bool
MhwdCommon::installNonFreeConfiguration( bool useKAuth )
{
    QString title = QString( tr( "Install proprietary graphic driver" ) );
    QString message = QString( tr( "MHWD will autodetect your proprietary graphic drivers and install it" ) );

    if ( useKAuth )
    {
        QVariantMap args;
        args["arguments"] = QStringList() << "-a" << "pci" << "nonfree" << "0300";
        KAuth::Action installAction( QLatin1String( "org.manjaro.msm.mhwd.install" ) );
        installAction.setHelperId( QLatin1String( "org.manjaro.msm.mhwd" ) );
        installAction.setArguments( args );
        installAction.setTimeout( std::numeric_limits<int>::max() );

        ActionDialog actionDialog;
        actionDialog.setInstallAction( installAction );
        actionDialog.setWindowTitle( title );
        actionDialog.setMessage( message );
        actionDialog.exec();
        return actionDialog.isJobSuccesful();
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question( nullptr,
                                       title,
                                       message,
                                       QMessageBox::Yes | QMessageBox::No );
        if ( reply == QMessageBox::Yes )
        {
            ApplyDialog dialog( nullptr );
            dialog.exec( "mhwd",
                         QStringList() << "-a" << "pci" << "nonfree" << "0300",
                         tr( "Installing driver..." ),
                         false );
            return dialog.processSuccess();
        }
        else
            return false;
    }
}


bool
MhwdCommon::reinstallConfiguration( QString configuration, bool useKAuth )
{
    QString title = QString( tr( "Reinstall configuration" ) );
    QString message = QString( tr( "MHWD will reinstall the '%1' configuration" ) )
                      .arg( configuration );

    if ( useKAuth )
    {
        QVariantMap args;
        args["arguments"] = QStringList() << "-f" << "-i" << "pci" << configuration;
        KAuth::Action installAction( QLatin1String( "org.manjaro.msm.mhwd.install" ) );
        installAction.setHelperId( QLatin1String( "org.manjaro.msm.mhwd" ) );
        installAction.setArguments( args );
        installAction.setTimeout( std::numeric_limits<int>::max() );

        ActionDialog actionDialog;
        actionDialog.setInstallAction( installAction );
        actionDialog.setWindowTitle( title );
        actionDialog.setMessage( message );
        actionDialog.exec();
        return actionDialog.isJobSuccesful();
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question( nullptr,
                                       title,
                                       message,
                                       QMessageBox::Yes | QMessageBox::No );
        if ( reply == QMessageBox::Yes )
        {
            ApplyDialog dialog( nullptr );
            dialog.exec( "mhwd",
                         QStringList() << "-f" << "-i" << "pci" << configuration,
                         tr( "Reinstalling driver..." ),
                         false );
            return dialog.processSuccess();
        }
        else
            return false;
    }
}


bool
MhwdCommon::removeConfiguration( QString configuration, bool useKAuth )
{
    QString title = QString( tr( "Remove configuration" ) );
    QString message = QString( tr( "MHWD will remove the '%1' configuration" ) )
                      .arg( configuration );

    if ( useKAuth )
    {
        QVariantMap args;
        args["arguments"] = QStringList() << "-r" << "pci" << configuration;
        KAuth::Action installAction( QLatin1String( "org.manjaro.msm.mhwd.remove" ) );
        installAction.setHelperId( QLatin1String( "org.manjaro.msm.mhwd" ) );
        installAction.setArguments( args );
        installAction.setTimeout( std::numeric_limits<int>::max() );

        ActionDialog actionDialog;
        actionDialog.setInstallAction( installAction );
        actionDialog.setWindowTitle( title );
        actionDialog.setMessage( message );
        actionDialog.exec();
        return actionDialog.isJobSuccesful();
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question( nullptr,
                                       title,
                                       message,
                                       QMessageBox::Yes | QMessageBox::No );
        if ( reply == QMessageBox::Yes )
        {
            ApplyDialog dialog( nullptr );
            dialog.exec( "mhwd",
                         QStringList() << "-r" << "pci" << configuration,
                         tr( "Removing driver..." ),
                         false );
            return dialog.processSuccess();
        }
        else
            return false;
    }
}


void
MhwdCommon::showItemContextMenu( Ui::PageMhwd* ui, const QPoint& pos )
{
    QMenu contextMenu( nullptr );
    QTreeWidgetItem* temp = ui->treeWidget->itemAt( pos );
    if ( ( temp != NULL ) && ( temp->text( 0 ).contains( "video-" ) || temp->text( 0 ).contains( "network-" ) ) )
    {
        if ( temp->checkState( 2 ) )
        {
            contextMenu.addAction( ui->removeAction );
            contextMenu.addAction( ui->reinstallAction );
        }
        else
            contextMenu.addAction( ui->installAction );
        contextMenu.exec( ui->treeWidget->viewport()->mapToGlobal( pos ) );
    }
}
