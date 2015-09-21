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

#include "PageLanguagePackages.h"
#include "ui_PageLanguagePackages.h"
#include "../common/ActionDialog.h"

#include <KAboutData>
#include <KAuth>
#include <KAuthAction>

#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QProcess>
#include <QtCore/QDebug>

#include <KPluginFactory>
K_PLUGIN_FACTORY(MsmLanguagePackagesFactory,
                 registerPlugin<PageLanguagePackages>("msm_language_packages");)

PageLanguagePackages::PageLanguagePackages(QWidget *parent, const QVariantList &args) :
    KCModule(parent, args),
    ui(new Ui::PageLanguagePackages)
{
    KAboutData *aboutData = new KAboutData("msm_language_packages",
                                           tr("Keyboard Settings", "@title"),
                                           PROJECT_VERSION,
                                           QStringLiteral(""),
                                           KAboutLicense::LicenseKey::GPL_V3,
                                           "Copyright 2014-2015 Ramon Buldó");

    aboutData->addAuthor("Ramon Buldó",
                         tr("Author", "@info:credit"),
                         QStringLiteral("ramon@manjaro.org"));
    aboutData->addAuthor("Roland Singer",
                         tr("Author", "@info:credit"),
                         QStringLiteral("roland@manjaro.org"));

    setAboutData(aboutData);
    setButtons(KCModule::NoAdditionalButton);

    ui->setupUi(this);

    ui->treeWidgetAvailable->setColumnWidth(0, 250);
    ui->treeWidgetAvailable->setColumnWidth(1, 250);
    ui->treeWidgetAvailable->setColumnWidth(2, 30);

    ui->treeWidgetInstalled->setColumnWidth(0, 300);
    ui->treeWidgetInstalled->setColumnWidth(1, 300);

    connect(ui->treeWidgetAvailable, &QTreeWidget::clicked,
            [=] (const QModelIndex &index)
    {
        if (index.isValid()) {
            emit changed(true);
        }
    });
    connect(ui->installPackagesButton, &QPushButton::clicked,
            this, &PageLanguagePackages::save);
}


PageLanguagePackages::~PageLanguagePackages()
{
    delete ui;
}


void PageLanguagePackages::load()
{
    // Clean up first
    ui->treeWidgetAvailable->clear();
    ui->treeWidgetInstalled->clear();

    QList<Global::LanguagePackage> availablePackages, installedPackages;

    getInstalledPackages();
    getAvailablePackages();
    QList<LanguagePackagesItem> lpiList { getLanguagePackages() };

    if (Global::getLanguagePackages(&availablePackages, &installedPackages, lpiList)) {
        addLanguagePackagesToTreeWidget(ui->treeWidgetAvailable, &availablePackages, true);
        if (availablePackages.size() > 0) {
            ui->installPackagesButton->setEnabled(true);
        } else {
            ui->installPackagesButton->setEnabled(false);
        }
        addLanguagePackagesToTreeWidget(ui->treeWidgetInstalled, &installedPackages, false);
    }
}


void PageLanguagePackages::save()
{
    // Check if system is up-to-date
    if (!isSystemUpToDate()) {
        QMessageBox::warning(this, tr("System is out-of-date"), tr("Your System is not up-to-date! You have to update it first to continue!"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    // Install packages
    QStringList packages;

    for(int i = 0; i < ui->treeWidgetAvailable->topLevelItemCount(); ++i) {
       QTreeWidgetItem *topItem = ui->treeWidgetAvailable->topLevelItem(i);

       for(int x = 0; x < topItem->childCount(); ++x) {
           if (topItem->child(x)->checkState(2))
               packages.append(topItem->child(x)->text(0));
       }
    }

    if (!packages.isEmpty()) {
        QStringList arguments;
        arguments << "--noconfirm" << "--noprogress" << "-S" << packages;
        QVariantMap args;
        args["arguments"] = arguments;
        KAuth::Action installAction(QLatin1String("org.manjaro.msm.languagepackages.install"));
        installAction.setHelperId(QLatin1String("org.manjaro.msm.languagepackages"));
        installAction.setArguments(args);

        ActionDialog actionDialog;
        actionDialog.setInstallAction(installAction);
        actionDialog.setWindowTitle(tr("Install language packages."));
        actionDialog.exec();
    }
    load();
}


void PageLanguagePackages::defaults()
{
    this->load();
}


void PageLanguagePackages::addLanguagePackagesToTreeWidget(QTreeWidget *treeWidget, QList<Global::LanguagePackage> *languagePackages, bool checkable)
{
    QMap<QString, QList<Global::LanguagePackage> > sortedPackagesLocale;

    for (int i = 0; i < languagePackages->size(); i++) {
        const Global::LanguagePackage *languagePackage = &languagePackages->at(i);
        sortedPackagesLocale[languagePackage->locale].append(*languagePackage);
    }

    QFont font;
    font.setBold(true);
    font.setWeight(75);

    QMapIterator<QString, QList<Global::LanguagePackage> > i(sortedPackagesLocale);

    while (i.hasNext()) {
        i.next();
        QTreeWidgetItem *parentItem = new QTreeWidgetItem(treeWidget);
        ui->treeWidgetAvailable->addTopLevelItem(parentItem);
        ui->treeWidgetAvailable->setFirstItemColumnSpanned(parentItem, true);
        parentItem->setText(0, tr("%1 language packages").arg(i.key()));

        for (int x = 0; x < i.value().size(); x++) {
            const Global::LanguagePackage *languagePackage = &i.value().at(x);

            QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
            item->setText(0, languagePackage->languagePackage);
            item->setText(1, languagePackage->parentPackage);

            if (checkable)
                item->setCheckState(2, Qt::Checked);
        }

        parentItem->setFont(0, font);
        parentItem->setSizeHint(0, QSize(0, 24));
        parentItem->setExpanded(true);
        parentItem->setFlags(Qt::ItemIsEnabled);
        parentItem->setIcon(0, QIcon(":/images/resources/language.png"));
    }
}


bool PageLanguagePackages::isSystemUpToDate()
{
    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LANG", "C");
    env.insert("LC_MESSAGES", "C");
    process.setProcessEnvironment(env);
    process.start("pacman", QStringList() << "-Sup");
    if (!process.waitForFinished()) {
        qDebug() << "error: failed to determine if system is up-to-date (pacman)!";
        return false;
    }

    return QString(process.readAll()).split("\n", QString::SkipEmptyParts) ==
               (QStringList() << ":: Starting full system upgrade...");
}


QList<LanguagePackagesItem> PageLanguagePackages::getLanguagePackages()
{
    QFile file;
    file.setFileName(":/language_packages.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!jsonDocument.isObject()) {
        qDebug() << "Cannot read 'language_packages.json' resource";
        return QList<LanguagePackagesItem>();
    }

    QJsonObject jsonObject = jsonDocument.object();
    QJsonValue packagesValue = jsonObject.value(QString("Packages"));
    QList<QVariantMap> packages;
    if (packagesValue.isArray()) {
        for (auto val : packagesValue.toArray()) {
            packages.append(val.toObject().toVariantMap());
        }
    }

    QList<LanguagePackagesItem> lpiList;
    for(QVariantMap package : packages) {
        QString name { package["name"].toString() };
        QString languagePackage { package["l10n_package"].toString() };
        QStringList parentPackages;
        for (auto val : package["parent_packages"].toList()) {
            parentPackages << val.toString();
        }
        QStringList parentPkgInstalled { checkInstalled(parentPackages) };
        QStringList languagePkgInstalled { checkInstalledLanguagePackages(languagePackage) };
        QStringList languagePkgAvailable { checkAvailableLanguagePackages(languagePackage) };
        LanguagePackagesItem lpi {
            name,
            languagePackage,
            parentPackages,
            parentPkgInstalled,
            languagePkgInstalled,
            languagePkgAvailable
        };
        lpiList.append(lpi);
    }
    return lpiList;
}


QStringList PageLanguagePackages::checkInstalled(const QStringList &packages)
{
    QStringList installedPackages;
    for (const QString package : packages) {
        if (m_installedPackages.contains(package))
            installedPackages.append(package);
    }
    return installedPackages;
}


QStringList PageLanguagePackages::checkInstalledLanguagePackages(QString package)
{
    package.remove(QChar('%'));
    QRegularExpression re(QString("^(%1)").arg(package));
    return m_installedPackages.filter(re);
}


QStringList PageLanguagePackages::checkAvailableLanguagePackages(QString package)
{
    package.remove(QChar('%'));
    QRegularExpression re(QString("^(%1)").arg(package));
    return m_availablePackages.filter(re);
}


void PageLanguagePackages::getAvailablePackages()
{
    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LANG", "C");
    env.insert("LC_MESSAGES", "C");
    process.setProcessEnvironment(env);
    process.start("pacman", QStringList() << "-Si");

    if (!process.waitForFinished()) {
        qDebug() << "error: failed to get informations about available packages (pacman)!";
        return;
    }

    QStringList output = QString(process.readAll()).split("\n", QString::SkipEmptyParts);

    m_availablePackages.clear();
    for (QString line : output) {
        line = line.remove(" ").remove("\t");
        if (!line.toLower().startsWith("name:")) {
            continue;
        }
        line = line.mid(line.indexOf(":") + 1);
        m_availablePackages.append(line);
    }
}


void PageLanguagePackages::getInstalledPackages()
{
    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LANG", "C");
    env.insert("LC_MESSAGES", "C");
    process.setProcessEnvironment(env);
    process.start("pacman", QStringList() << "-Qq");
    if (!process.waitForFinished()) {
        qDebug() << "error: failed to get installed packages (pacman)!";
        return;
    }

    if (process.exitCode() != 0) {
        qDebug() << "error: failed to get installed packages (pacman)!";
        return;
    }

    m_installedPackages = QString(process.readAll()).split("\n", QString::SkipEmptyParts);
}

#include "PageLanguagePackages.moc"
