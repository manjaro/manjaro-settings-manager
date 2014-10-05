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

#ifndef PAGETIMEDATE_H
#define PAGETIMEDATE_H

#include "models/TimeDate.h"
#include "widgets/TimeZoneWidget.h"
#include "widgets/PageWidget.h"

namespace Ui {
class PageTimeDate;
}

class PageTimeDate : public PageWidget
{
    Q_OBJECT

public:
    explicit PageTimeDate(QWidget *parent = 0);
    ~PageTimeDate();
    void activated();
    void apply_clicked();

public slots:
    void updateFields();
    void updateTimeFields();
    void timeEdited();
    void dateEdited();
    void isNtpEnabledToggled();
    void timeZoneClicked();

private:
    Ui::PageTimeDate *ui;
    TimeDate *timeDate;
    TimeZoneWidget timeZoneWidget;
    bool isTimeEdited_;
    bool isDateEdited_;
    QString timeZone_;

};

#endif // PAGETIMEDATE_H
