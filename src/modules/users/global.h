/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
 *  Ramon Buldo <ramon@manjaro.org>
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

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStringList>
#include <QList>
#include <QHash>
#include <QMap>
#include <QRegExp>
#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QtCore/QByteArray>

class Global
{
public:

    struct User
    {
        QString username, homePath;
        int uuid;
    };

    struct Group
    {
        QString name;
        QStringList members;
    };

    static QList<Global::User> getAllUsers();
    static QList<Global::Group> getAllGroups();

private:

};

#endif // GLOBAL_H
