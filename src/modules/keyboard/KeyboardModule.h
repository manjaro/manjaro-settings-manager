/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
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

class PageKeyboard : public KCModule
{
    Q_OBJECT

public:
    /**
     * Constructor.
     *
     * @param parent Parent widget of the module
     * @param args Arguments for the module
     */
    explicit PageKeyboard( QWidget* parent, const QVariantList& args = QVariantList() );
    /**
     * Destructor.
     */
    ~PageKeyboard();

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

    QString m_currentLayout;
    QString m_currentVariant;
    QString m_currentModel;

    void setKeyboardLayout();
    void configureKeystroke();
    void setLayoutsListViewIndex( const QString& layout );
    void setVariantsListViewIndex( const QString& variant );
    void setModelComboBoxIndex( const QString& model );

    int getKeyboardRate();
    int getKeyboardDelay();
};

#endif // KEYBOARDMODULE_H
