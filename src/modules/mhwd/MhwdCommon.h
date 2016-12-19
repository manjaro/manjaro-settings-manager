/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Ramon Buldó <ramon@manjaro.org>
 *
 *  Manjaro Settings Manager is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Manjaro Settings Manager is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Manjaro Settings Manager.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MHWDCOMMON_H
#define MHWDCOMMON_H

#include "ui_PageMhwd.h"

class MhwdCommon : public QObject
{
    Q_OBJECT
public:
    static QString getDescription();
    static QString getName();
    static QString getTitle();
    static void load( Ui::PageMhwd* ui );
    static bool installConfiguration( QString configuration );
    static bool installFreeConfiguration( );
    static bool installNonFreeConfiguration( );
    static bool reinstallConfiguration( QString configuration );
    static bool removeConfiguration( QString configuration );
    static void showItemContextMenu( Ui::PageMhwd* ui, const QPoint& pos );
};

#endif // MHWDCOMMON_H
