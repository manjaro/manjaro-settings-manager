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

#include "LanguagePackagesCommon.h"
#include "LanguagePackagesPage.h"
#include "ui_PageLanguagePackages.h"

#include <QtCore/QDebug>

LanguagePackagesPage::LanguagePackagesPage( QWidget* parent ) :
    PageWidget( parent ),
    ui( new Ui::PageLanguagePackages )
{
    ui->setupUi( this );
    setTitle( LanguagePackagesCommon::getTitle() );
    setIcon( QPixmap( ":/images/resources/locale-package.png" ) );
    setName( LanguagePackagesCommon::getName() );

    ui->treeWidgetAvailable->setColumnWidth( 0, 250 );
    ui->treeWidgetAvailable->setColumnWidth( 1, 300 );
    ui->treeWidgetAvailable->setColumnWidth( 2, 30 );

    ui->treeWidgetInstalled->setColumnWidth( 0, 300 );
    ui->treeWidgetInstalled->setColumnWidth( 1, 300 );

    connect( ui->installPackagesButton, &QPushButton::clicked,
             [=] ( bool checked )
    {
        Q_UNUSED( checked )
        LanguagePackagesCommon::installPackages( ui );
        load();
    } );
}


LanguagePackagesPage::~LanguagePackagesPage()
{
    delete ui;
}


void
LanguagePackagesPage::load()
{
    LanguagePackagesCommon::load( ui );
    QApplication::restoreOverrideCursor();
}
