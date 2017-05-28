/*
 *  This file is part of Manjaro Settings Manager.
 *
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
#include <limits>
#include "ActionDialog.h"
#include "LanguageCommon.h"
#include "LanguagePackages.h"
#include "LanguagePackagesCommon.h"
#include "LanguagePackagesItem.h"
#include "PacmanUtils.h"

#include <QDebug>
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTreeWidgetItem>


QString
LanguagePackagesCommon::getName()
{
    return QString( "msm_language_packages" );
}


QString
LanguagePackagesCommon::getTitle()
{
    return QString( tr ( "Language Packages" ) );
}


QString
LanguagePackagesCommon::getDescription()
{
    return QString( tr ( "Detection and installation of language packages" ) );
}


void
LanguagePackagesCommon::load( Ui::PageLanguagePackages* ui )
{
    ui->treeWidgetAvailable->clear();
    ui->treeWidgetInstalled->clear();
    ui->installPackagesButton->setEnabled( false );

    LanguagePackages languagePackages;
    QList<LanguagePackagesItem> lpiList { languagePackages.languagePackages() };

    // Global language packages
    QTreeWidgetItem* parentItemAvailable = newParentTreeWidgetItem( ui, ui->treeWidgetAvailable );
    QTreeWidgetItem* parentItemInstalled = newParentTreeWidgetItem( ui, ui->treeWidgetInstalled );
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

        QTreeWidgetItem* parentItemAvailable = newParentTreeWidgetItem( ui, ui->treeWidgetAvailable );
        parentItemAvailable->setText( 0, tr( "%1 language packages" ).arg( locale ) );
        QTreeWidgetItem* parentItemInstalled = newParentTreeWidgetItem( ui, ui->treeWidgetInstalled );
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


QTreeWidgetItem*
LanguagePackagesCommon::newParentTreeWidgetItem( Ui::PageLanguagePackages* ui, QTreeWidget* parent )
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


void
LanguagePackagesCommon::installPackages( Ui::PageLanguagePackages* ui  )
{
    // Check if system is up-to-date
    if ( !PacmanUtils::isSystemUpToDate() )
    {
        QMessageBox::warning( ui->treeWidgetAvailable,
                              tr( "System is out-of-date" ),
                              tr( "Your System is not up-to-date! You have to update it first to continue!" ),
                              QMessageBox::Ok, QMessageBox::Ok );
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
    {
        QStringList arguments;
        arguments << "--noconfirm" << "--noprogressbar" << "-S" << packages;
        QVariantMap args;
        args["arguments"] = arguments;
        KAuth::Action installAction( QLatin1String( "org.manjaro.msm.languagepackages.install" ) );
        installAction.setHelperId( QLatin1String( "org.manjaro.msm.languagepackages" ) );
        installAction.setArguments( args );
        installAction.setTimeout( std::numeric_limits<int>::max() );

        ActionDialog actionDialog;
        actionDialog.setInstallAction( installAction );
        actionDialog.setWindowTitle( tr( "Install language packages." ) );
        actionDialog.writeToTerminal( QString( tr( "Waiting for user input..." ) ) );
        actionDialog.exec();
    }
}
