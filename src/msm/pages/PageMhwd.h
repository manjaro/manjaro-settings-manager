/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
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


#ifndef PAGEMHWD_H
#define PAGEMHWD_H

#include "global/Global.h"
#include "ApplyDialog.h"
#include "PageWidget.h"

#include <QString>
#include <QStringList>
#include <QList>
#include <QIcon>
#include <QMenu>
#include <QAction>
#include <string>
#include <vector>

namespace Ui
{
class PageMhwd;
}


class PageMhwd : public PageWidget
{
    Q_OBJECT

public:
    explicit PageMhwd( QWidget* parent = 0 );
    ~PageMhwd();

    void load();

private:
    Ui::PageMhwd* ui;
    QAction* installAction;
    QAction* removeAction;
    QAction* forceReinstallationAction;

private slots:
    void buttonInstallFree_clicked();
    void buttonInstallNonFree_clicked();
    void showContextMenuForTreeWidget( const QPoint& );
    void installAction_triggered();
    void removeAction_triggered();
    void forceReinstallationAction_triggered();
};

#endif // PAGEMHWD_H
