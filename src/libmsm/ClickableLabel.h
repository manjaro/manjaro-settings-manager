/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Sanchit Samuel <sanchit.samuel@live.com>
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

#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

class ClickableLabel : public QLabel
{
    Q_OBJECT
    
public:
    explicit ClickableLabel( const QString& text="", QWidget* parent=0 );
    ~ClickableLabel();
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif // CLICKABLELABEL_H
