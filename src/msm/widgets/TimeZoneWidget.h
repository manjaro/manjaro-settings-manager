#ifndef TIMEZONEWIDGET_H
#define TIMEZONEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QStringList>
#include <QMouseEvent>
#include <QFontMetrics>
#include <QFont>
#include "LocaleGlobal.h"


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

    LocaleGlobal::Location getCurrentLocation()
    {
        return currentLocation;
    }
    void setCurrentLocation( QString region, QString zone );
    void setCurrentLocation( LocaleGlobal::Location location );

signals:
    void locationChanged( LocaleGlobal::Location location );

private:
    QFont font;
    QImage background, pin, currentZoneImage;
    QList<QImage> timeZoneImages;
    LocaleGlobal::Location currentLocation;

    QPoint getLocationPosition( double longitude, double latitude );

    void paintEvent( QPaintEvent* event );
    void mousePressEvent( QMouseEvent* event );
};

#endif // TIMEZONEWIDGET_H

