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

#ifndef PAGE_LANGUAGE_H
#define PAGE_LANGUAGE_H

#include "widgets/pagewidget.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QRadioButton>
#include <QButtonGroup>
#include <QList>
#include <QStringList>
#include <QMessageBox>
#include <QProcess>
#include <global.h>
#include <const.h>
#include "dialogs/applydialog.h"
#include "widgets/selectlocalesdialog.h"


namespace Ui {
class Page_Language;
}


class Page_Language : public PageWidget
{
    Q_OBJECT
    
public:
    explicit Page_Language(QWidget *parent = 0);
    ~Page_Language();

    void activated();
    void apply_clicked();
    
private:
    class TreeWidgetItem : public QTreeWidgetItem {
    public:
        TreeWidgetItem(QTreeWidget * parent) : QTreeWidgetItem(parent) {}
        QRadioButton localeRadioButton;
        QRadioButton formatsRadioButton;
    };
    QButtonGroup groupLocale;
    QButtonGroup groupFormats;
    Ui::Page_Language *ui;
    QProcess process;

protected slots:
    void buttonRemove_clicked();
    void buttonAdd_clicked();
    void buttonRestore_clicked();

};

#endif // PAGE_LANGUAGE_H
