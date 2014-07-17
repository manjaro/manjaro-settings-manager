/*
 *  Manjaro Settings Manager
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

#ifndef PAGE_NOTIFICATIONS_H
#define PAGE_NOTIFICATIONS_H

#include <QSettings>
#include <QWidget>
#include "widgets/pagewidget.h"

namespace Ui {
class PageNotifications;
}

class PageNotifications : public PageWidget
{
    Q_OBJECT

public:
    explicit PageNotifications(QWidget *parent = 0);
    ~PageNotifications();

    void activated();
    void apply_clicked();

private:
    Ui::PageNotifications *ui;

protected slots:
    void unsupportedKernelStateBoxChanged(int);
    void newKernelStateBoxChanged(int);
};

#endif // PAGE_NOTIFICATIONS_H
