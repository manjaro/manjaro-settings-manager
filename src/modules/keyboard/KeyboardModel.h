/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Ramon Buldó <ramon@manjaro.org>
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

#ifndef KEYBOARDMODEL_H
#define KEYBOARDMODEL_H

#include "KeyboardItem.h"

#include <QtCore/QAbstractItemModel>
#include <QtCore/QSortFilterProxyModel>
#include <QtCore/QXmlStreamReader>


class KeyboardModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum KeyboardRoles
    {
        KeyRole = Qt::UserRole + 1,
        DescriptionRole
    };

    explicit KeyboardModel( QObject* parent = 0 );
    ~KeyboardModel();

    QVariant data( const QModelIndex& index, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;
    QVariant headerData( int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole ) const;
    QModelIndex index( int row, int column,
                       const QModelIndex& parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex& index ) const;
    int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    int columnCount( const QModelIndex& parent = QModelIndex() ) const;

    QString layout() const;
    QString variant() const;
    QString model() const;
    int rate() const;
    int delay() const;
    QString newLayout() const;
    QString newVariant() const;

    void setNewLayout(const QString &newLayout);
    void setNewVariant(const QString &newVariant);
    void setNewModel(const QString &newModel);
    void setNewRate(int newRate);
    void setNewDelay(int newDelay);

    bool saveKeyboardLayout();
    void saveRateAndDelay();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    void initModel( KeyboardItem* parent );
    void initLayout();
    void initRateAndDelay();
    void processLayoutLists();
    void processLayout();
    void processVariantList();
    void processVariant();
    void processModelList();
    void processModel();
    QString readNextText();
    QString errorString();

    QXmlStreamReader xml;

    KeyboardItem* m_rootItem;
    KeyboardItem* m_layoutsRoot;
    KeyboardItem* m_modelsRoot;
    KeyboardItem* m_currentlayout;

    QString m_layout;
    QString m_variant;
    QString m_model;
    int m_rate;
    int m_delay;

    QString m_newLayout;
    QString m_newVariant;
    QString m_newModel;
    int m_newRate;
    int m_newDelay;

signals:
    void changed();
};

#endif // KEYBOARDMODEL__H
