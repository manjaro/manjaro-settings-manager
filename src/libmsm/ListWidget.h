/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Roland Singer <roland@manjaro.org>
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

#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QListWidget>
#include <QListWidgetItem>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QFont>
#include <QPen>
#include <QModelIndex>

class ListWidgetItemSeperatorDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    ListWidgetItemSeperatorDelegate( QListWidget* m_parent, QListWidgetItem* m_item );

public slots:
    void parentResized();

private:
    QListWidget* m_parent;
    QListWidgetItem* m_item;

protected:
    virtual void paint ( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    virtual QSize sizeHint ( const QStyleOptionViewItem& option, const QModelIndex& index ) const;
};



class ListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit ListWidget( QWidget* parent = 0 );
    void addSeparator( QString text );

signals:
    void resized();

protected:
    void resizeEvent( QResizeEvent* e );

};

#endif // LISTWIDGET_H
