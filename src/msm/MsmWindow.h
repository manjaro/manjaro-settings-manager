/*
 *  Manjaro Settings Manager
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

#ifndef MSMWINDOW_H
#define MSMWINDOW_H

#include "ModuleView.h"

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedWidget>

#include <QListWidgetItem>
class MsmWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MsmWindow(QWidget *parent = 0);
    ~MsmWindow();
    void readPositionSettings();

private:
    QStackedWidget *stackedWidget;
    ModuleView *moduleView;
    QMap<QString, KCModuleInfo*> moduleInfoList;

    void init();
    void addPageWidget(ModuleView &page);
    void closeEvent(QCloseEvent *);
    void writePositionSettings();

protected slots:
    void listWidget_itemClicked(QListWidgetItem *);
    void buttonShowAllSettings_clicked();
    void setApplyEnabled(ModuleView *page, bool enabled);
    void buttonApply_clicked();
    void closePageRequested(ModuleView *page);

};


#endif // MSMWINDOW_H
