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

#ifndef SELECTLOCALESDIALOG_H
#define SELECTLOCALESDIALOG_H

#include "SupportedLocalesModel.h"

#include <KF5/KItemModels/KSelectionProxyModel>

#include <QtWidgets/QDialog>
#include <QtCore/QSortFilterProxyModel>

namespace Ui
{
class SelectLocalesDialog;
}

class SelectLocalesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectLocalesDialog( QWidget* parent = 0 );
    ~SelectLocalesDialog();

    int exec();
    bool localeAdded();
    QString getLocale();

private:
    Ui::SelectLocalesDialog* ui;
    SupportedLocalesModel* m_supportedLocalesModel;
    QSortFilterProxyModel* m_languageSortProxy;
    QSortFilterProxyModel* m_countrySortProxy;
    KSelectionProxyModel* m_languageSelectionProxy;
    KSelectionProxyModel* m_countrySelectionProxy;
    bool m_accepted;

    void updateApplyEnabledState();

private slots:
    void hideLocaleComboBox( const QModelIndex& index );
    void showLocaleComboBox( const QModelIndex& index );
    void buttonAdd_clicked();

};

#endif // SELECTLOCALESDIALOG_H
