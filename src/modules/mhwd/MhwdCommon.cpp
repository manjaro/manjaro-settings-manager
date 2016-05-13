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

#include "ActionDialog.h"
#include "ApplyDialog.h"
#include "MhwdCommon.h"

#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QDebug>


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
