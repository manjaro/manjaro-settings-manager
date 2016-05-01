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

#include "MsmWindow.h"
#include "ui_MsmWindow.h"

#include <QtCore/QPropertyAnimation>
#include <QtCore/QSettings>
#include <QtWidgets/QGraphicsOpacityEffect>
#include <QTranslator>

MsmWindow::MsmWindow( QWidget* parent ) :
    QMainWindow( parent ),
    ui( new Ui::MsmWindow )
{
    ui->setupUi( this );

    /* Center the window */
    move( qApp->desktop()->availableGeometry().center() - rect().center() );

    readPositionSettings();

    // Trigger method to setup titels and icons
    buttonShowAllSettings_clicked();

    ui->listWidget->addSeparator( tr( "System" ) );
    addPageWidget( pageLanguage );
    addPageWidget( pageLanguagePackages );
    addPageWidget( pageKernel );
    addPageWidget( pageUsers );
    addPageWidget( pageTimeDate );
    ui->listWidget->addSeparator( tr( "Hardware" ) );
    addPageWidget( pageKeyboard );
    addPageWidget( pageMhwd );

    // Connect signals and slots
    connect( ui->buttonQuit, &QPushButton::clicked,
             qApp, &qApp->closeAllWindows );
    connect( ui->listWidget, &ListWidget::itemClicked,
             this, &MsmWindow::listWidget_itemClicked );
    connect( ui->buttonAllSettings, &QPushButton::clicked,
             this, &MsmWindow::buttonShowAllSettings_clicked );
    connect( ui->buttonApply, &QPushButton::clicked,
             this, &MsmWindow::buttonApply_clicked );

    ui->buttonAllSettings->setIcon(QIcon::fromTheme("draw-arrow-back", QIcon(":/images/resources/back.png") ) );
    ui->buttonApply->setIcon(QIcon::fromTheme("dialog-ok-apply", QIcon(":/images/resources/apply.png") ) );
    ui->buttonQuit->setIcon(QIcon::fromTheme("gtk-quit", QIcon(":/images/resources/quit.png") ) );
}



MsmWindow::~MsmWindow()
{
    delete ui;
}


void
MsmWindow::loadModule( QString moduleName )
{

    for ( int i = 0; i < ui->listWidget->count(); i++ )
    {
        ListWidgetItem* item = dynamic_cast<ListWidgetItem*>( ui->listWidget->item( i ) );
        if ( !item || !item->page )
            continue;
        // TODO SEARCH FOR CORRECT MODULENAME
        if ( item->page->getName() == moduleName )
        {
            listWidget_itemClicked( item );
            break;
        }
    }

}


void
MsmWindow::addPageWidget( PageWidget& page )
{
    // Add list widget item
    ListWidgetItem* item = new ListWidgetItem( ui->listWidget );
    item->setText( page.getTitle() );
    item->setIcon( QIcon( page.getIcon() ) );
    item->setSizeHint( QSize( 135, 100 ) );
    item->page = &page;

    // Add to stacked widget
    ui->stackedWidget->addWidget( &page );

    connect( &page, &PageWidget::setApplyEnabled,
             this, &MsmWindow::setApplyEnabled );
    connect( &page, &PageWidget::closePage,
             this, &MsmWindow::closePageRequested );
}


void
MsmWindow::listWidget_itemClicked( QListWidgetItem* current )
{
    ListWidgetItem* item = dynamic_cast<ListWidgetItem*>( current );
    if ( !item || !item->page )
        return;

    // Show page and buttons
    ui->stackedWidget->setCurrentWidget( item->page );
    ui->buttonAllSettings->setVisible( true );
    ui->buttonApply->setEnabled( false );
    ui->buttonApply->setVisible( item->page->getShowApplyButton() );

    // Setup icon and titel
    ui->labelHeader->setText( item->page->getTitle() );
    ui->labelIcon->setPixmap( item->page->getIcon() );

    // Remove list widget selection
    ui->listWidget->clearSelection();

    // Trigger load method of page
    item->page->load();
}


void
MsmWindow::buttonShowAllSettings_clicked()
{
    PageWidget* page = dynamic_cast<PageWidget*>( ui->stackedWidget->currentWidget() );
    if ( page && !page->showAllSettingsRequested() )
        return;

    // Remove list widget selection
    ui->listWidget->clearSelection();

    // Setup icon and titel
    ui->labelHeader->setText( tr( "Manjaro Settings" ) );
    ui->labelIcon->setPixmap( QPixmap( ":/images/resources/settings.png" ) );

    // Hide buttons
    ui->buttonAllSettings->setVisible( false );
    ui->buttonApply->setVisible( false );

    // Show all settings
    ui->stackedWidget->setCurrentIndex( 0 );
}


void
MsmWindow::setApplyEnabled( PageWidget* page, bool enabled )
{
    if ( dynamic_cast<PageWidget*>( ui->stackedWidget->currentWidget() ) != page )
        return;
    ui->buttonApply->setEnabled( enabled );
}


void
MsmWindow::buttonApply_clicked()
{
    PageWidget* page = dynamic_cast<PageWidget*>( ui->stackedWidget->currentWidget() );
    if ( !page )
        return;
    page->save();
}


void
MsmWindow::closePageRequested( PageWidget* page )
{
    if ( dynamic_cast<PageWidget*>( ui->stackedWidget->currentWidget() ) != page )
        return;
    buttonShowAllSettings_clicked();
}


void
MsmWindow::writePositionSettings()
{
    QSettings settings( "manjaro", "manjaro-settings-manager" );

    settings.beginGroup( "mainwindow" );

    settings.setValue( "geometry", saveGeometry() );
    settings.setValue( "savestate", saveState() );
    settings.setValue( "maximized", isMaximized() );
    if ( !isMaximized() )
    {
        settings.setValue( "pos", pos() );
        settings.setValue( "size", size() );
    }

    settings.endGroup();
}


void
MsmWindow::readPositionSettings()
{
    QSettings settings( "manjaro", "manjaro-settings-manager" );

    settings.beginGroup( "mainwindow" );

    restoreGeometry( settings.value( "geometry", saveGeometry() ).toByteArray() );
    restoreState( settings.value( "savestate", saveState() ).toByteArray() );
    move( settings.value( "pos", pos() ).toPoint() );
    resize( settings.value( "size", size() ).toSize() );
    if ( settings.value( "maximized", isMaximized() ).toBool() )
        showMaximized();

    settings.endGroup();
}


void
MsmWindow::closeEvent( QCloseEvent* )
{
    writePositionSettings();
}
