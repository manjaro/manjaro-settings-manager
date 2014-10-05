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

#include "SelectLocalesDialog.h"
#include "ui_SelectLocalesDialog.h"

SelectLocalesDialog::SelectLocalesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectLocalesDialog)
{
    ui->setupUi(this);

    // Connect signals and slots
    connect(ui->languageListView, &QListView::activated,
            this, &SelectLocalesDialog::languageListViewActivated);
    connect(ui->countryListView, &QListView::activated,
            this, &SelectLocalesDialog::countryListViewActivated);
    connect(ui->buttonCancel, &QPushButton::clicked,
            this, &SelectLocalesDialog::close);
    connect(ui->buttonAdd, &QPushButton::clicked,
            this, &SelectLocalesDialog::buttonAdd_clicked);
}


SelectLocalesDialog::~SelectLocalesDialog()
{
    delete ui;
    delete supportedLocalesModel_;
    delete languageSortProxy_;
    delete countrySortProxy_;
    delete languageSelectionProxy_;
    delete countrySelectionProxy_;
}


bool SelectLocalesDialog::localeAdded()
{
    return accepted_;
}


QString SelectLocalesDialog::getLocale()
{
    if (ui->localeComboBox->count() <= 0) {
        return QString();
    }
    return ui->localeComboBox->currentText();
}


int SelectLocalesDialog::exec()
{
    accepted_ = false;
    supportedLocalesModel_ = new SupportedLocalesModel();

    languageSortProxy_ = new QSortFilterProxyModel();
    languageSortProxy_->setSourceModel(supportedLocalesModel_);
    languageSortProxy_->setSortLocaleAware(true);
    languageSortProxy_->sort(0, Qt::AscendingOrder);
    ui->languageListView->setModel(languageSortProxy_);

    languageSelectionProxy_ = new KSelectionProxyModel(ui->languageListView->selectionModel(), this);
    languageSelectionProxy_->setSourceModel(supportedLocalesModel_);
    languageSelectionProxy_->setFilterBehavior(KSelectionProxyModel::ChildrenOfExactSelection);

    countrySortProxy_ = new QSortFilterProxyModel();
    countrySortProxy_->setSourceModel(languageSelectionProxy_);
    countrySortProxy_->setSortLocaleAware(true);
    countrySortProxy_->sort(0, Qt::AscendingOrder);
    ui->countryListView->setModel(countrySortProxy_);

    countrySelectionProxy_ = new KSelectionProxyModel(ui->countryListView->selectionModel(), this);
    countrySelectionProxy_->setSourceModel(supportedLocalesModel_);
    countrySelectionProxy_->setFilterBehavior(KSelectionProxyModel::ChildrenOfExactSelection);
    ui->localeComboBox->setModel(countrySelectionProxy_);

    ui->localeComboBox->hide();
    updateApplyEnabledState();

    return QDialog::exec();
}


void SelectLocalesDialog::updateApplyEnabledState()
{
    ui->buttonAdd->setEnabled(ui->localeComboBox->isVisible());
}


void SelectLocalesDialog::languageListViewActivated(const QModelIndex &index)
{
    if (index.isValid()) {
        ui->localeComboBox->hide();
        updateApplyEnabledState();
    }
}


void SelectLocalesDialog::countryListViewActivated(const QModelIndex &index)
{
    if (index.isValid()) {
        /* Select locale with UTF-8 encoding by default */
        QAbstractItemModel *model = ui->localeComboBox->model();
        QModelIndexList localeIndexList = model->match(model->index(0,0),
                                                       SupportedLocalesModel::ValueRole,
                                                       "UTF-8",
                                                       -1,
                                                       Qt::MatchContains);
        if (localeIndexList.size() > 0) {
            QModelIndex modelIndex = localeIndexList.first();
            ui->localeComboBox->setCurrentIndex(modelIndex.row());
        }
        ui->localeComboBox->show();
        updateApplyEnabledState();
    }
}


void SelectLocalesDialog::buttonAdd_clicked()
{
    accepted_ = true;
    close();
}
