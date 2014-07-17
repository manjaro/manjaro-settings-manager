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

#ifndef KERNEL_H
#define KERNEL_H

#include <QAbstractListModel>
#include <QDebug>
#include <QObject>
#include <QProcess>
#include <QStringList>
#include <QSortFilterProxyModel>
#include <global.h>

class Kernel
{
public:
    Kernel();
    Kernel(const QString &package,
           const QString &version,
           const QStringList &modules,
           const bool isAvailable,
           const bool isInstalled,
           const bool isLts,
           const bool isRecommended,
           const bool isRunning);
    ~Kernel();

    QString package() const;
    QString version() const;
    QStringList modules() const;
    bool isAvailable() const;
    bool isInstalled() const;
    bool isLts() const;
    bool isRecommended() const;
    bool isRunning() const;
    void setPackage(const QString &package);
    void setVersion(const QString &version);
    void setModules(const QStringList &modules);
    void setAvailable(const bool isAvailable);
    void setInstalled(const bool isInstalled);
    void setLts(const bool isLts);
    void setRecommended(const bool isRecommended);
    void setRunning(const bool isRunning);

private:
    QString package_;
    QString version_;
    QStringList modules_;
    bool isAvailable_;
    bool isInstalled_;
    bool isLts_;
    bool isRecommended_;
    bool isRunning_;
};



class KernelModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum KernelRoles {
        PackageRole = Qt::UserRole + 1,
        VersionRole,
        ModulesRole,
        IsAvailableRole,
        IsInstalledRole,
        IsLtsRole,
        IsRecommendedRole,
        IsRunningRole
    };

    KernelModel(QObject *parent = 0);

    void update();
    void update(const QString package);
    void add(const Kernel &kernel);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool remove(int position, int rows, const QModelIndex &parent);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<Kernel> kernels_;
};



class KernelSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    KernelSortFilterProxyModel(QObject *parent = 0);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

#endif // KERNEL_H
