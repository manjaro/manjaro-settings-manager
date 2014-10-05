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

#ifndef SELECTLOCALESDIALOG_H
#define SELECTLOCALESDIALOG_H

#include "models/SupportedLocalesModel.h"

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QPushButton>
#include <QComboBox>
#include <QSortFilterProxyModel>

#include <KF5/KItemModels/KSelectionProxyModel>

namespace Ui {
class SelectLocalesDialog;
}


class SelectLocalesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SelectLocalesDialog(QWidget *parent = 0);
    ~SelectLocalesDialog();

    int exec();
    bool localeAdded();
    QString getLocale();
    
private:
    Ui::SelectLocalesDialog *ui;
    SupportedLocalesModel *supportedLocalesModel_;
    QSortFilterProxyModel *languageSortProxy_;
    QSortFilterProxyModel *countrySortProxy_;
    KSelectionProxyModel *languageSelectionProxy_;
    KSelectionProxyModel *countrySelectionProxy_;
    bool accepted_;

    void updateApplyEnabledState();

private slots:
    void languageListViewActivated(const QModelIndex &index);
    void countryListViewActivated(const QModelIndex &index);
    void buttonAdd_clicked();

};

#endif // SELECTLOCALESDIALOG_H
