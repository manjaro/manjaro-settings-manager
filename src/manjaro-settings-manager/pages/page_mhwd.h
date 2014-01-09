/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
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


#ifndef PAGE_MHWD_H
#define PAGE_MHWD_H

#include "widgets/pagewidget.h"
#include <QString>
#include <QStringList>
#include <QList>
#include <QListWidgetItem>
#include <QIcon>
#include <QMenu>
#include <QAction>
#include <string>
#include <vector>
#include <mhwd.h>
#include "global.h"
#include "dialogs/applydialog.h"

namespace Ui {
class Page_MHWD;
}

class Page_MHWD : public PageWidget
{
    Q_OBJECT
    
public:
    explicit Page_MHWD(QWidget *parent = 0);
    ~Page_MHWD();

    void activated();
    
private:
    Ui::Page_MHWD *ui;
    QAction *installAction;
    QAction *removeAction;
    QAction *forceReinstallationAction;

private slots:
    void buttonInstallFree_clicked();
    void buttonInstallNonFree_clicked();
    void showContextMenuForListWidget(const QPoint &);
    void installAction_triggered();
    void removeAction_triggered();
    void forceReinstallationAction_triggered();
    void checkBoxShowAll_toggled();
};

#endif // PAGE_MHWD_H
