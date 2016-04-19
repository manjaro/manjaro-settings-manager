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

#include "PageLanguagePackages.h"
#include "ui_PageLanguagePackages.h"
#include "LanguageCommon.h"

#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QProcess>
#include <QtCore/QDebug>
#include <QtCore/QSettings>

PageLanguagePackages::PageLanguagePackages( QWidget* parent ) :
    PageWidget( parent ),
    ui( new Ui::PageLanguagePackages )
{
    ui->setupUi( this );
    setTitle( tr( "Language Packages" ) );
    setIcon( QPixmap( ":/images/resources/locale-package.png" ) );
    setName( "msm_language_packages" );

    ui->treeWidgetAvailable->setColumnWidth( 0, 250 );
    ui->treeWidgetAvailable->setColumnWidth( 1, 300 );
    ui->treeWidgetAvailable->setColumnWidth( 2, 30 );

    ui->treeWidgetInstalled->setColumnWidth( 0, 300 );
    ui->treeWidgetInstalled->setColumnWidth( 1, 300 );

    connect( ui->installPackagesButton, &QPushButton::clicked,
             this, &PageLanguagePackages::save );

    connect( ui->checkLanguagePackage, &QCheckBox::stateChanged,
             [=] ()
    {
        QSettings settings( "manjaro", "manjaro-settings-manager" );
        settings.setValue( "notifications/checkLanguagePackages",
                           ui->checkLanguagePackage->isChecked() );
        settings.sync();
    } );
}


PageLanguagePackages::~PageLanguagePackages()
{
    delete ui;
}


void
PageLanguagePackages::load()
{
    loadLanguagePackages();
    loadNotificationsSettings();
}


void
PageLanguagePackages::loadLanguagePackages()
{
    ui->treeWidgetAvailable->clear();
    ui->treeWidgetInstalled->clear();
    ui->installPackagesButton->setEnabled( false );

    LanguagePackages languagePackages;
    QList<LanguagePackagesItem> lpiList { languagePackages.languagePackages() };

    // Global language packages
    QTreeWidgetItem* parentItemAvailable = newParentTreeWidgetItem( ui->treeWidgetAvailable );
    QTreeWidgetItem* parentItemInstalled = newParentTreeWidgetItem( ui->treeWidgetInstalled );
    foreach ( const auto item, lpiList )
    {
        if ( item.parentPkgInstalled().length() == 0 )
            continue;
        if ( item.languagePackage().contains( "%" ) )
            continue;

        if ( item.languagePkgInstalled().contains( item.languagePackage() ) )
        {
            QTreeWidgetItem* widgetItem = new QTreeWidgetItem( parentItemInstalled );
            widgetItem->setText( 0, item.languagePackage() );
            widgetItem->setText( 1, item.name() );
            // Don't list package in installed tree
            continue;
        }
        if ( item.languagePkgAvailable().contains( item.languagePackage() ) )
        {
            QTreeWidgetItem* widgetItem = new QTreeWidgetItem( parentItemAvailable );
            widgetItem->setText( 0, item.languagePackage() );
            widgetItem->setText( 1, item.name() );
            widgetItem->setCheckState( 2, Qt::Checked );
            if ( !ui->installPackagesButton->isEnabled() )
                ui->installPackagesButton->setEnabled( true );
        }
    }

    // Split language packages
    QStringList locales { LanguageCommon::enabledLocales( true ) };
    qSort( locales );
    foreach ( const QString locale, locales )
    {
        QStringList split = locale.split( "_", QString::SkipEmptyParts );
        if ( split.size() != 2 )
            continue;
        QByteArray language = QString( split.at( 0 ) ).toUtf8();
        QByteArray territory = QString( split.at( 1 ) ).toUtf8();

        QTreeWidgetItem* parentItemAvailable = newParentTreeWidgetItem( ui->treeWidgetAvailable );
        parentItemAvailable->setText( 0, tr( "%1 language packages" ).arg( locale ) );
        QTreeWidgetItem* parentItemInstalled = newParentTreeWidgetItem( ui->treeWidgetInstalled );
        parentItemInstalled->setText( 0, tr( "%1 language packages" ).arg( locale ) );
        foreach ( const auto item, lpiList )
        {
            if ( item.parentPkgInstalled().length() == 0 )
                continue;
            if ( !item.languagePackage().contains( "%" ) )
                continue;

            QList<QByteArray> checkPkgs;
            // Example: firefox-i18n-% -> firefox-i18n-en-US
            checkPkgs << item.languagePackage().replace( "%", language.toLower() + "-" + territory );
            // Example: firefox-i18n-% -> firefox-i18n-en-us
            checkPkgs << item.languagePackage().replace( "%", language.toLower() + "-" + territory.toLower() );
            // Example: firefox-i18n-% -> firefox-i18n-en_US
            checkPkgs << item.languagePackage().replace( "%", language.toLower() + "_" + territory );
            // Example: firefox-i18n-% -> firefox-i18n-en_us
            checkPkgs << item.languagePackage().replace( "%", language.toLower() + "_" + territory.toLower() );
            // Example: firefox-i18n-% -> firefox-i18n-en
            checkPkgs << item.languagePackage().replace( "%", language.toLower() );

            foreach ( const auto checkPkg, checkPkgs )
            {
                if ( item.languagePkgInstalled().contains( checkPkg ) )
                {
                    QTreeWidgetItem* widgetItem = new QTreeWidgetItem( parentItemInstalled );
                    widgetItem->setText( 0, checkPkg );
                    widgetItem->setText( 1, item.name() );
                    // Don't list package in installed tree
                    continue;
                }
                if ( item.languagePkgAvailable().contains( checkPkg ) )
                {
                    QTreeWidgetItem* widgetItem = new QTreeWidgetItem( parentItemAvailable );
                    widgetItem->setText( 0, checkPkg );
                    widgetItem->setText( 1, item.name() );
                    widgetItem->setCheckState( 2, Qt::Checked );
                    if ( !ui->installPackagesButton->isEnabled() )
                        ui->installPackagesButton->setEnabled( true );
                }
            }
        }
    }
}


void
PageLanguagePackages::loadNotificationsSettings()
{
    QSettings settings( "manjaro", "manjaro-settings-manager" );
    bool checkLanguagePackage = settings.value( "notifications/checkLanguagePackages", true ).toBool();
    ui->checkLanguagePackage->setChecked( checkLanguagePackage );
}


void
PageLanguagePackages::save()
{
    ApplyDialog dialog( this );

    // Update pacman databases first
    dialog.exec( "pacman", QStringList() << "--noconfirm" << "--noprogressbar" << "-Sy", tr( "Updating pacman databases..." ), true );

    if ( !dialog.processSuccess() )
    {
        emit closePage( this );
        return;
    }

    // Check if system is up-to-date
    if ( !LanguageCommon::isSystemUpToDate() )
    {
        QMessageBox::warning( this, tr( "System is out-of-date" ), tr( "Your System is not up-to-date! You have to update it first to continue!" ), QMessageBox::Ok, QMessageBox::Ok );
        emit closePage( this );
        return;
    }

    // Install packages
    QStringList packages;

    for ( int i = 0; i < ui->treeWidgetAvailable->topLevelItemCount(); ++i )
    {
        QTreeWidgetItem* topItem = ui->treeWidgetAvailable->topLevelItem( i );

        for ( int x = 0; x < topItem->childCount(); ++x )
        {
            if ( topItem->child( x )->checkState( 2 ) )
                packages.append( topItem->child( x )->text( 0 ) );
        }
    }

    if ( !packages.isEmpty() )
        dialog.exec( "pacman", QStringList() << "--noconfirm" << "--noprogressbar" << "-S" << packages, tr( "Installing language packages..." ), false );

    emit closePage( this );
}


QTreeWidgetItem*
PageLanguagePackages::newParentTreeWidgetItem( QTreeWidget* parent )
{
    QTreeWidgetItem* item = new QTreeWidgetItem( parent );
    ui->treeWidgetAvailable->addTopLevelItem( item );
    ui->treeWidgetAvailable->setFirstItemColumnSpanned( item, true );
    item->setText( 0, tr( "Global language packages" ) );

    QFont font;
    font.setBold( true );
    font.setWeight( 75 );
    item->setFont( 0, font );

    item->setSizeHint( 0, QSize( 0, 24 ) );
    item->setExpanded( true );
    item->setFlags( Qt::ItemIsEnabled );
    item->setIcon( 0, QIcon( ":/images/resources/language.png" ) );
    return item;
}
