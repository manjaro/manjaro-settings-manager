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

#include "widgets/pagewidget.h"
#include "pages/page_languagepackages.h"
#include "pages/page_mhwd.h"
#include "pages/page_users.h"
#include "pages/PageKernel.h"
#include "pages/PageKeyboard.h"
#include "pages/PageLanguage.h"
#include "pages/PageNotifications.h"
#include "pages/PageTimeDate.h"

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
#include <QtCore/QTimer>


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
    Page_LanguagePackages page_LanguagePackages;
    Page_MHWD page_MHWD;
    PageKernel pageKernel;
    PageKeyboard pageKeyboard;
    PageLanguage pageLanguage;
    PageNotifications pageNotifications;
    PageTimeDate pageTimeDate;
    Page_Users page_Users;

    QTimer messageTimer_;

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
    void showMessage(PageWidget *page, QString message, PageWidget::MessageType type);
    void hideMessage();

};


#endif // MAINWINDOW_H
