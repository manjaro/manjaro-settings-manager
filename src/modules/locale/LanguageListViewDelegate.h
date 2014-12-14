#ifndef LANGUAGELISTVIEWDELEGATE_H
#define LANGUAGELISTVIEWDELEGATE_H

#include <QtGui/QPainter>
#include <QtWidgets/QStyledItemDelegate>

class LanguageListViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit LanguageListViewDelegate(QObject *parent = 0);
    virtual ~LanguageListViewDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

};

#endif // LANGUAGELISTVIEWDELEGATE_H
