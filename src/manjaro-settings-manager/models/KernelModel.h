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

#ifndef KERNELMODEL_H
#define KERNELMODEL_H

#include <models/Kernel.h>
#include <QAbstractListModel>
#include <QObject>
#include <QSortFilterProxyModel>


class KernelModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum KernelRoles {
        PackageRole = Qt::UserRole + 1,
        VersionRole,
        MajorVersionRole,
        MinorVersionRole,
        AvailableModulesRole,
        InstalledModulesRole,
        IsAvailableRole,
        IsInstalledRole,
        IsLtsRole,
        IsRecommendedRole,
        IsRunningRole,
        IsUnsupportedRole
    };

    KernelModel(QObject *parent = 0);

    void update();
    void update(const QString package);
    void add(const Kernel &kernel);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool remove(int position, int rows, const QModelIndex &parent);
    Kernel latestInstalledKernel();
    QList<Kernel> unsupportedKernels();
    QList<Kernel> newerKernels(const Kernel auxKernel);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<Kernel> kernels_;
    QStringList installedKernelPackages_;
    QStringList availableKernelPackages_;
    void updateKernelPackages();
};



class KernelSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    KernelSortFilterProxyModel(QObject *parent = 0);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

#endif // KERNELMODEL_H

