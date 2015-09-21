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

#ifndef PAGELANGUAGEPACKAGES_H
#define PAGELANGUAGEPACKAGES_H

#include "global.h"
#include "LanguagePackagesItem.h"

#include <KCModule>

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMap>
#include <QMapIterator>
#include <QMessageBox>


namespace Ui
{
class PageLanguagePackages;
}


class PageLanguagePackages : public KCModule
{
    Q_OBJECT

public:
    /**
     * Constructor.
     *
     * @param parent Parent widget of the module
     * @param args Arguments for the module
     */
    explicit PageLanguagePackages( QWidget* parent, const QVariantList& args = QVariantList() );
    /**
     * Destructor.
     */
    ~PageLanguagePackages();

    /**
     * Overloading the KCModule load() function.
     */
    void load();

    /**
     * Overloading the KCModule save() function.
     */
    void save();

    /**
     * Overloading the KCModule defaults() function.
     */
    void defaults();



private:
    Ui::PageLanguagePackages* ui;

    void addLanguagePackagesToTreeWidget( QTreeWidget* treeWidget, QList<Global::LanguagePackage>* languagePackages, bool checkable );
    bool isSystemUpToDate();

    QList<LanguagePackagesItem> getLanguagePackages();
    QStringList checkInstalled( const QStringList& packages );
    QStringList checkInstalledLanguagePackages( QString package );
    QStringList checkAvailableLanguagePackages( QString package );
    void getInstalledPackages();
    void getAvailablePackages();

    QStringList m_installedPackages;
    QStringList m_availablePackages;
};

#endif // PAGELANGUAGEPACKAGES_H
