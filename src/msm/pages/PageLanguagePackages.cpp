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


PageLanguagePackages::PageLanguagePackages( QWidget* parent ) :
    PageWidget( parent ),
    ui( new Ui::PageLanguagePackages )
{
    ui->setupUi( this );
    setTitel( tr( "Language Packages" ) );
    setIcon( QPixmap( ":/images/resources/locale-package.png" ) );
    setShowApplyButton( true );

    ui->treeWidgetAvailable->setColumnWidth( 0, 250 );
    ui->treeWidgetAvailable->setColumnWidth( 1, 250 );
    ui->treeWidgetAvailable->setColumnWidth( 2, 30 );

    ui->treeWidgetInstalled->setColumnWidth( 0, 300 );
    ui->treeWidgetInstalled->setColumnWidth( 1, 300 );

    connect( ui->treeWidgetAvailable, &QTreeWidget::clicked,
             this, &PageLanguagePackages::updateApplyEnabledState );
}


PageLanguagePackages::~PageLanguagePackages()
{
    delete ui;
}


void
PageLanguagePackages::activated()
{
    // Clean up first
    ui->treeWidgetAvailable->clear();
    ui->treeWidgetInstalled->clear();

    QList<Global::LanguagePackage> availablePackages, installedPackages;

    if ( Global::getLanguagePackages( &availablePackages, &installedPackages ) )
    {
        addLanguagePackagesToTreeWidget( ui->treeWidgetAvailable, &availablePackages, true );
        addLanguagePackagesToTreeWidget( ui->treeWidgetInstalled, &installedPackages, false );
    }

    updateApplyEnabledState();
}


void
PageLanguagePackages::apply_clicked()
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
    if ( !Global::isSystemUpToDate() )
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


void
PageLanguagePackages::updateApplyEnabledState()
{
    for ( int i = 0; i < ui->treeWidgetAvailable->topLevelItemCount(); ++i )
    {
        QTreeWidgetItem* topItem = ui->treeWidgetAvailable->topLevelItem( i );

        for ( int x = 0; x < topItem->childCount(); ++x )
        {
            if ( topItem->child( x )->checkState( 2 ) )
            {
                emit setApplyEnabled( this, true );
                return;
            }
        }
    }

    emit setApplyEnabled( this, false );
}


void
PageLanguagePackages::addLanguagePackagesToTreeWidget( QTreeWidget* treeWidget, QList<Global::LanguagePackage>* languagePackages, bool checkable )
{
    QMap<QString, QList<Global::LanguagePackage> > sortedPackagesLocale;

    for ( int i = 0; i < languagePackages->size(); i++ )
    {
        const Global::LanguagePackage* languagePackage = &languagePackages->at( i );
        sortedPackagesLocale[languagePackage->locale].append( *languagePackage );
    }

    QFont font;
    font.setBold( true );
    font.setWeight( 75 );

    QMapIterator<QString, QList<Global::LanguagePackage> > i( sortedPackagesLocale );
    while ( i.hasNext() )
    {
        i.next();
        QTreeWidgetItem* parentItem = new QTreeWidgetItem( treeWidget );
        ui->treeWidgetAvailable->addTopLevelItem( parentItem );
        ui->treeWidgetAvailable->setFirstItemColumnSpanned( parentItem, true );
        parentItem->setText( 0, tr( "%1 language packages" ).arg( i.key() ) );

        for ( int x = 0; x < i.value().size(); x++ )
        {
            const Global::LanguagePackage* languagePackage = &i.value().at( x );

            QTreeWidgetItem* item = new QTreeWidgetItem( parentItem );
            item->setText( 0, languagePackage->languagePackage );
            item->setText( 1, languagePackage->parentPackage );

            if ( checkable )
                item->setCheckState( 2, Qt::Checked );
        }

        parentItem->setFont( 0, font );
        parentItem->setSizeHint( 0, QSize( 0, 24 ) );
        parentItem->setExpanded( true );
        parentItem->setFlags( Qt::ItemIsEnabled );
        parentItem->setIcon( 0, QIcon( ":/images/resources/language.png" ) );
    }
}
