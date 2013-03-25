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

#ifndef PAGE_KEYBOARD_H
#define PAGE_KEYBOARD_H

#include "widgets/pagewidget.h"
#include <QMap>
#include <QMapIterator>
#include <QListWidgetItem>
#include <QProcess>
#include <stdlib.h>
#include <global.h>
#include <QMessageBox>
#include "widgets/keyboardpreview.h"
#include "dialogs/applydialog.h"



namespace Ui {
class Page_Keyboard;
}


class Page_Keyboard : public PageWidget
{
    Q_OBJECT
    
public:
    explicit Page_Keyboard(QWidget *parent = 0);
    ~Page_Keyboard();

    void updateApplyEnabledState();
    void activated();
    void apply_clicked();
    
private:
    class LayoutItem : public QListWidgetItem {
    public:
        LayoutItem(QListWidget *parent) : QListWidgetItem(parent) {}
        QString data;
        Global::KeyboardInfo info;
    };

    Ui::Page_Keyboard *ui;
    KeyBoardPreview keyboardPreview;
    int defaultIndex;
    QMap<QString, QString> models;

protected slots:
    void listLayout_currentItemChanged(QListWidgetItem * current, QListWidgetItem * previous);
    void listVariant_currentItemChanged(QListWidgetItem * current, QListWidgetItem * previous);
    void buttonRestore_clicked();

};

#endif // PAGE_KEYBOARD_H
