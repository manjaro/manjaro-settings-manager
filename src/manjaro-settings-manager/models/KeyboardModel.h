#ifndef KEYBOARDMODEL_H
#define KEYBOARDMODEL__H

#include "models/KeyboardItem.h"

#include <QtCore/QAbstractItemModel>
#include <QtCore/QSortFilterProxyModel>

class KeyboardModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum KeyboardRoles {
        KeyRole = Qt::UserRole + 1,
        DescriptionRole
    };

    explicit KeyboardModel(QObject *parent = 0);
    ~KeyboardModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    bool getCurrentKeyboardLayout(QString &layout, QString &variant, QString &model);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    void init(KeyboardItem *parent);

    KeyboardItem *rootItem_;

signals:

public slots:

};

#endif // KEYBOARDMODEL__H
