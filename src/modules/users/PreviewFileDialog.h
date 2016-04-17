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
    explicit PreviewFileDialog( QWidget* parent = 0 );

private:
    QLabel m_labelPreview;
    QLabel m_labelPreviewWidth;
    QLabel m_labelPreviewHeight;
    QLabel m_labelPreviewRatio;
    QLabel m_labelPreviewWidthText;
    QLabel m_labelPreviewHeightText;
    QLabel m_labelPreviewRatioText;

private slots:
    void setPreviewPicture( const QString& picture );

};

#endif // PREVIEWFILEDIALOG_H
