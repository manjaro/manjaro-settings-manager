/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Roland Singer <roland@manjaro.org>
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

#ifndef PAGEUSERS_H
#define PAGEUSERS_H

#include "PageWidget.h"
#include "UsersCommon.h"

namespace Ui
{
class PageUsers;
}


class UsersPage : public PageWidget
{
    Q_OBJECT

public:
    explicit UsersPage( QWidget* parent = 0 );
    ~UsersPage();

    void load();

private:
    class ListWidgetItem : public QListWidgetItem
    {
    public:
        ListWidgetItem( QListWidget* parent ) : QListWidgetItem( parent ) {}
        UsersCommon::User user;
    };

    Ui::PageUsers* ui;
};

#endif // PAGEUSERS_H
