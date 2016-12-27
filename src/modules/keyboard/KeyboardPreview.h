/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Roland Singer <roland@manjaro.org>
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

#ifndef KEYBOARDPREVIEW_H
#define KEYBOARDPREVIEW_H

#include <QWidget>
#include <QRectF>
#include <QFont>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QColor>
#include <QPixmap>
#include <QProcess>
#include <QString>
#include <QStringList>


class KeyBoardPreview : public QWidget
{
    Q_OBJECT
public:
    explicit KeyBoardPreview( QWidget* parent = nullptr );

    void setLayout( const QString layout );
    void setVariant( const QString variant );

private:
    enum KB_TYPE { KB_104, KB_105, KB_106 };

    struct KB
    {
        bool kb_extended_return;
        QList<QList<int> > keys;
    };

    struct Code
    {
        QString plain;
        QString shift;
        QString ctrl;
        QString alt;
    };

    QString layout;
    QString variant;
    QFont lowerFont;
    QFont upperFont;
    KB* kb;
    KB kbList[3];
    QList<Code> codes;
    int space;
    double usable_width;
    double key_w;

    void loadInfo();
    bool loadCodes();
    QString regular_text( const int index ) const;
    QString shift_text( const int index ) const;
    QString ctrl_text( const int index ) const;
    QString alt_text( const int index ) const;
    QString fromUnicodeString( const QString raw ) const;

protected:
    void paintEvent( QPaintEvent* event );
    void resizeEvent( QResizeEvent* event );

};

#endif // KEYBOARDPREVIEW_H
