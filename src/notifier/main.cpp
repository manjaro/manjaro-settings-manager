/*
 *  Manjaro Settings Manager
 *  Ramon Buldó <rbuldo@gmail.com>
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

#include "NotifierApp.h"
#include "Notifier.h"
#include "kdsingleapplicationguard/kdsingleapplicationguard.h"

#include <QtWidgets/QApplication>
#include <QtCore/QTranslator>
#include <QtCore/QFile>
#include <QtCore/QDir>

#include <QDebug>

int main( int argc, char* argv[] )
{
    NotifierApp app( argc, argv );

    Q_INIT_RESOURCE( language_packages );
    Q_INIT_RESOURCE( translations );

    QTranslator appTranslator;
    appTranslator.load( ":/translations/msmd_" + QLocale::system().name() );
    app.installTranslator( &appTranslator );

    KDSingleApplicationGuard guard( KDSingleApplicationGuard::AutoKillOtherInstances );

    int returnCode = 0;
    if ( guard.isPrimaryInstance() )
    {
        app.init();
        Notifier notifier( &app );
        returnCode = app.exec();
    }
    else
        qDebug() << "MSM Notifier is already running, shutting down.";

    return returnCode;
}

