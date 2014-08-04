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

#ifndef PAGELANGUAGE_H
#define PAGELANGUAGE_H

#include <global.h>
#include <const.h>
#include "dialogs/applydialog.h"
#include "widgets/pagewidget.h"
#include "widgets/selectlocalesdialog.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QRadioButton>
#include <QButtonGroup>
#include <QList>
#include <QStringList>
#include <QMessageBox>



namespace Ui {
class PageLanguage;
}


class PageLanguage : public PageWidget
{
    Q_OBJECT
    
public:
    explicit PageLanguage(QWidget *parent = 0);
    ~PageLanguage();

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
    Ui::PageLanguage *ui;

protected slots:
    void buttonRemove_clicked();
    void buttonAdd_clicked();
    void buttonRestore_clicked();

};

#endif // PAGELANGUAGE_H
