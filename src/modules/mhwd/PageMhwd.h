/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Roland Singer <roland@manjaro.org>
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

#ifndef PAGEMHWD_H
#define PAGEMHWD_H

#include "ApplyDialog.h"
#include "PageWidget.h"

#include <QString>
#include <QStringList>
#include <QList>
#include <QIcon>
#include <QMenu>
#include <QAction>
#include <string>
#include <vector>

namespace Ui
{
class PageMhwd;
}


class PageMhwd : public PageWidget
{
    Q_OBJECT

public:
    explicit PageMhwd( QWidget* parent = 0 );
    ~PageMhwd();

    void load();

private:
    Ui::PageMhwd* ui;
};

#endif // PAGEMHWD_H
