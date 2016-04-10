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

#ifndef USERSCOMMON_H
#define USERSCOMMON_H

#define LANGUAGE_PACKAGES_FILE  "/var/lib/manjaro-system-settings/language_packages.list"
#define LOCALE_GEN              "/etc/locale.gen"
#define LOCALE_CONF              "/etc/locale.conf"
#define LOCALESSUPPORTED "/usr/share/i18n/SUPPORTED"
#define LOCALESINFO "/usr/share/i18n/LOCALESINFO"
#define XKB_FILE "/usr/share/X11/xkb/rules/base.lst"
#define MIN_USER_UUID 1000
#define PASSWD "/etc/passwd"
#define GROUPCONF "/etc/group"
#define FACES_IMAGE_FOLDER "/usr/share/pixmaps/faces"
#define DEFAULT_USER_GROUPS "video,audio,power,disk,storage,optical,network,lp,scanner"
#define ADMIN_GROUP "wheel"
#define SUDOERSFILE "/etc/sudoers"

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

class UsersCommon
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

    static int runProcess( QString cmd, QStringList args, QStringList writeArgs, QString& error );
    static QList<UsersCommon::User> getAllUsers();
    static QList<UsersCommon::Group> getAllGroups();

private:

};

#endif // USERSCOMMON_H
