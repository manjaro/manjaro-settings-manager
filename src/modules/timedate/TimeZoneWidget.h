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

#ifndef TIMEZONEWIDGET_H
#define TIMEZONEWIDGET_H

#include "LocaleGlobal.h"

#include <QtWidgets/QWidget>

#define MAP_Y_OFFSET 0.125
#define MAP_X_OFFSET -0.0370
#define RGB_TRANSPARENT 0
#define ZONES "0.0 1.0 2.0 3.0 3.5 4.0 4.5 5.0 5.5 5.75 6.0 6.5 7.0 8.0 9.0 9.5 10.0 10.5 11.0 11.5 12.0 12.75 13.0 -1.0 -2.0 -3.0 -3.5 -4.0 -4.5 -5.0 -5.5 -6.0 -7.0 -8.0 -9.0 -9.5 -10.0 -11.0"
#define X_SIZE 780
#define Y_SIZE 340

class TimeZoneWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimeZoneWidget( QWidget* parent = 0 );

    void setCurrentLocation( QString region, QString zone );
    void setCurrentLocation( LocaleGlobal::Location location );

    LocaleGlobal::Location currentLocation() const;

signals:
    void locationChanged( LocaleGlobal::Location location );

private:
    QFont m_font;
    QImage m_background;
    QImage m_pin;
    QImage m_currentZoneImage;
    QList<QImage> m_timeZoneImages;
    LocaleGlobal::Location m_currentLocation;

    QPoint getLocationPosition( double longitude, double latitude );

    void paintEvent( QPaintEvent* event );
    void mousePressEvent( QMouseEvent* event );
};

#endif // TIMEZONEWIDGET_H

