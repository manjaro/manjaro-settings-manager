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

#include "PageLanguage.h"
#include "ui_PageLanguage.h"

#include <QtDBus/QDBusInterface>

PageLanguage::PageLanguage(QWidget *parent) :
    PageWidget(parent),
    ui(new Ui::PageLanguage)
{
    ui->setupUi(this);
    setTitel(tr("Language"));
    setIcon(QPixmap(":/images/resources/locale.png"));
    setShowApplyButton(true);

    ui->treeWidget->setColumnWidth(0, 150);
    ui->treeWidget->setColumnWidth(1, 150);
    ui->treeWidget->setColumnWidth(2, 150);
    ui->treeWidget->setColumnWidth(3, 150);
    ui->treeWidget->setColumnWidth(4, 150);

    connect(ui->buttonRemove, SIGNAL(clicked()) ,   this, SLOT(buttonRemove_clicked()));
    connect(ui->buttonRestore, SIGNAL(clicked())    ,   this, SLOT(buttonRestore_clicked()));
    connect(ui->buttonAdd, SIGNAL(clicked())    ,   this, SLOT(buttonAdd_clicked()));
}



PageLanguage::~PageLanguage()
{
    delete ui;
}



void PageLanguage::activated() {
    ui->treeWidget->clear();

    QString currentLocale = Global::getCurrentLocale();
    QString currentFormats = Global::getCurrentFormats();

    QList<Global::LocaleInfo> locales = Global::getAllEnabledLocales();


    for (int i = 0; i < locales.size(); i++) {
        const Global::LocaleInfo *locale = &locales.at(i);

        TreeWidgetItem *item = new TreeWidgetItem(ui->treeWidget);
        item->setText(0, locale->locale);
        item->setText(1, locale->language);
        item->setText(2, locale->territory);

        if (currentLocale == locale->locale)
            item->localeRadioButton.setChecked(true);
        ui->treeWidget->setItemWidget(item, 3, &item->localeRadioButton);
        groupLocale.addButton(&item->localeRadioButton);

        if (currentFormats == locale->locale)
            item->formatsRadioButton.setChecked(true);
        ui->treeWidget->setItemWidget(item, 4, &item->formatsRadioButton);
        groupFormats.addButton(&item->formatsRadioButton);
    }

    ui->treeWidget->sortItems(0, Qt::AscendingOrder);
}



void PageLanguage::apply_clicked() {
    QString systemLocale;
    QString systemFormats;
    QStringList locales;

    for(int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i) {
        TreeWidgetItem *item = dynamic_cast<TreeWidgetItem*>(ui->treeWidget->topLevelItem(i));
        if (!item)
            continue;

       locales.append(item->text(0));

       if (item->localeRadioButton.isChecked())
           systemLocale = item->text(0);

       if (item->formatsRadioButton.isChecked())
           systemFormats = item->text(0);
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

    /* Modify /etc/locale.conf using systemd-localed */
    QStringList localeList;
    localeList << QString("LANG=%1").arg(systemLocale)
               << QString("LANGUAGE=%1").arg(systemLocale)
               << QString("LC_CTYPE=%1").arg(systemLocale)
               << QString("LC_NUMERIC=%1").arg(systemFormats)
               << QString("LC_TIME=%1").arg(systemFormats)
               << QString("LC_COLLATE=%1").arg(systemLocale)
               << QString("LC_MONETARY=%1").arg(systemFormats)
               << QString("LC_MESSAGES=%1").arg(systemLocale)
               << QString("LC_PAPER=%1").arg(systemFormats)
               << QString("LC_NAME=%1").arg(systemFormats)
               << QString("LC_ADDRESS=%1").arg(systemFormats)
               << QString("LC_TELEPHONE=%1").arg(systemFormats)
               << QString("LC_MEASUREMENT=%1").arg(systemFormats)
               << QString("LC_IDENTIFICATION=%1").arg(systemFormats);

    QDBusInterface dbusInterface("org.freedesktop.locale1",
                                 "/org/freedesktop/locale1",
                                 "org.freedesktop.locale1",
                                 QDBusConnection::systemBus());
    /*
     * asb
     * array_string -> locale
     * boolean -> arg_ask_password
     */
    QDBusMessage reply;
    reply = dbusInterface.call("SetLocale", localeList, true);
    if (reply.type() == QDBusMessage::ErrorMessage) {
        QMessageBox::warning(this, tr("Error!"), QString(tr("Failed to set locale!") + "\n" + reply.errorMessage()), QMessageBox::Ok, QMessageBox::Ok);
    }

    /* Generate new locales */
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


void PageLanguage::buttonRemove_clicked() {
    TreeWidgetItem *item = dynamic_cast<TreeWidgetItem*>(ui->treeWidget->currentItem());

    if (ui->treeWidget->selectedItems().size() <= 0 || ui->treeWidget->topLevelItemCount() <= 1 || !item)
        return;

    // Set check on another item if this one had the check
    if (item->localeRadioButton.isChecked()) {
        TreeWidgetItem *checkItem = dynamic_cast<TreeWidgetItem*>(ui->treeWidget->itemAbove(item));
        if (checkItem) {
            checkItem->localeRadioButton.setChecked(true);
        }
        else {
            checkItem = dynamic_cast<TreeWidgetItem*>(ui->treeWidget->itemBelow(item));
            if (checkItem)
                checkItem->localeRadioButton.setChecked(true);
        }
    }

    // Set check on another item if this one had the check
    if (item->formatsRadioButton.isChecked()) {
        TreeWidgetItem *checkItem = dynamic_cast<TreeWidgetItem*>(ui->treeWidget->itemAbove(item));
        if (checkItem) {
            checkItem->formatsRadioButton.setChecked(true);
        }
        else {
            checkItem = dynamic_cast<TreeWidgetItem*>(ui->treeWidget->itemBelow(item));
            if (checkItem)
                checkItem->formatsRadioButton.setChecked(true);
        }
    }

    delete item;
}



void PageLanguage::buttonRestore_clicked()
{
    activated();
}



void PageLanguage::buttonAdd_clicked()
{
    SelectLocalesDialog dialog(this);
    dialog.exec();

    if (!dialog.localeAdded()) {
        return;
    }

    QString locale = dialog.getLocale();
    // Check if already in list
    for(int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i) {
       if (ui->treeWidget->topLevelItem(i)->text(0) == locale) {
           return;
       }
    }

    TreeWidgetItem *item = new TreeWidgetItem(ui->treeWidget);
    item->setText(0, locale);
    item->setText(1, QLocale::countryToString(QLocale(locale).country()));
    item->setText(2, QLocale::languageToString(QLocale(locale).language()));
    ui->treeWidget->setItemWidget(item, 3, &item->localeRadioButton);
    groupLocale.addButton(&item->localeRadioButton);
    ui->treeWidget->setItemWidget(item, 4, &item->formatsRadioButton);
    groupFormats.addButton(&item->formatsRadioButton);
    ui->treeWidget->sortItems(0, Qt::AscendingOrder);
}
