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
#include "KernelCommon.h"
#include "KernelInfoDialog.h"

#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QDebug>


void
KernelCommon::load( KernelModel* kernelModel )
{
    kernelModel->update();
}


void
KernelCommon::kernelAction( const QModelIndex& index )
{
    bool isInstalled = qvariant_cast<bool>( index.data( KernelModel::IsInstalledRole ) );
    if ( isInstalled )
        removeKernel( index );
    else
        installKernel( index );
}


void
KernelCommon::installKernel( const QModelIndex& index )
{
    QStringList modules = qvariant_cast<QStringList>( index.data( KernelModel::InstalledModulesRole ) );
    QString kernel = qvariant_cast<QString>( index.data( KernelModel::PackageRole ) );
    QString version = qvariant_cast<QString>( index.data( KernelModel::VersionRole ) );
    QStringList packageList;
    packageList << kernel << modules;

    QString title = QString( tr( "Install Linux %1" ) ).arg( version );
    QString message = QString( tr( "The following packages will be installed:" ) );
    QString information = QString();
    foreach ( const QString p, packageList )
    {
        information.append( p );
        information.append( "\n" );
    }

    QStringList arguments;
    arguments << "--noconfirm" << "--noprogressbar" << "-S" << packageList;
    QVariantMap args;
    args["arguments"] = arguments;
    KAuth::Action installAction( QLatin1String( "org.manjaro.msm.kernel.install" ) );
    installAction.setHelperId( QLatin1String( "org.manjaro.msm.kernel" ) );
    installAction.setArguments( args );
    installAction.setTimeout( std::numeric_limits<int>::max() );

    ActionDialog actionDialog;
    actionDialog.setInstallAction( installAction );
    actionDialog.setWindowTitle( title );
    actionDialog.setMessage( message );
    actionDialog.setInformation( information );
    actionDialog.exec();
}


void
KernelCommon::removeKernel( const QModelIndex& index )
{
    QStringList modules = qvariant_cast<QStringList>( index.data( KernelModel::InstalledModulesRole ) );
    QString kernel = qvariant_cast<QString>( index.data( KernelModel::PackageRole ) );
    QString version = qvariant_cast<QString>( index.data( KernelModel::VersionRole ) );
    QStringList packageList;
    packageList << kernel << modules;

    QString title = QString( tr( "Remove Linux %1" ) ).arg( version );
    QString message = QString( tr( "The following packages will be removed:" ) );
    QString information = QString();
    foreach ( const QString p, packageList )
    {
        information.append( p );
        information.append( "\n" );
    }

    QStringList arguments;
    arguments << "--noconfirm" << "--noprogressbar" << "-R" << packageList;
    QVariantMap args;
    args["arguments"] = arguments;
    KAuth::Action installAction( QLatin1String( "org.manjaro.msm.kernel.remove" ) );
    installAction.setHelperId( QLatin1String( "org.manjaro.msm.kernel" ) );
    installAction.setArguments( args );
    installAction.setTimeout( std::numeric_limits<int>::max() );

    ActionDialog actionDialog;
    actionDialog.setInstallAction( installAction );
    actionDialog.setWindowTitle( title );
    actionDialog.setMessage( message );
    actionDialog.setInformation( information );
    actionDialog.exec();
}


void
KernelCommon::showChangelog( const QModelIndex& index )
{
    QString package = qvariant_cast<QString>( index.data( KernelModel::PackageRole ) );
    QString majorVersion = qvariant_cast<QString>( index.data( KernelModel::MajorVersionRole ) );
    QString minorVersion = qvariant_cast<QString>( index.data( KernelModel::MinorVersionRole ) );
    QString title = QString( tr( "Linux %1.%2 changelog" ) ).arg( majorVersion, minorVersion );
    KernelInfoDialog kernelInfoDialog;
    kernelInfoDialog.setWindowTitle( title );
    kernelInfoDialog.setPackage( package );
    kernelInfoDialog.exec();
}
