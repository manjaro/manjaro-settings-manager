/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
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

#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QString>


class PageWidget : public QWidget
{
    Q_OBJECT
public:
    enum MessageType {
        Success = Qt::UserRole + 1,
        Info,
        Warning,
        Error
    };

    explicit PageWidget(QWidget *parent = 0);

    QPixmap getIcon();
    QString getTitel();
    bool getShowApplyButton();

    virtual void activated() {}
    virtual bool showAllSettingsRequested() { return true; }
    virtual void apply_clicked() {}

signals:
    void setApplyEnabled(PageWidget *page, bool enabled);
    void closePage(PageWidget *page);
    void showMessage(PageWidget *page, QString message, PageWidget::MessageType type);

protected:
    void setTitel(QString titel);
    void setIcon(QPixmap icon);
    void setShowApplyButton(bool visible);
    
private:
    QPixmap icon;
    QString titel;
    bool showApplyButton;
    
};

#endif // PAGEWIDGET_H
