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

#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QString>


class PageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PageWidget( QWidget* parent = 0 );

    QPixmap getIcon();
    QString getTitle();
    QString getName();
    bool getShowApplyButton();

    virtual void load() {}
    virtual bool showAllSettingsRequested()
    {
        return true;
    }
    virtual void save() {}

signals:
    void setApplyEnabled( PageWidget* page, bool enabled );
    void closePage( PageWidget* page );

protected:
    void setTitle( QString title );
    void setIcon( QPixmap icon );
    void setName( QString name );
    void setShowApplyButton( bool visible );

private:
    QPixmap m_icon;
    QString m_title;
    QString m_name;
    bool m_showApplyButton;

};

#endif // PAGEWIDGET_H
