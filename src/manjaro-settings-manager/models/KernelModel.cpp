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

#include <global.h>

#include <QtCore/QRegularExpression>
#include <QtCore/QSet>

KernelModel::KernelModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

/*
 * Clear and update ther list of kernels
 */
void KernelModel::update()
{
    QStringList installedKernelPackages = getInstalledPackages();
    QStringList availableKernelPackages = getAvailablePackages();
    QStringList allKernelPackages = QStringList() << installedKernelPackages << availableKernelPackages;
    allKernelPackages.removeDuplicates();
    QString runningKernel = Global::getRunningKernel();
    QStringList ltsKernels = Global::getLtsKernels();
    QStringList recommendedKernels = Global::getRecommendedKernels();

    QSet<QString> modulesToInstall;
    for (const QString &module : installedKernelPackages.filter(QRegularExpression("^linux[0-9][0-9]?([0-9])-"))) {
        QString aux = QString(module).remove(QRegularExpression("^linux[0-9][0-9]?([0-9])-"));
        modulesToInstall.insert(aux);
    }

    beginResetModel();
    kernels_.clear();
    for (const QString &kernel : allKernelPackages.filter(QRegularExpression("^linux[0-9][0-9]?([0-9])$"))) {
        Kernel newKernel;

        newKernel.setPackage(kernel);
        newKernel.setAvailable(availableKernelPackages.contains(kernel));
        newKernel.setInstalled(installedKernelPackages.contains(kernel));

        QString pkgInfo = Global::packageInformation(kernel, !newKernel.isAvailable());
        newKernel.setVersion(Global::packageVersion(pkgInfo));

        newKernel.setAvailableModules(availableKernelPackages
                                      .filter(QRegularExpression(QString("^%1-").arg(kernel))));
        if (newKernel.isInstalled()) {
            newKernel.setInstalledModules(installedKernelPackages
                                          .filter(QRegularExpression(QString("^%1-").arg(kernel))));
        } else {
            QStringList installableModules;
            for (const QString &module : modulesToInstall) {
                QString modulePackage = QString("%1-%2").arg(kernel).arg(module);
                if (availableKernelPackages.contains(modulePackage)) {
                    installableModules << modulePackage;
                }
            }
            newKernel.setInstalledModules(installableModules);
        }

        newKernel.setLts(ltsKernels.contains(kernel));
        newKernel.setRecommended(recommendedKernels.contains(kernel));
        newKernel.setRunning(QString::compare(runningKernel, kernel) == 0);

        kernels_.append(newKernel);
    }
    endResetModel();
}


void KernelModel::add(const Kernel &kernel)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    kernels_ << kernel;
    endInsertRows();
}


bool KernelModel::remove(int position, int rows, const QModelIndex &parent)
{
    Q_UNUSED(parent)
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    for (int row = 0; row < rows; ++row) {
        kernels_.removeAt(position);
    }
    endRemoveRows();
    return true;
}


int KernelModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return kernels_.size();
}


QVariant KernelModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= kernels_.count())
        return QVariant();
    const Kernel &kernel = kernels_[index.row()];
    if (role == PackageRole)
        return kernel.package();
    else if (role == VersionRole)
        return kernel.version();
    else if (role == MajorVersionRole)
        return kernel.majorVersion();
    else if (role == MinorVersionRole)
        return kernel.minorVersion();
    else if (role == InstalledModulesRole)
        return kernel.installedModules();
    else if (role == AvailableModulesRole)
        return kernel.availableModules();
    else if (role == IsAvailableRole)
        return kernel.isAvailable();
    else if (role == IsInstalledRole)
        return kernel.isInstalled();
    else if (role == IsLtsRole)
        return kernel.isLts();
    else if (role == IsRecommendedRole)
        return kernel.isRecommended();
    else if (role == IsRunningRole)
        return kernel.isRunning();
    else if (role == IsUnsupportedRole)
        return kernel.isUnsupported();
    return QVariant();
}


QHash<int, QByteArray> KernelModel::roleNames() const
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
    return roles;
}


QStringList KernelModel::getAvailablePackages() const
{
    QProcess process;
    process.setEnvironment(QStringList() << "LANG=C" << "LC_MESSAGES=C");
    process.start("pacman", QStringList() << "-Sqs" << "^linux[0-9][0-9]?([0-9])");
    if (!process.waitForFinished(15000))
        qDebug() << "error: failed to get all installed kernels";
    QString result = process.readAll();
    return result.split("\n", QString::SkipEmptyParts);
}


QStringList KernelModel::getInstalledPackages() const
{
    QProcess process;
    process.setEnvironment(QStringList() << "LANG=C" << "LC_MESSAGES=C");
    process.start("pacman", QStringList() << "-Qqs" << "^linux[0-9][0-9]?([0-9])");
    if (!process.waitForFinished(15000))
        qDebug() << "error: failed to get all installed kernels";
    QString result = process.readAll();
    return result.split("\n", QString::SkipEmptyParts);
}


/*
 * Returns the Kernel with the higher version and installed
 */
Kernel KernelModel::latestInstalledKernel()
{
    Kernel auxKernel;
    for ( Kernel &kernel : kernels_ ) {
        if ( !kernel.isInstalled() ) {
            continue;
        }
        if (kernel.majorVersion() > auxKernel.majorVersion()) {
            auxKernel = kernel;
        } else if ( (kernel.majorVersion() == auxKernel.majorVersion())
                     && (kernel.minorVersion() > auxKernel.minorVersion()) ) {
            auxKernel = kernel;
        }
    }
    return auxKernel;
}

/*
 * Return a list of all unsupported kernels installed (installed but not available)
 */
QList<Kernel> KernelModel::unsupportedKernels() const
{
    QList<Kernel> auxList;
    for (const Kernel &kernel : kernels_) {
        if (kernel.isUnsupported()) {
            auxList << kernel;
        }
    }
    return auxList;
}

/*
 * Returns a list of all kernels with a higher version than the kernel supplied
 * and available in the repos
 */
QList<Kernel> KernelModel::newerKernels(const Kernel auxKernel)
{
    QList<Kernel> auxList;
    for ( Kernel &kernel : kernels_ ) {
        if (!kernel.isAvailable()) {
            continue;
        }
        if (kernel.majorVersion() > auxKernel.majorVersion()) {
            auxList << kernel;
        } else if ( (kernel.majorVersion() == auxKernel.majorVersion())
                     && (kernel.minorVersion() > auxKernel.minorVersion()) ) {
            auxList << kernel;
        }
    }
    return auxList;
}

/* KernelSortFilterProxyModel class */
KernelSortFilterProxyModel::KernelSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool KernelSortFilterProxyModel::lessThan(const QModelIndex &left,
                                      const QModelIndex &right) const
{
    QVariant leftData = sourceModel()->data(left, sortRole());
    QVariant rightData = sourceModel()->data(right, sortRole());

    if (sortRole() == KernelModel::VersionRole) {

        int leftMajor = sourceModel()->data(left, KernelModel::MajorVersionRole).toInt();
        int rightMajor = sourceModel()->data(right, KernelModel::MajorVersionRole).toInt();
        int leftMinor = sourceModel()->data(left, KernelModel::MinorVersionRole).toInt();
        int rightMinor = sourceModel()->data(right, KernelModel::MinorVersionRole).toInt();

        if (leftMajor == rightMajor) {
            if (leftMinor < rightMinor) {
                return true;
            } else {
                return false;
            }
        } else {
            if (leftMajor < rightMajor) {
                return true;
            } else {
                return false;
            }
        }
    } else {
        return QString::localeAwareCompare(leftData.toString(), rightData.toString()) < 0;
    }
}
