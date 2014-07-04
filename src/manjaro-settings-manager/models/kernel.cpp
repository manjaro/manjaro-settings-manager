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
    QStringList installedKernels = getAllInstalledKernels();
    QStringList availableKernels = getAllAvailableKernels();
    QString runningKernel = getRunningKernel();
    QStringList ltsKernels = getLtsKernels();
    QStringList recommendedKernels = getRecommendedKernels();

    beginResetModel();
    kernels_.clear();
    for (QString kernelPackage : availableKernels) {
        Kernel kernel;
        kernel.setPackage(kernelPackage);
        kernel.setVersion(getKernelVersion(kernelPackage));
        kernel.setAvailable(true);
        if (installedKernels.contains(kernelPackage)) {
            kernel.setInstalled(true);
            kernel.setModules(getKernelModules(kernelPackage));
        } else {
            QStringList modules = getKernelModules(runningKernel);
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
    QStringList installedKernels = getAllInstalledKernels();
    QStringList availableKernels = getAllAvailableKernels();
    QString runningKernel = getRunningKernel();
    QStringList ltsKernels = getLtsKernels();
    QStringList recommendedKernels = getRecommendedKernels();

    for (Kernel &kernel : kernels_) {
        if (QString::compare(kernelPackage, kernel.package()) == 0) {
            kernel.setVersion(getKernelVersion(kernelPackage));
            if (availableKernels.contains(kernelPackage))
                kernel.setAvailable(true);
            if (installedKernels.contains(kernelPackage)) {
                kernel.setInstalled(true);
                kernel.setModules(getKernelModules(kernelPackage));
            } else {
                kernel.setModules(getKernelModules(runningKernel));
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


QStringList KernelModel::getAllInstalledKernels()
{
    QProcess process;
    process.setEnvironment(QStringList() << "LANG=C" << "LC_MESSAGES=C");
    process.start("pacman", QStringList() << "-Qqs" << "^linux[0-9][0-9]?([0-9])$");
    if (!process.waitForFinished(15000))
        qDebug() << "error: failed to get all installed kernels";
    QString result = process.readAll();
    return result.split("\n", QString::SkipEmptyParts);
}


QStringList KernelModel::getAllAvailableKernels()
{
    QProcess process;
    process.setEnvironment(QStringList() << "LANG=C" << "LC_MESSAGES=C");
    process.start("pacman", QStringList() << "-Sqs" << "^linux[0-9][0-9]?([0-9])$");
    if (!process.waitForFinished(15000))
        qDebug() << "error: failed to get all available kernels";
    QString result = process.readAll();
    return result.split("\n", QString::SkipEmptyParts);
}


QString KernelModel::getRunningKernel()
{
    QProcess process;
    process.setEnvironment(QStringList() << "LANG=C" << "LC_MESSAGES=C");
    process.start("uname", QStringList() << "-r");
    if (!process.waitForFinished(15000))
        qDebug() << "error: failed to determine running kernel";
    QString result = process.readAll();
    QStringList aux = result.split(".", QString::SkipEmptyParts);
    return QString("linux%1%2").arg(aux.at(0)).arg(aux.at(1));
}


QString KernelModel::getKernelVersion(const QString &package)
{
    QProcess process;
    process.setEnvironment(QStringList() << "LANG=C" << "LC_MESSAGES=C");
    process.start("pacman", QStringList() << "-Si" << package);
    if (!process.waitForFinished(15000))
        qDebug() << "error: failed to determine kernel version!";
    QString result = process.readAll();
    QStringList pkgInfo = result.split("\n", QString::SkipEmptyParts);
    return QStringList(pkgInfo.at(2).split(":", QString::SkipEmptyParts)).last().simplified();
}


QStringList KernelModel::getKernelModules(const QString &package)
{
    QProcess process;
    process.setEnvironment(QStringList() << "LANG=C" << "LC_MESSAGES=C");
    process.start("pacman", QStringList() << "-Qqs" << package << "-g" << package + "-extramodules");
    if (!process.waitForFinished(15000))
        qDebug() << "error: failed to get installed kernel modules";
    QString result = process.readAll();
    QStringList kernelModules = result.split("\n", QString::SkipEmptyParts);

    process.start("pacman", QStringList() << "-Qqs" << (package + "-headers"));
    if (!process.waitForFinished(15000))
        qDebug() << "error: failed to get installed headers";
    result = process.readAll();
    kernelModules.append(result.split("\n", QString::SkipEmptyParts));
    return kernelModules;
}


QStringList KernelModel::getLtsKernels()
{
    return QStringList() << "linux34" << "linux310" << "linux312" << "linux314";
}

QStringList KernelModel::getRecommendedKernels()
{
    return QStringList() << "linux310" << "linux312" << "linux314";
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
        int leftMinor = leftStringList.at(1).toInt();
        int rightMinor = rightStringList.at(1).toInt();

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
