/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Roland Singer <roland@manjaro.org>
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

#ifndef KEYBOARDMODULE_H
#define KEYBOARDMODULE_H

#include "KeyboardModel.h"
#include "KeyboardPreview.h"

#include <KF5/KItemModels/KSelectionProxyModel>
#include <KCModule>

namespace Ui
{
class PageKeyboard;
}

class KeyboardModule : public KCModule
{
    Q_OBJECT

public:
    /**
     * Constructor.
     *
     * @param parent Parent widget of the module
     * @param args Arguments for the module
     */
    explicit KeyboardModule( QWidget* parent, const QVariantList& args = QVariantList() );
    /**
     * Destructor.
     */
    ~KeyboardModule();

    /**
     * Overloading the KCModule load() function.
     */
    void load();

    /**
     * Overloading the KCModule save() function.
     */
    void save();

    /**
     * Overloading the KCModule defaults() function.
     */
    void defaults();

private:
    Ui::PageKeyboard* ui;
    KeyboardModel* m_keyboardModel;
    QSortFilterProxyModel* m_keyboardProxyModel;
    KeyBoardPreview* m_keyboardPreview;
    KSelectionProxyModel* m_layoutsSelectionProxy;
    QSortFilterProxyModel* m_variantsSortProxy;

    void setLayoutsListViewIndex( const QString& layout );
    void setVariantsListViewIndex( const QString& variant );
    void setModelComboBoxIndex( const QString& model );

private slots:
    void setNewLayout( const QModelIndex& index );
    void setNewVariant( const QModelIndex& index );
};

#endif // KEYBOARDMODULE_H
