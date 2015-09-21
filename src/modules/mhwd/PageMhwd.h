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

#include <KCModule>

namespace Ui
{
class PageMhwd;
}

class PageMhwd : public KCModule
{
    Q_OBJECT

public:
    /**
     * Constructor.
     *
     * @param parent Parent widget of the module
     * @param args Arguments for the module
     */
    explicit PageMhwd( QWidget* parent, const QVariantList& args = QVariantList() );
    /**
     * Destructor.
     */
    ~PageMhwd();

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
    Ui::PageMhwd* ui;
    QAction* m_installAction;
    QAction* m_removeAction;
    QAction* m_forceReinstallationAction;

private slots:
    void installFreeConfiguration();
    void installNonFreeConfiguration();
    void showContextMenuForTreeWidget( const QPoint& );
    void installConfiguration();
    void removeConfiguration();
    void reinstallConfiguration();
};

#endif // PAGEMHWD_H
