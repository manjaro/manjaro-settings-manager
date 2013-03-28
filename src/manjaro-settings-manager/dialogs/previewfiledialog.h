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

#ifndef PREVIEWFILEDIALOG_H
#define PREVIEWFILEDIALOG_H


#include <QFileDialog>
#include <QString>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLayoutItem>
#include <QSplitter>
#include <QWidget>
#include <QLabel>
#include <QPixmap>


class PreviewFileDialog : public QFileDialog
{
    Q_OBJECT
public:
    explicit PreviewFileDialog(QWidget *parent = 0);
    
private:
    QLabel labelPreview;
    QLabel labelPreviewWidth;
    QLabel labelPreviewHeight;
    QLabel labelPreviewRatio;
    QLabel labelPreviewWidthText;
    QLabel labelPreviewHeightText;
    QLabel labelPreviewRatioText;
    
private slots:
    void setPreviewPicture(const QString &picture);

};

#endif // PREVIEWFILEDIALOG_H
