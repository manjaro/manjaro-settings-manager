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

#ifndef MSMAPPLICATION_H
#define MSMAPPLICATION_H

#include <QtWidgets/QApplication>

#define APP MsmApplication::instance()

class MsmWindow;

class MsmApplication : public QApplication
{
    Q_OBJECT
public:
    MsmApplication( int& argc, char* argv[] );
    virtual ~MsmApplication();

    void init();
    static MsmApplication* instance();

    void setDebug( bool enabled );
    void setStartModule( QString module );
    bool isDebug();

    MsmWindow* mainWindow();

private:
    MsmWindow* m_mainwindow;
    bool m_debugMode;
    QString m_startModule;
};

#endif //MSMAPPLICATION_H

