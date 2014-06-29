#ifndef KERNEL_H
#define KERNEL_H

#include <QAbstractListModel>
#include <QDebug>
#include <QObject>
#include <QProcess>
#include <QStringList>
#include <QSortFilterProxyModel>

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
    QStringList getAllInstalledKernels();
    QStringList getAllAvailableKernels();
    QString getRunningKernel();
    QString getKernelVersion(const QString &package);
    QStringList getKernelModules(const QString &package);
    QStringList getLtsKernels();
    QStringList getRecommendedKernels();
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
