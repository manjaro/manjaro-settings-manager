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

#include "MsmApplication.h"
#include <QtWidgets/QApplication>
#include <QtCore/QTranslator>
#include <QtCore/QFile>
#include <QtCore/QCommandLineParser>
#include <QtCore/QDir>

#include <QDebug>

int main( int argc, char* argv[] )
{
    MsmApplication app( argc, argv );
    Q_INIT_RESOURCE(kernel);

    QCommandLineParser parser;
    parser.setApplicationDescription( app.applicationName() );
    parser.addHelpOption();
    //parser.addVersionOption();

    QCommandLineOption debugOption( QStringList() << "d" << "debug",
                                    "Verbose output for debugging purposes." );
    parser.addOption( debugOption );

    QCommandLineOption moduleOption( QStringList() << "m" << "module",
                                     "Module to open at start.",
                                     "module" );
    parser.addOption( moduleOption );

    parser.process( app );

    app.setDebug( parser.isSet( debugOption ) );
    if ( parser.isSet( moduleOption ) )
        app.setStartModule( parser.value( moduleOption ) );

    QTranslator appTranslator;
    appTranslator.load(":/translations/msm_" + QLocale::system().name());
    app.installTranslator(&appTranslator);

    app.init();
    return app.exec();
}
