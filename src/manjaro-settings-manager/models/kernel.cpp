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

#include "kernel.h"

/* Kernel Class */
Kernel::Kernel() : package_(QString()), version_(QString()), modules_(QStringList()),
                   isAvailable_(false), isInstalled_(false), isLts_(false),
                   isRecommended_(false), isRunning_(false)
{
}

Kernel::Kernel(const QString &package,
               const QString &version,
               const QStringList &modules,
               const bool isAvailable,
               const bool isInstalled,
               const bool isLts,
               const bool isRecommended,
               const bool isRunning)
    : package_(package),
      version_(version),
      modules_(modules),
      isAvailable_(isAvailable),
      isInstalled_(isInstalled),
      isLts_(isLts),
      isRecommended_(isRecommended),
      isRunning_(isRunning)
{
}

Kernel::~Kernel()
{
}

QString Kernel::package() const { return package_; }
QString Kernel::version() const { return version_; }
QStringList Kernel::modules() const { return modules_; }
bool Kernel::isAvailable() const { return isAvailable_; }
bool Kernel::isInstalled() const { return isInstalled_; }
bool Kernel::isLts() const {return isLts_; }
bool Kernel::isRecommended() const { return isRecommended_; }
bool Kernel::isRunning() const { return isRunning_; }

void Kernel::setPackage(const QString &package)
{
    package_ = package;
}

void Kernel::setVersion(const QString &version)
{
    version_ = version;
}

void Kernel::setModules(const QStringList &modules)
{
    modules_ = modules;
}

void Kernel::setAvailable(const bool isAvailable)
{
    isAvailable_ = isAvailable;
}

void Kernel::setInstalled(const bool isInstalled)
{
    isInstalled_ = isInstalled;
}

void Kernel::setLts(const bool isLts)
{
    isLts_ = isLts;
}

void Kernel::setRecommended(const bool isRecommended)
{
    isRecommended_ = isRecommended;
}

void Kernel::setRunning(const bool isRunning)
{
    isRunning_ = isRunning;
}



/* KernelModel Class */
KernelModel::KernelModel(QObject *parent)
    : QAbstractListModel(parent)
{
    update();
}


void KernelModel::update()
{
    QStringList installedKernels = Global::getAllInstalledKernels();
    QStringList availableKernels = Global::getAllAvailableKernels();
    QString runningKernel = Global::getRunningKernel();
    QStringList ltsKernels = Global::getLtsKernels();
    QStringList recommendedKernels = Global::getRecommendedKernels();
    QStringList allKernels = QStringList() << installedKernels << availableKernels;
    allKernels.removeDuplicates();

    beginResetModel();
    kernels_.clear();
    for (QString kernelPackage : allKernels) {
        Kernel kernel;
        kernel.setPackage(kernelPackage);
        /* package available in the repository */
        if (availableKernels.contains(kernelPackage)){
            kernel.setAvailable(true);
            kernel.setVersion(Global::getKernelVersion(kernelPackage, false));
        } else {
            kernel.setVersion(Global::getKernelVersion(kernelPackage, true));
        }
        if (installedKernels.contains(kernelPackage)) {
            kernel.setInstalled(true);
            kernel.setModules(Global::getKernelModules(kernelPackage));
        } else {
            QStringList modules = Global::getKernelModules(runningKernel);
            for (auto &module : modules)
                module.replace(runningKernel, kernelPackage);
            kernel.setModules(modules);
        }
        if (QString::compare(runningKernel, kernelPackage) == 0)
            kernel.setRunning(true);
        if (ltsKernels.contains(kernelPackage))
            kernel.setLts(true);
        if (recommendedKernels.contains(kernelPackage))
            kernel.setRecommended(true);
        kernels_.append(kernel);
    }
    endResetModel();
}


void KernelModel::update(const QString kernelPackage)
{
    QStringList installedKernels = Global::getAllInstalledKernels();
    QStringList availableKernels = Global::getAllAvailableKernels();
    QString runningKernel = Global::getRunningKernel();
    QStringList ltsKernels = Global::getLtsKernels();
    QStringList recommendedKernels = Global::getRecommendedKernels();

    for (Kernel &kernel : kernels_) {
        if (QString::compare(kernelPackage, kernel.package()) == 0) {
            if (availableKernels.contains(kernelPackage)){
                kernel.setAvailable(true);
                kernel.setVersion(Global::getKernelVersion(kernelPackage, false));
            } else {
                kernel.setVersion(Global::getKernelVersion(kernelPackage, true));
            }
            if (installedKernels.contains(kernelPackage)) {
                kernel.setInstalled(true);
                kernel.setModules(Global::getKernelModules(kernelPackage));
            } else {
                kernel.setModules(Global::getKernelModules(runningKernel));
            }
            if (QString::compare(runningKernel, kernelPackage) == 0)
                kernel.setRunning(true);
            if (ltsKernels.contains(kernelPackage))
                kernel.setLts(true);
            if (recommendedKernels.contains(kernelPackage))
                kernel.setRecommended(true);
            // TODO: Implement signal kernelUpdated()
            // emit kernelUpdated();
            break;
        }
    }
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
    else if (role == ModulesRole)
        return kernel.modules();
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
    return QVariant();
}


QHash<int, QByteArray> KernelModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PackageRole] = "package";
    roles[VersionRole] = "version";
    roles[ModulesRole] = "modules";
    roles[IsAvailableRole] = "isAvailable";
    roles[IsInstalledRole] = "isInstalled";
    roles[IsLtsRole] = "isLts";
    roles[IsRecommendedRole] = "isRecommended";
    roles[IsRunningRole] = "isRunning";
    return roles;
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
        QStringList leftStringList = leftData.toString().split(".");
        QStringList rightStringList = rightData.toString().split(".");

        int leftMajor = leftStringList.at(0).toInt();
        int rightMajor = rightStringList.at(0).toInt();
        int leftMinor = leftStringList.at(1).left(2).toInt();
        int rightMinor = rightStringList.at(1).left(2).toInt();

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
