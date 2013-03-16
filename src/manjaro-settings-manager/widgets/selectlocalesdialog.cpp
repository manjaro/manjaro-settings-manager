/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
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

#include "selectlocalesdialog.h"
#include "ui_selectlocalesdialog.h"

SelectLocalesDialog::SelectLocalesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectLocalesDialog)
{
    ui->setupUi(this);

    // Connect signals and slots
    connect(ui->listWidgetLanguage, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*))   ,   this, SLOT(listWidgetLanguageItemChanged(QListWidgetItem*,QListWidgetItem*)));
    connect(ui->listWidgetTerritory, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*))   ,   this, SLOT(listWidgetTerritoryItemChanged(QListWidgetItem*,QListWidgetItem*)));
    connect(ui->comboBoxLocale, SIGNAL(currentIndexChanged(QString))    ,   this, SLOT(comboBoxLocaleIndexChanged(QString)));
    connect(ui->buttonCancel, SIGNAL(clicked())   ,   this, SLOT(close()));
    connect(ui->buttonAdd, SIGNAL(clicked())    ,   this, SLOT(buttonAdd_clicked()));
}



SelectLocalesDialog::~SelectLocalesDialog()
{
    delete ui;
}


bool SelectLocalesDialog::localeAdded() {
    return accepted;
}


QString SelectLocalesDialog::getLocale() {
    if (ui->comboBoxLocale->count() <= 0)
        return "";

    return ui->comboBoxLocale->currentText();
}



int SelectLocalesDialog::exec() {
    accepted = false;
    ui->listWidgetLanguage->clear();
    ui->listWidgetTerritory->clear();
    ui->comboBoxLocale->clear();
    ui->comboBoxLocale->hide();

    // Block signals
    ui->listWidgetLanguage->setEnabled(false);
    ui->listWidgetLanguage->blockSignals(true);
    ui->listWidgetTerritory->blockSignals(true);
    ui->comboBoxLocale->blockSignals(true);


    // Setup locales
    locales = Global::getAllLocales();

    QStringList keys = locales.keys();
    keys.sort();

    for (int i = 0; i < keys.size(); ++i) {
        ui->listWidgetLanguage->addItem(keys.at(i));
    }


    // Enable signals
    ui->listWidgetLanguage->setEnabled(true);
    ui->listWidgetLanguage->blockSignals(false);
    ui->listWidgetTerritory->blockSignals(false);
    ui->comboBoxLocale->blockSignals(false);

    updateApplyEnabledState();

    return QDialog::exec();
}



void SelectLocalesDialog::updateApplyEnabledState() {
    ui->buttonAdd->setEnabled(ui->comboBoxLocale->count() > 0);
}




//###
//### Private
//###


void SelectLocalesDialog::listWidgetLanguageItemChanged(QListWidgetItem *current, QListWidgetItem*) {
    // Block signals
    ui->comboBoxLocale->blockSignals(true);
    ui->listWidgetTerritory->blockSignals(true);


    // Clear fields first
    ui->listWidgetTerritory->clear();
    ui->labelDescription->clear();
    ui->comboBoxLocale->clear();
    ui->comboBoxLocale->hide();
    currentTerritories.clear();


    if (!locales.contains(current->text()))
        return;

    currentTerritories = locales.value(current->text());

    QStringList keys = currentTerritories.keys();
    keys.sort();

    for (int i = 0; i < keys.size(); ++i) {
        ui->listWidgetTerritory->addItem(keys.at(i));
    }


    // Enable signals
    ui->listWidgetTerritory->blockSignals(false);
    ui->comboBoxLocale->blockSignals(false);

    updateApplyEnabledState();
}




void SelectLocalesDialog::listWidgetTerritoryItemChanged(QListWidgetItem *current, QListWidgetItem*) {
    // Block signals
    ui->comboBoxLocale->blockSignals(true);


    QString currentText = current->text();

    // Clear fields first
    ui->labelDescription->clear();
    ui->comboBoxLocale->clear();
    ui->comboBoxLocale->show();
    currentLocales.clear();

    if (!currentTerritories.contains(currentText))
        return;

    currentLocales = currentTerritories.value(currentText);

    // Find item and set text
    int index = -1;

    for (int i = 0; i < currentLocales.size(); ++i) {
        QString locale = currentLocales.at(i).locale;

        ui->comboBoxLocale->addItem(locale);

        if (locale.toLower().endsWith(".utf8") || locale.toLower().endsWith(".utf-8"))
            index = ui->comboBoxLocale->count() - 1;
    }

    // Set description
    if (index >= 0)
        ui->comboBoxLocale->setCurrentIndex(index);

    comboBoxLocaleIndexChanged(ui->comboBoxLocale->currentText());


    // Enable signals
    ui->comboBoxLocale->blockSignals(false);

    updateApplyEnabledState();
}



void SelectLocalesDialog::comboBoxLocaleIndexChanged(const QString &text) {
    // Clear fields first
    ui->labelDescription->clear();

    // Set right description
    for (int i = 0; i < currentLocales.size(); ++i) {
        if (currentLocales.at(i).locale != text)
            continue;

        ui->labelDescription->setText(currentLocales.at(i).description);
        break;
    }

    updateApplyEnabledState();
}



void SelectLocalesDialog::buttonAdd_clicked() {
    accepted = true;
    close();
}
