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

#ifndef TIMEDATEPAGE_H
#define TIMEDATEPAGE_H

#include "TimeDateService.h"
#include "TimeZoneWidget.h"
#include "PageWidget.h"


namespace Ui
{
class PageTimeDate;
}


class TimeDatePage : public PageWidget
{
    Q_OBJECT

public:
    explicit TimeDatePage( QWidget* parent = 0 );
    ~TimeDatePage();
    void load();
    void save();

private:
    Ui::PageTimeDate* ui;
    TimeDateService* m_timeDateService;
    QTimer* m_timeFieldsTimer;
    TimeZoneWidget m_timeZoneWidget;
    bool m_isTimeEdited;
    bool m_isDateEdited;
    QString m_timeZone;
};

#endif // TIMEDATEPAGE_H
