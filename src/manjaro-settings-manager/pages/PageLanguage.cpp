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
    ui(new Ui::PageLanguage),
    enabledLocalesModel_(new EnabledLocalesModel),
    languageListViewDelegate_(new LanguageListViewDelegate)
{
    ui->setupUi(this);
    setTitel(tr("Language"));
    setIcon(QPixmap(":/images/resources/locale.png"));
    setShowApplyButton(true);

    ui->localeListView->setModel(enabledLocalesModel_);
    ui->localeListView->setItemDelegate(languageListViewDelegate_);

    connect(ui->buttonRemove, &QPushButton::clicked,
            this, &PageLanguage::removeLocale);
    connect(ui->buttonRestore, &QPushButton::clicked,
            this, &PageLanguage::restoreLocaleList);
    connect(ui->buttonAdd, &QPushButton::clicked,
            this, &PageLanguage::addLocale);
    connect(ui->localeListView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &PageLanguage::disableRemoveButton);
    connect(ui->localeListView, &QListView::doubleClicked,
            [=] (const QModelIndex &index)
            {
                enabledLocalesModel_->setLang(index);
            });
}


PageLanguage::~PageLanguage()
{
    delete ui;
}


void PageLanguage::activated()
{
    ui->buttonRemove->setDisabled(true);
    enabledLocalesModel_->init();
    enabledLocalesModel_->updateSystemLocales();
}


void PageLanguage::apply_clicked()
{
    /* TODO only update locale-gen or setSystemLocale if needed */
    updateLocaleGen();
    setSystemLocale();
}


/*
 * Update /etc/locale.gen file and run locale-gen
 * Return true if successful
 */
bool PageLanguage::updateLocaleGen()
{
    const QString localeGen = "/etc/locale.gen";

    QStringList locales = enabledLocalesModel_->locales();

    QFile file(localeGen);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this,
                             tr("Error!"),
                             tr("Failed to open file '%1'!").arg(localeGen),
                             QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }

    QStringList content;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        content.append(line);
        line = line.trimmed();

        bool found = false;

        for (QString locale : locales) {
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

    // Add missing locales in the file
    for (QString locale : locales) {
        QString str = Global::localeToValidLocaleGenString(locale);

        if (str.isEmpty()) {
            QMessageBox::warning(this,
                                 tr("Error!"),
                                 tr("Failed to obtain valid locale string for locale '%1'!").arg(locale),
                                 QMessageBox::Ok, QMessageBox::Ok);
            continue;
        }

        content.append(str);
    }


    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this,
                             tr("Error!"),
                             tr("Failed to open file '%1'!").arg(localeGen),
                             QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }

    QTextStream out(&file);
    out << content.join("\n");
    file.close();


    /* Generate new locales */
    ApplyDialog dialog(this);
    dialog.exec("locale-gen", QStringList(), tr("Generating locale.gen file..."), false);

    if (dialog.processSuccess()) {
        QMessageBox::information(this, tr("Hint"), tr("You might have to restart the graphical environment to apply the new settings..."), QMessageBox::Ok, QMessageBox::Ok);
    }
    return true;
}


/*
 * Modify /etc/locale.conf using systemd-localed
 */
bool PageLanguage::setSystemLocale()
{
    QStringList localeList;
    if (!enabledLocalesModel_->systemLocales.lang.isEmpty()) {
        localeList << QString("LANG=%1").arg(enabledLocalesModel_->systemLocales.lang);
    }
    if (!enabledLocalesModel_->systemLocales.language.isEmpty()) {
        localeList << QString("LANGUAGE=%1").arg(enabledLocalesModel_->systemLocales.language);
    }
    if (!enabledLocalesModel_->systemLocales.ctype.isEmpty()) {
        localeList << QString("LC_CTYPE=%1").arg(enabledLocalesModel_->systemLocales.ctype);
    }
    if (!enabledLocalesModel_->systemLocales.numeric.isEmpty()) {
        localeList << QString("LC_NUMERIC=%1").arg(enabledLocalesModel_->systemLocales.numeric);
    }
    if (!enabledLocalesModel_->systemLocales.time.isEmpty()) {
        localeList << QString("LC_TIME=%1").arg(enabledLocalesModel_->systemLocales.time);
    }
    if (!enabledLocalesModel_->systemLocales.collate.isEmpty()) {
        localeList << QString("LC_COLLATE=%1").arg(enabledLocalesModel_->systemLocales.collate);
    }
    if (!enabledLocalesModel_->systemLocales.monetary.isEmpty()) {
        localeList << QString("LC_MONETARY=%1").arg(enabledLocalesModel_->systemLocales.monetary);
    }
    if (!enabledLocalesModel_->systemLocales.messages.isEmpty()) {
        localeList << QString("LC_MESSAGES=%1").arg(enabledLocalesModel_->systemLocales.messages);
    }
    if (!enabledLocalesModel_->systemLocales.paper.isEmpty()) {
        localeList << QString("LC_PAPER=%1").arg(enabledLocalesModel_->systemLocales.paper);
    }
    if (!enabledLocalesModel_->systemLocales.name.isEmpty()) {
        localeList << QString("LC_NAME=%1").arg(enabledLocalesModel_->systemLocales.name);
    }
    if (!enabledLocalesModel_->systemLocales.address.isEmpty()) {
        localeList << QString("LC_ADDRESS=%1").arg(enabledLocalesModel_->systemLocales.address);
    }
    if (!enabledLocalesModel_->systemLocales.telephone.isEmpty()) {
        localeList << QString("LC_TELEPHONE=%1").arg(enabledLocalesModel_->systemLocales.telephone);
    }
    if (!enabledLocalesModel_->systemLocales.measurement.isEmpty()) {
        localeList << QString("LC_MEASUREMENT=%1").arg(enabledLocalesModel_->systemLocales.measurement);
    }
    if (!enabledLocalesModel_->systemLocales.identification.isEmpty()) {
        localeList << QString("LC_IDENTIFICATION=%1").arg(enabledLocalesModel_->systemLocales.identification);
    }

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
        QMessageBox::warning(this,
                             tr("Error!"),
                             QString(tr("Failed to set locale!") + "\n" + reply.errorMessage()),
                             QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }
    return true;
}


void PageLanguage::addLocale()
{
    SelectLocalesDialog dialog(this);
    dialog.exec();

    if (!dialog.localeAdded()) {
        return;
    }

    QString locale = dialog.getLocale();
    LocaleItem localeItem(locale);
    if (!enabledLocalesModel_->contains(localeItem)) {
        enabledLocalesModel_->insertLocale(enabledLocalesModel_->rowCount(QModelIndex()), 1, localeItem);
    }
}


void PageLanguage::removeLocale() {
    QModelIndex localeCurrentIndex = ui->localeListView->currentIndex();
    if (localeCurrentIndex.isValid()) {
        if (enabledLocalesModel_->removeLocale(localeCurrentIndex.row(), 1)) {
            ui->localeListView->setCurrentIndex(QModelIndex());
        }
    }
}


void PageLanguage::restoreLocaleList()
{
    activated();
}


/*
 * Disables the remove button if only one locale is in the list or no locale is selected
 */
void PageLanguage::disableRemoveButton(const QModelIndex &current, const QModelIndex &previous)
{
    if (enabledLocalesModel_->rowCount(QModelIndex()) == 1) {
        ui->buttonRemove->setDisabled(true);
    } else {
        if (current.isValid() && !previous.isValid()) {
        ui->buttonRemove->setDisabled(false);
        } else if (!current.isValid() && previous.isValid()) {
            ui->buttonRemove->setDisabled(true);
        }
    }
}




