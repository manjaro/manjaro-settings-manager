/*
 *  Manjaro Settings Manager
 *  Ramon Buldó <ramon@manjaro.org>
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

#include "MsmWindow.h"
#include "ModuleView.h"

#include <QtCore/QSettings>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>

#include <KCModuleInfo>

#include <QDebug>

MsmWindow::MsmWindow( QWidget* parent ) :
    QMainWindow( parent )
{
    // Prepare the view area
    m_stackedWidget = new QStackedWidget( this );
    setCentralWidget( m_stackedWidget );

    QQuickView* view = new QQuickView();
    m_menuView = QWidget::createWindowContainer( view, this );
    m_menuView->setFocusPolicy( Qt::TabFocus );
    view->setSource( QUrl( "qrc:/qml/main.qml" ) );
    m_stackedWidget->addWidget( m_menuView );
    m_stackedWidget->setCurrentWidget( m_menuView );

    m_moduleView = new ModuleView();
    m_stackedWidget->addWidget( m_moduleView );

    QQuickItem* rootObject = view->rootObject();
    QQuickItem::connect( rootObject, SIGNAL( itemClicked( QString ) ),
                         this, SLOT( loadModule( QString ) ) );

    ModuleView::connect( m_moduleView, &ModuleView::closeRequest,
                         [=]()
    {
        m_moduleView->resolveChanges();
        m_moduleView->closeModules();
        m_stackedWidget->setCurrentWidget( m_menuView );
    } );

    init();
    readPositionSettings();
}


MsmWindow::~MsmWindow()
{
    delete m_moduleView;
}


void
MsmWindow::init()
{
    QStringList moduleList = QStringList() << "msm_kernel" << "msm_keyboard" << "msm_language_packages"
                             << "msm_locale" << "msm_mhwd" << "msm_timedate" << "msm_users";
    for ( QString module : moduleList )
        m_moduleInfoList.insert( module, new KCModuleInfo( module ) );
}


void
MsmWindow::loadModule( QString moduleName )
{
    qDebug() << QString( "Loading module '%1'" ).arg( moduleName );
    if ( m_moduleInfoList.contains( moduleName ) )
    {
        m_moduleView->addModule( m_moduleInfoList.value( moduleName ) );
        emit m_moduleView->moduleChanged( false );
    }
    m_stackedWidget->setCurrentWidget( m_moduleView );
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
