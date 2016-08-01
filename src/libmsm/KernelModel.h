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

#ifndef KERNELMODEL_H
#define KERNELMODEL_H

#include "Kernel.h"
#include <QtCore/QAbstractListModel>
#include <QtCore/QObject>
#include <QtCore/QSortFilterProxyModel>

class KernelModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum KernelRoles
    {
        PackageRole = Qt::UserRole + 1,
        VersionRole,
        MajorVersionRole,
        MinorVersionRole,
        AvailableModulesRole,
        InstalledModulesRole, /* Installed modules or will be installed if kernel is not installed */
        IsAvailableRole,
        IsInstalledRole,
        IsLtsRole,
        IsRecommendedRole,
        IsRunningRole,
        IsUnsupportedRole,
        IsRcRole,
        IsRealtimeRole
    };

    KernelModel( QObject* parent = 0 );

    void update();
    void add( const Kernel& kernel );
    int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    bool remove( int position, int rows, const QModelIndex& parent );
    Kernel latestInstalledKernel();
    QList<Kernel> newerKernels( const Kernel& auxKernel );
    QList<Kernel> unsupportedKernels() const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<Kernel> m_kernels;
    QHash<QString, QString> getInstalledPackages() const;
    QHash<QString, QString> getAvailablePackages() const;
    Kernel getRunningKernel() const;
    QStringList getLtsKernels() const;
    QStringList getRecommendedKernels() const;
};



class KernelSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    KernelSortFilterProxyModel( QObject* parent = 0 );

protected:
    bool lessThan( const QModelIndex& left, const QModelIndex& right ) const;
};

#endif // KERNELMODEL_H
