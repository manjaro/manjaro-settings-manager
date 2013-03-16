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

#include "page_language.h"
#include "ui_page_language.h"

Page_Language::Page_Language(QWidget *parent) :
    PageWidget(parent),
    ui(new Ui::Page_Language)
{
    ui->setupUi(this);
    setTitel(tr("Language"));
    setIcon(QPixmap(":/images/resources/locale.png"));
    setShowApplyButton(true);

    ui->treeWidget->setColumnWidth(0, 190);
    ui->treeWidget->setColumnWidth(1, 190);
    ui->treeWidget->setColumnWidth(2, 190);
    ui->treeWidget->setColumnWidth(3, 30);

    connect(ui->buttonRemove, SIGNAL(clicked()) ,   this, SLOT(buttonRemove_clicked()));
    connect(ui->buttonRestore, SIGNAL(clicked())    ,   this, SLOT(buttonRestore_clicked()));
    connect(ui->buttonAdd, SIGNAL(clicked())    ,   this, SLOT(buttonAdd_clicked()));
}



Page_Language::~Page_Language()
{
    delete ui;
}



void Page_Language::activated() {
    ui->treeWidget->clear();

    QString currentLocale = Global::getCurrentLocale();
    QList<Global::LocaleInfo> locales = Global::getAllEnabledLocales();

    for (int i = 0; i < locales.size(); i++) {
        const Global::LocaleInfo *locale = &locales.at(i);

        TreeWidgetItem *item = new TreeWidgetItem(ui->treeWidget);
        item->setText(0, locale->locale);
        item->setText(1, locale->language);
         item->setText(2, locale->territory);

        if (currentLocale == locale->locale)
            item->radioButton.setChecked(true);

        ui->treeWidget->setItemWidget(item, 3, &item->radioButton);
    }

    ui->treeWidget->sortItems(0, Qt::AscendingOrder);
}



void Page_Language::apply_clicked() {
    QString systemLocale;
    QStringList locales;

    for(int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i) {
        TreeWidgetItem *item = dynamic_cast<TreeWidgetItem*>(ui->treeWidget->topLevelItem(i));
        if (!item)
            continue;

       locales.append(item->text(0));

       if (item->radioButton.isChecked())
           systemLocale = item->text(0);
    }


    QFile file(LOCALE_GEN);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error!"), tr("Failed to open file '%1'!").arg(LOCALE_GEN), QMessageBox::Ok, QMessageBox::Ok);
        activated();
        return;
    }

    QStringList content;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        content.append(line);
        line = line.trimmed();

        bool found = false;

        foreach (QString locale, locales) {
            if (line.startsWith(locale + " ")) {
                found = true;
                locales.removeAll(locale);
                break;
            }

            if (line.startsWith("#" + locale + " ")) {
                content.removeLast();
                content.append(line.remove(0, 1));
                found = true;
                locales.removeAll(locale);
                break;
            }
        }

        if (!found && !line.split("#", QString::KeepEmptyParts).first().trimmed().isEmpty()) {
            content.removeLast();
            content.append("#" + line);
        }
    }
    file.close();

    // Add missing locales
    foreach (QString locale, locales) {
        QString str = Global::localeToValidLocaleGenString(locale);

        if (str.isEmpty()) {
            QMessageBox::warning(this, tr("Error!"), tr("Failed to obtain valid locale string for locale '%1'!").arg(locale), QMessageBox::Ok, QMessageBox::Ok);
            continue;
        }

        content.append(str);
    }


    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error!"), tr("Failed to open file '%1'!").arg(LOCALE_GEN), QMessageBox::Ok, QMessageBox::Ok);
        activated();
        return;
    }

    QTextStream out(&file);
    out << content.join("\n");
    file.close();



    file.setFileName(LOCALE_CONF);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error!"), tr("Failed to open file '%1'!").arg(LOCALE_CONF), QMessageBox::Ok, QMessageBox::Ok);
        activated();
        return;
    }

    content.clear();
    in.setDevice(&file);
    bool found = false;

    while (!in.atEnd()) {
        QString line = in.readLine();
        content.append(line);
        line = line.trimmed();

        if (!line.startsWith("LANG="))
            continue;

        content.removeLast();
        content.append("LANG=" + systemLocale);
        found = true;
    }
    file.close();

    if (!found)
        content.append("LANG=" + systemLocale);


    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error!"), tr("Failed to open file '%1'!").arg(LOCALE_CONF), QMessageBox::Ok, QMessageBox::Ok);
        activated();
        return;
    }

    out.setDevice(&file);
    out << content.join("\n");
    file.close();


    ApplyDialog dialog(this);
    dialog.exec("locale-gen", QStringList(), tr("Generating locale.gen file..."), false);

    if (dialog.processSuccess()) {
        QMessageBox::information(this, tr("Hint"), tr("You might have to restart the graphical environment to apply the new settings..."), QMessageBox::Ok, QMessageBox::Ok);
    }

    emit closePage(this);
}



//###
//### Private
//##


void Page_Language::buttonRemove_clicked() {
    TreeWidgetItem *item = dynamic_cast<TreeWidgetItem*>(ui->treeWidget->currentItem());

    if (ui->treeWidget->selectedItems().size() <= 0 || ui->treeWidget->topLevelItemCount() <= 1 || !item)
        return;

    // Set check on another item if this one had the check
    if (item->radioButton.isChecked()) {
        TreeWidgetItem *checkItem = dynamic_cast<TreeWidgetItem*>(ui->treeWidget->itemAbove(item));
        if (checkItem) {
            checkItem->radioButton.setChecked(true);
        }
        else {
            checkItem = dynamic_cast<TreeWidgetItem*>(ui->treeWidget->itemBelow(item));
            if (checkItem)
                checkItem->radioButton.setChecked(true);
        }
    }

    delete item;
}



void Page_Language::buttonRestore_clicked() {
    activated();
}



void Page_Language::buttonAdd_clicked() {
    SelectLocalesDialog dialog(this);
    dialog.exec();

    if (!dialog.localeAdded())
        return;

    Global::LocaleInfo locale = dialog.getLocale();
    if (locale.locale.isEmpty())
        return;

    // Check if already in list
    for(int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i) {
       if (ui->treeWidget->topLevelItem(i)->text(0) == locale.locale)
           return;
    }

    TreeWidgetItem *item = new TreeWidgetItem(ui->treeWidget);
    item->setText(0, locale.locale);
    item->setText(1, locale.language);
    item->setText(2, locale.territory);
    ui->treeWidget->setItemWidget(item, 3, &item->radioButton);
    ui->treeWidget->sortItems(0, Qt::AscendingOrder);
}
