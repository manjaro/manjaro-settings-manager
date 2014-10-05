/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pages/PageKernel.h"
#include "pages/PageKeyboard.h"
#include "pages/PageLanguage.h"
#include "pages/PageLanguagePackages.h"
#include "pages/PageMhwd.h"
#include "pages/PageNotifications.h"
#include "pages/PageTimeDate.h"
#include "pages/PageUsers.h"
#include "widgets/PageWidget.h"

#include <QApplication>
#include <QMainWindow>
#include <QString>
#include <QListWidget>
#include <QPixmap>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QListWidgetItem>
#include <QDesktopWidget>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QMessageBox>


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void readPositionSettings();

private:
    class ListWidgetItem : public QListWidgetItem
    {
    public:
        ListWidgetItem(QListWidget *parent) : QListWidgetItem(parent) { page = NULL; }

        PageWidget *page;
    };

    Ui::MainWindow *ui;
    PageKernel pageKernel;
    PageKeyboard pageKeyboard;
    PageLanguage pageLanguage;
    PageLanguagePackages pageLanguagePackages;
    PageMhwd pageMhwd;
    PageNotifications pageNotifications;
    PageTimeDate pageTimeDate;
    PageUsers pageUsers;

    void checkAppArguments();
    void addPageWidget(PageWidget &page);
    void closeEvent(QCloseEvent *);
    void writePositionSettings();

protected slots:
    void listWidget_itemClicked(QListWidgetItem *);
    void buttonShowAllSettings_clicked();
    void setApplyEnabled(PageWidget *page, bool enabled);
    void buttonApply_clicked();
    void closePageRequested(PageWidget *page);

};


#endif // MAINWINDOW_H
