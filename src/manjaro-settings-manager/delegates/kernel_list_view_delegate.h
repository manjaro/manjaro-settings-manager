#ifndef KERNEL_LIST_VIEW_DELEGATE_H
#define KERNEL_LIST_VIEW_DELEGATE_H

#include "models/kernel.h"
#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QSize>
#include <QMouseEvent>
#include <QPainter>
#include <QApplication>
#include <QStyleOptionButton>

class KernelListViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    KernelListViewDelegate(QObject *parent = 0);
    virtual ~KernelListViewDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    void installButtonClicked(const QModelIndex &index);
    void infoButtonClicked(const QModelIndex &index);

private:
    QStyle::State stateInstallButton_;
    QStyle::State stateInfoButton_;
};

#endif // KERNEL_LIST_VIEW_DELEGATE_H
