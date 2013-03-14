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

#ifndef PAGE_LANGUAGEPACKAGES_H
#define PAGE_LANGUAGEPACKAGES_H


#include "pagewidget.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMap>
#include <QMapIterator>
#include <QMessageBox>
#include <global.h>
#include "applydialog.h"


namespace Ui {
class Page_LanguagePackages;
}



class Page_LanguagePackages : public PageWidget
{
    Q_OBJECT
    
public:
    explicit Page_LanguagePackages(QWidget *parent = 0);
    ~Page_LanguagePackages();

    void activated();
    void apply_clicked();
    
private:
    Ui::Page_LanguagePackages *ui;

    void addLanguagePackagesToTreeWidget(QTreeWidget *treeWidget, QList<Global::LanguagePackage> *languagePackages, bool checkable);

protected slots:
    void updateApplyEnabledState();

};

#endif // PAGE_LANGUAGEPACKAGES_H
