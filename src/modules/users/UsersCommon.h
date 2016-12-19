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

#ifndef USERSCOMMON_H
#define USERSCOMMON_H

#include "ui_PageUsers.h"

#include "ListWidget.h"

class UsersCommon : public QObject
{
    Q_OBJECT
public:

    struct User
    {
        QString username;
        QString homePath;
        int uuid;
    };

    struct Group
    {
        QString name;
        QStringList members;
    };

    static QString getDescription();
    static QString getName();
    static QString getTitle();
    static bool addUser();
    static void changePassword( QListWidgetItem* currentItem );
    static bool changeAccountType( Ui::PageUsers* ui );
    static void loadUsers( ListWidget* listWidget );
    static void removeUser( QListWidgetItem* currentItem );
    static void setUserImage( Ui::PageUsers* ui );
    static void setupUserData( Ui::PageUsers* ui, QListWidgetItem* current );

private:
    class ListWidgetItem : public QListWidgetItem
    {
    public:
        ListWidgetItem( QListWidget* parent ) : QListWidgetItem( parent ) {}
        UsersCommon::User user;
    };

    static QList<UsersCommon::User> Users();
    static QList<UsersCommon::Group> Groups();
};

#endif // USERSCOMMON_H
