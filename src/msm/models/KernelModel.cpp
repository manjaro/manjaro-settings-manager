/*
 *  Manjaro Settings Manager
 *  Ramon Buldó <ramon@manjaro.org>
 *
 *  Copyright (C) 2007 Free Software Foundation, Inc.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "KernelModel.h"

#include "global/Global.h"

#include <QtCore/QRegularExpression>
#include <QtCore/QSet>

#include <QtCore/QElapsedTimer>
#include <QDebug>


KernelModel::KernelModel( QObject* parent )
    : QAbstractListModel( parent )
{
}

/*
 * Clear and update ther list of kernels
 */
void
KernelModel::update()
{
    QHash<QString, QString> installedKernelPackages = getInstalledPackages();
    QHash<QString, QString> availableKernelPackages = getAvailablePackages();

    QHash<QString, QString> allKernelPackages;
    allKernelPackages.unite( installedKernelPackages );
    QHashIterator<QString, QString> i( availableKernelPackages );
    while ( i.hasNext() )
    {
        i.next();
        allKernelPackages.insert( i.key(), i.value() );
    }

    QString runningKernel = Global::getRunningKernel();
    QStringList ltsKernels = Global::getLtsKernels();
    QStringList sesKernels = Global::getSesKernels();
    QStringList recommendedKernels = Global::getRecommendedKernels();

    QSet<QString> modulesToInstall;
    for ( const QString& module : QStringList( installedKernelPackages.keys() ).filter( QRegularExpression( "^linux[0-9][0-9]?([0-9])-" ) ) )
    {
        QString aux = QString( module ).remove( QRegularExpression( "^linux[0-9][0-9]?([0-9])-" ) );
        modulesToInstall.insert( aux );
    }

    beginResetModel();
    kernels_.clear();
    for ( const QString& kernel : QStringList( allKernelPackages.keys() ).filter( QRegularExpression( "^linux[0-9][0-9]?([0-9])$" ) ) )
    {
        Kernel newKernel;

        newKernel.setPackage( kernel );
        newKernel.setAvailable( availableKernelPackages.contains( kernel ) );
        newKernel.setInstalled( installedKernelPackages.contains( kernel ) );
        newKernel.setVersion( allKernelPackages.value( kernel ) );
        newKernel.setAvailableModules( QStringList( availableKernelPackages.keys() )
                                       .filter( QRegularExpression( QString( "^%1-" ).arg( kernel ) ) ) );
        if ( newKernel.isInstalled() )
        {
            newKernel.setInstalledModules( QStringList( installedKernelPackages.keys() )
                                           .filter( QRegularExpression( QString( "^%1-" ).arg( kernel ) ) ) );
        }
        else
        {
            QStringList installableModules;
            for ( const QString& module : modulesToInstall )
            {
                QString modulePackage = QString( "%1-%2" ).arg( kernel ).arg( module );
                if ( availableKernelPackages.contains( modulePackage ) )
                    installableModules << modulePackage;
            }
            newKernel.setInstalledModules( installableModules );
        }
        if ( ltsKernels.contains( kernel ) )
        {
            newKernel.setLts( true );
            newKernel.setLtsString( "LTS" );
        }
        else if ( sesKernels.contains( kernel ) )
        {
            newKernel.setLts( true );
            newKernel.setLtsString( "ESS" );
        }
        newKernel.setRecommended( recommendedKernels.contains( kernel ) );
        newKernel.setRunning( QString::compare( runningKernel, kernel ) == 0 );

        kernels_.append( newKernel );
    }
    endResetModel();
}


void
KernelModel::add( const Kernel& kernel )
{
    beginInsertRows( QModelIndex(), rowCount(), rowCount() );
    kernels_ << kernel;
    endInsertRows();
}


bool
KernelModel::remove( int position, int rows, const QModelIndex& parent )
{
    Q_UNUSED( parent )
    beginRemoveRows( QModelIndex(), position, position+rows-1 );
    for ( int row = 0; row < rows; ++row )
        kernels_.removeAt( position );
    endRemoveRows();
    return true;
}


int
KernelModel::rowCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent );
    return kernels_.size();
}


QVariant
KernelModel::data( const QModelIndex& index, int role ) const
{
    if ( index.row() < 0 || index.row() >= kernels_.count() )
        return QVariant();
    const Kernel& kernel = kernels_[index.row()];
    switch ( role )
    {
    case PackageRole:
        return kernel.package();
    case VersionRole:
        return kernel.version();
    case MajorVersionRole:
        return kernel.majorVersion();
    case MinorVersionRole:
        return kernel.minorVersion();
    case InstalledModulesRole:
        return kernel.installedModules();
    case AvailableModulesRole:
        return kernel.availableModules();
    case IsAvailableRole:
        return kernel.isAvailable();
    case IsInstalledRole:
        return kernel.isInstalled();
    case IsLtsRole:
        return kernel.isLts();
    case IsRecommendedRole:
        return kernel.isRecommended();
    case IsRunningRole:
        return kernel.isRunning();
    case IsUnsupportedRole:
        return kernel.isUnsupported();
    case LtsStringRole:
        return kernel.ltsString();
    }
    return QVariant();
}


QHash<int, QByteArray>
KernelModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PackageRole] = "package";
    roles[VersionRole] = "version";
    roles[MajorVersionRole] = "majorVersion";
    roles[MinorVersionRole] = "minorVersion";
    roles[InstalledModulesRole] = "installedModules";
    roles[AvailableModulesRole] = "availableModules";
    roles[IsAvailableRole] = "isAvailable";
    roles[IsInstalledRole] = "isInstalled";
    roles[IsLtsRole] = "isLts";
    roles[IsRecommendedRole] = "isRecommended";
    roles[IsRunningRole] = "isRunning";
    roles[LtsStringRole] = "ltsString";
    return roles;
}


QHash<QString, QString>
KernelModel::getAvailablePackages() const
{
    QProcess process;
    process.setEnvironment( QStringList() << "LANG=C" << "LC_MESSAGES=C" );
    process.start( "pacman", QStringList() << "-Ss" << "^linux[0-9][0-9]?([0-9])" );
    if ( !process.waitForFinished( 15000 ) )
        qDebug() << "error: failed to get all installed kernels";
    QString result = process.readAll();

    QHash<QString, QString> packages;
    for ( QString line : result.split( "\n", QString::SkipEmptyParts ) )
    {
        if ( line.isEmpty() )
            continue;
        if ( line[0].isSpace() )
            continue;
        QStringList parts = line.split( ' ' );
        QString repoName = parts.value( 0 );
        int a = repoName.indexOf( "/" );
        QString pkgName = repoName.mid( a+1 );
        QString pkgVersion = parts.value( 1 );
        packages.insert( pkgName, pkgVersion );
    }
    return packages;
}

QHash<QString, QString>
KernelModel::getInstalledPackages() const
{
    QProcess process;
    process.setEnvironment( QStringList() << "LANG=C" << "LC_MESSAGES=C" );
    process.start( "pacman", QStringList() << "-Qs" << "^linux[0-9][0-9]?([0-9])" );
    if ( !process.waitForFinished( 15000 ) )
        qDebug() << "error: failed to get all installed kernels";
    QString result = process.readAll();

    QHash<QString, QString> packages;
    for ( QString line : result.split( "\n", QString::SkipEmptyParts ) )
    {
        if ( line.isEmpty() )
            continue;
        if ( line[0].isSpace() )
            continue;
        QStringList parts = line.split( ' ' );
        QString repoName = parts.value( 0 );
        int a = repoName.indexOf( "/" );
        QString pkgName = repoName.mid( a+1 );
        QString pkgVersion = parts.value( 1 );
        packages.insert( pkgName, pkgVersion );
    }
    return packages;
}


/*
 * Returns the Kernel with the higher version and installed
 */
Kernel
KernelModel::latestInstalledKernel()
{
    Kernel auxKernel;
    for ( Kernel& kernel : kernels_ )
    {
        if ( !kernel.isInstalled() )
            continue;
        if ( kernel.majorVersion() > auxKernel.majorVersion() )
            auxKernel = kernel;
        else if ( ( kernel.majorVersion() == auxKernel.majorVersion() )
                  && ( kernel.minorVersion() > auxKernel.minorVersion() ) )
            auxKernel = kernel;
    }
    return auxKernel;
}


/*
 * Return a list of all unsupported kernels installed (installed but not available)
 */
QList<Kernel>
KernelModel::unsupportedKernels() const
{
    QList<Kernel> auxList;
    for ( const Kernel& kernel : kernels_ )
    {
        if ( kernel.isUnsupported() )
            auxList << kernel;
    }
    return auxList;
}


/*
 * Returns a list of all kernels with a higher version than the kernel supplied
 * and available in the repos
 */
QList<Kernel>
KernelModel::newerKernels( const Kernel auxKernel )
{
    QList<Kernel> auxList;
    for ( Kernel& kernel : kernels_ )
    {
        if ( !kernel.isAvailable() )
            continue;
        if ( kernel.majorVersion() > auxKernel.majorVersion() )
            auxList << kernel;
        else if ( ( kernel.majorVersion() == auxKernel.majorVersion() )
                  && ( kernel.minorVersion() > auxKernel.minorVersion() ) )
            auxList << kernel;
    }
    return auxList;
}


/* KernelSortFilterProxyModel class */
KernelSortFilterProxyModel::KernelSortFilterProxyModel( QObject* parent )
    : QSortFilterProxyModel( parent )
{
}


bool
KernelSortFilterProxyModel::lessThan( const QModelIndex& left,
                                      const QModelIndex& right ) const
{
    QVariant leftData = sourceModel()->data( left, sortRole() );
    QVariant rightData = sourceModel()->data( right, sortRole() );

    if ( sortRole() == KernelModel::VersionRole )
    {
        int leftMajor = sourceModel()->data( left, KernelModel::MajorVersionRole ).toInt();
        int rightMajor = sourceModel()->data( right, KernelModel::MajorVersionRole ).toInt();
        int leftMinor = sourceModel()->data( left, KernelModel::MinorVersionRole ).toInt();
        int rightMinor = sourceModel()->data( right, KernelModel::MinorVersionRole ).toInt();

        if ( leftMajor == rightMajor )
        {
            if ( leftMinor < rightMinor )
                return true;
            else
                return false;
        }
        else
        {
            if ( leftMajor < rightMajor )
                return true;
            else
                return false;
        }
    }
    else
        return QString::localeAwareCompare( leftData.toString(), rightData.toString() ) < 0;
}
