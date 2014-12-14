/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
 *  Ramon Buldo <ramon@manjaro.org>
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

#include "LocaleModule.h"
#include "ui_LocaleModule.h"

#include <KAboutData>

#include <QtDBus/QDBusInterface>
#include <QtWidgets/QMenu>

#include <KPluginFactory>
K_PLUGIN_FACTORY(MsmLocaleFactory,
                 registerPlugin<LocaleModule>("msm_locale");)

LocaleModule::LocaleModule(QWidget *parent, const QVariantList &args) :
    KCModule(parent, args),
    ui(new Ui::LocaleModule),
    enabledLocalesModel_(new EnabledLocalesModel),
    languageListViewDelegate_(new LanguageListViewDelegate)
{
    KAboutData *aboutData = new KAboutData("msm_locale",
                                           i18nc("@title", "Locale Settings"),
                                           PROJECT_VERSION,
                                           QStringLiteral(""),
                                           KAboutLicense::LicenseKey::GPL_V3,
                                           i18nc("@info:credit", "Copyright 2014 Ramon Buldó"));

    aboutData->addAuthor(i18nc("@info:credit", "Ramon Buldó"),
                         i18nc("@info:credit", "Author"),
                         QStringLiteral("ramon@manjaro.org"));
    aboutData->addAuthor(i18nc("@info:credit", "Roland Singer"),
                         i18nc("@info:credit", "Author"),
                         QStringLiteral("roland@manjaro.org"));

    setAboutData(aboutData);

    ui->setupUi(this);

    /* Set Models */
    ui->localeListView->setModel(enabledLocalesModel_);
    ui->addressComboBox->setModel(enabledLocalesModel_);
    ui->collateComboBox->setModel(enabledLocalesModel_);
    ui->ctypeComboBox->setModel(enabledLocalesModel_);
    ui->identificationComboBox->setModel(enabledLocalesModel_);
    ui->langComboBox->setModel(enabledLocalesModel_);
    ui->measurementComboBox->setModel(enabledLocalesModel_);
    ui->messagesComboBox->setModel(enabledLocalesModel_);
    ui->monetaryComboBox->setModel(enabledLocalesModel_);
    ui->nameComboBox->setModel(enabledLocalesModel_);
    ui->numericComboBox->setModel(enabledLocalesModel_);
    ui->paperComboBox->setModel(enabledLocalesModel_);
    ui->telephoneComboBox->setModel(enabledLocalesModel_);
    ui->timeComboBox->setModel(enabledLocalesModel_);

    /* Set localeListView delegate */
    ui->localeListView->setItemDelegate(languageListViewDelegate_);

    /* Define QActions */
    setRegionAndFormatsAction_ = new QAction(tr("Set as default display language and format"), ui->localeListView);
    setRegionAction_ = new QAction(tr("Set as default display language"), ui->localeListView);
    setFormatsAction_ = new QAction(tr("Set as default format"), ui->localeListView);

    /* Connect top buttons signal/slot */
    connect(ui->buttonRemove, &QPushButton::clicked,
            this, &LocaleModule::removeLocale);
    connect(ui->buttonRestore, &QPushButton::clicked,
            this, &LocaleModule::defaults);
    connect(ui->buttonAdd, &QPushButton::clicked,
            this, &LocaleModule::addLocale);

    /* Connect "System Locales" tab signal/slots */
    connect(ui->localeListView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &LocaleModule::disableRemoveButton);
    connect(ui->localeListView, &QListView::doubleClicked,
            [=] (const QModelIndex &index)
            {
                enabledLocalesModel_->setLang(index);
                enabledLocalesModel_->setLanguage(index);
                enabledLocalesModel_->setCtype(index);
                enabledLocalesModel_->setCollate(index);
                enabledLocalesModel_->setMessages(index);
                enabledLocalesModel_->setAddress(index);
                enabledLocalesModel_->setIdentification(index);
                enabledLocalesModel_->setMeasurement(index);
                enabledLocalesModel_->setMonetary(index);
                enabledLocalesModel_->setName(index);
                enabledLocalesModel_->setNumeric(index);
                enabledLocalesModel_->setPaper(index);
                enabledLocalesModel_->setTelephone(index);
                enabledLocalesModel_->setTime(index);
            });

    /* Connect "Detailed Settings" tab signal/slots */
    connect(ui->addressComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=] (int row)
            {
                QModelIndex index = enabledLocalesModel_->index(row, 0);
                enabledLocalesModel_->setAddress(index);
            });
    connect(ui->collateComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=] (int row)
            {
                QModelIndex index = enabledLocalesModel_->index(row, 0);
                enabledLocalesModel_->setCollate(index);
            });
    connect(ui->ctypeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=] (int row)
            {
                QModelIndex index = enabledLocalesModel_->index(row, 0);
                enabledLocalesModel_->setCtype(index);
            });
    connect(ui->identificationComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=] (int row)
            {
                QModelIndex index = enabledLocalesModel_->index(row, 0);
                enabledLocalesModel_->setIdentification(index);
            });
    connect(ui->langComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=] (int row)
            {
                QModelIndex index = enabledLocalesModel_->index(row, 0);
                enabledLocalesModel_->setLang(index);
            });
    connect(ui->measurementComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=] (int row)
            {
                QModelIndex index = enabledLocalesModel_->index(row, 0);
                enabledLocalesModel_->setMeasurement(index);
            });
    connect(ui->messagesComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=] (int row)
            {
                QModelIndex index = enabledLocalesModel_->index(row, 0);
                enabledLocalesModel_->setMessages(index);
            });
    connect(ui->monetaryComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=] (int row)
            {
                QModelIndex index = enabledLocalesModel_->index(row, 0);
                enabledLocalesModel_->setMonetary(index);
            });
    connect(ui->nameComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=] (int row)
            {
                QModelIndex index = enabledLocalesModel_->index(row, 0);
                enabledLocalesModel_->setName(index);
            });
    connect(ui->numericComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=] (int row)
            {
                QModelIndex index = enabledLocalesModel_->index(row, 0);
                enabledLocalesModel_->setNumeric(index);
            });
    connect(ui->paperComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=] (int row)
            {
                QModelIndex index = enabledLocalesModel_->index(row, 0);
                enabledLocalesModel_->setPaper(index);
            });
    connect(ui->telephoneComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=] (int row)
            {
                QModelIndex index = enabledLocalesModel_->index(row, 0);
                enabledLocalesModel_->setTelephone(index);
            });
    connect(ui->timeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=] (int row)
            {
                QModelIndex index = enabledLocalesModel_->index(row, 0);
                enabledLocalesModel_->setTime(index);
            });

    connect(enabledLocalesModel_, &QAbstractListModel::dataChanged,
            [=] (const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
    {
        Q_UNUSED(bottomRight);
        if (roles.contains(EnabledLocalesModel::AddressRole)) {
            ui->addressComboBox->setCurrentIndex(topLeft.row());
            isSystemLocalesModified_ = true;
        } else if (roles.contains(EnabledLocalesModel::CollateRole)) {
            ui->collateComboBox->setCurrentIndex(topLeft.row());
            isSystemLocalesModified_ = true;
        } else if (roles.contains(EnabledLocalesModel::CtypeRole)) {
            ui->ctypeComboBox->setCurrentIndex(topLeft.row());
            isSystemLocalesModified_ = true;
        } else if (roles.contains(EnabledLocalesModel::IdentificationRole)) {
            ui->identificationComboBox->setCurrentIndex(topLeft.row());
            isSystemLocalesModified_ = true;
        } else if (roles.contains(EnabledLocalesModel::LangRole)) {
            ui->langComboBox->setCurrentIndex(topLeft.row());
            isSystemLocalesModified_ = true;
        } else if (roles.contains(EnabledLocalesModel::MeasurementRole)) {
            ui->measurementComboBox->setCurrentIndex(topLeft.row());
            isSystemLocalesModified_ = true;
        } else if (roles.contains(EnabledLocalesModel::MonetaryRole)) {
            ui->monetaryComboBox->setCurrentIndex(topLeft.row());
            isSystemLocalesModified_ = true;
        } else if (roles.contains(EnabledLocalesModel::MessagesRole)) {
            ui->messagesComboBox->setCurrentIndex(topLeft.row());
            isSystemLocalesModified_ = true;
        } else if (roles.contains(EnabledLocalesModel::NameRole)) {
            ui->nameComboBox->setCurrentIndex(topLeft.row());
            isSystemLocalesModified_ = true;
        } else if (roles.contains(EnabledLocalesModel::NumericRole)) {
            ui->numericComboBox->setCurrentIndex(topLeft.row());
            isSystemLocalesModified_ = true;
        } else if (roles.contains(EnabledLocalesModel::PaperRole)) {
            ui->paperComboBox->setCurrentIndex(topLeft.row());
            isSystemLocalesModified_ = true;
        } else if (roles.contains(EnabledLocalesModel::TelephoneRole)) {
            ui->telephoneComboBox->setCurrentIndex(topLeft.row());
            isSystemLocalesModified_ = true;
        } else if (roles.contains(EnabledLocalesModel::TimeRole)) {
            ui->timeComboBox->setCurrentIndex(topLeft.row());
            isSystemLocalesModified_ = true;
        }
    });

    /* Context menu for the list view in "System Locales" tab */
    connect(ui->localeListView, &QListView::customContextMenuRequested,
            [=] (const QPoint &pos)
    {
        QMenu localeListViewMenu;
        localeListViewMenu.addAction(setRegionAndFormatsAction_);
        localeListViewMenu.addAction(setRegionAction_);
        localeListViewMenu.addAction(setFormatsAction_);
        localeListViewMenu.exec(ui->localeListView->mapToGlobal(pos));
    });

    connect(setRegionAndFormatsAction_, &QAction::triggered,
            [=]
    {
        QModelIndex index = ui->localeListView->currentIndex();
        enabledLocalesModel_->setLang(index);
        enabledLocalesModel_->setLanguage(index);
        enabledLocalesModel_->setCtype(index);
        enabledLocalesModel_->setCollate(index);
        enabledLocalesModel_->setMessages(index);
        enabledLocalesModel_->setAddress(index);
        enabledLocalesModel_->setIdentification(index);
        enabledLocalesModel_->setMeasurement(index);
        enabledLocalesModel_->setMonetary(index);
        enabledLocalesModel_->setName(index);
        enabledLocalesModel_->setNumeric(index);
        enabledLocalesModel_->setPaper(index);
        enabledLocalesModel_->setTelephone(index);
        enabledLocalesModel_->setTime(index);
    });

    connect(setRegionAction_, &QAction::triggered,
            [=]
    {
        QModelIndex index = ui->localeListView->currentIndex();
        enabledLocalesModel_->setLang(index);
        enabledLocalesModel_->setLanguage(index);
        enabledLocalesModel_->setCtype(index);
        enabledLocalesModel_->setCollate(index);
        enabledLocalesModel_->setMessages(index);
    });

    connect(setFormatsAction_, &QAction::triggered,
            [=]
    {
        QModelIndex index = ui->localeListView->currentIndex();
        enabledLocalesModel_->setAddress(index);
        enabledLocalesModel_->setIdentification(index);
        enabledLocalesModel_->setMeasurement(index);
        enabledLocalesModel_->setMonetary(index);
        enabledLocalesModel_->setName(index);
        enabledLocalesModel_->setNumeric(index);
        enabledLocalesModel_->setPaper(index);
        enabledLocalesModel_->setTelephone(index);
        enabledLocalesModel_->setTime(index);
    });
}


LocaleModule::~LocaleModule()
{
    delete ui;
    delete enabledLocalesModel_;
    delete languageListViewDelegate_;
    delete setRegionAndFormatsAction_;
    delete setRegionAction_;
    delete setFormatsAction_;
}


void LocaleModule::load()
{
    ui->buttonRemove->setDisabled(true);
    enabledLocalesModel_->init();
    enabledLocalesModel_->updateSystemLocales();
    isLocaleListModified_ = false;
    isSystemLocalesModified_ = false;
}


void LocaleModule::save()
{
    if (isLocaleListModified_) {
        updateLocaleGen();
    }
    if (isSystemLocalesModified_) {
        setSystemLocale();
    }
}

void LocaleModule::defaults()
{
    this->load();
}


/*
 * Update /etc/locale.gen file and run locale-gen
 * Return true if successful
 */
bool LocaleModule::updateLocaleGen()
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
bool LocaleModule::setSystemLocale()
{
    QStringList localeList;
    if (!enabledLocalesModel_->lang().isEmpty()) {
        localeList << QString("LANG=%1").arg(enabledLocalesModel_->lang());
    }
    if (!enabledLocalesModel_->language().isEmpty()) {
        localeList << QString("LANGUAGE=%1").arg(enabledLocalesModel_->language());
    }
    if (!enabledLocalesModel_->ctype().isEmpty()) {
        localeList << QString("LC_CTYPE=%1").arg(enabledLocalesModel_->ctype());
    }
    if (!enabledLocalesModel_->numeric().isEmpty()) {
        localeList << QString("LC_NUMERIC=%1").arg(enabledLocalesModel_->numeric());
    }
    if (!enabledLocalesModel_->time().isEmpty()) {
        localeList << QString("LC_TIME=%1").arg(enabledLocalesModel_->time());
    }
    if (!enabledLocalesModel_->collate().isEmpty()) {
        localeList << QString("LC_COLLATE=%1").arg(enabledLocalesModel_->collate());
    }
    if (!enabledLocalesModel_->monetary().isEmpty()) {
        localeList << QString("LC_MONETARY=%1").arg(enabledLocalesModel_->monetary());
    }
    if (!enabledLocalesModel_->messages().isEmpty()) {
        localeList << QString("LC_MESSAGES=%1").arg(enabledLocalesModel_->messages());
    }
    if (!enabledLocalesModel_->paper().isEmpty()) {
        localeList << QString("LC_PAPER=%1").arg(enabledLocalesModel_->paper());
    }
    if (!enabledLocalesModel_->name().isEmpty()) {
        localeList << QString("LC_NAME=%1").arg(enabledLocalesModel_->name());
    }
    if (!enabledLocalesModel_->address().isEmpty()) {
        localeList << QString("LC_ADDRESS=%1").arg(enabledLocalesModel_->address());
    }
    if (!enabledLocalesModel_->telephone().isEmpty()) {
        localeList << QString("LC_TELEPHONE=%1").arg(enabledLocalesModel_->telephone());
    }
    if (!enabledLocalesModel_->measurement().isEmpty()) {
        localeList << QString("LC_MEASUREMENT=%1").arg(enabledLocalesModel_->measurement());
    }
    if (!enabledLocalesModel_->identification().isEmpty()) {
        localeList << QString("LC_IDENTIFICATION=%1").arg(enabledLocalesModel_->identification());
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


void LocaleModule::addLocale()
{
    SelectLocalesDialog dialog(this);
    dialog.exec();

    if (!dialog.localeAdded()) {
        return;
    }

    QString localeCode = dialog.getLocale();
    if (enabledLocalesModel_->insertLocale(enabledLocalesModel_->rowCount(QModelIndex()), 1, localeCode)) {
        isLocaleListModified_ = true;
    }
}


void LocaleModule::removeLocale() {
    QModelIndex localeCurrentIndex = ui->localeListView->currentIndex();
    if (localeCurrentIndex.isValid()) {
        if (enabledLocalesModel_->removeLocale(localeCurrentIndex.row(), 1)) {
            ui->localeListView->setCurrentIndex(QModelIndex());
            isLocaleListModified_ = true;
        }
    }
}


/*
 * Disables the remove button if only one locale is in the list or no locale is selected
 */
void LocaleModule::disableRemoveButton(const QModelIndex &current, const QModelIndex &previous)
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

#include "LocaleModule.moc"

