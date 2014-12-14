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


#include "previewfiledialog.h"



PreviewFileDialog::PreviewFileDialog(QWidget *parent) :
    QFileDialog(parent)
{
    QGridLayout *gridLayout = dynamic_cast<QGridLayout *>(this->layout());

    if (gridLayout) {
        QLayoutItem *layoutItem = gridLayout->itemAtPosition(1,0);
        QSplitter *splitter;
        splitter = dynamic_cast<QSplitter *>(layoutItem->widget());

        if (splitter) {
            QFrame *framePreview = new QFrame(splitter);
            splitter->addWidget(framePreview);

            QVBoxLayout *vboxPreviewLayout = new QVBoxLayout(framePreview);
            QGridLayout *gridPreviewLayout = new QGridLayout();

            labelPreview.setFrameShape(QFrame::NoFrame);
            labelPreview.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            labelPreview.setMinimumSize(100,100);

            gridPreviewLayout->addWidget(&labelPreviewWidthText, 0, 0, 1, 1, Qt::AlignLeft);
            gridPreviewLayout->addWidget(&labelPreviewWidth, 0, 1, 1, 1, Qt::AlignLeft);
            gridPreviewLayout->addWidget(&labelPreviewHeightText, 1, 0, 1, 1, Qt::AlignLeft);
            gridPreviewLayout->addWidget(&labelPreviewHeight, 1, 1, 1, 1, Qt::AlignLeft);
            gridPreviewLayout->addWidget(&labelPreviewRatioText, 2, 0, 1, 1, Qt::AlignLeft);
            gridPreviewLayout->addWidget(&labelPreviewRatio, 2, 1, 1, 1, Qt::AlignLeft);
            gridPreviewLayout->setColumnStretch(1,1);

            vboxPreviewLayout->addWidget(&labelPreview, 1, Qt::AlignHCenter);
            vboxPreviewLayout->addLayout(gridPreviewLayout);
            vboxPreviewLayout->addStretch(1);

            framePreview->setLayout(vboxPreviewLayout);

            connect(this, SIGNAL(currentChanged(QString)), this, SLOT(setPreviewPicture(QString)));
        }
    }
}



void PreviewFileDialog::setPreviewPicture(const QString &picture) {
    QPixmap pixmapPicture;

    if (pixmapPicture.load(picture)) {
        int x, y;
        double ratio;
        x = pixmapPicture.width();
        y = pixmapPicture.height();
        ratio = double(x) / double(y);

        if ((x > labelPreview.width()) || (y > labelPreview.height())) {
            labelPreview.setScaledContents(true);
            if (x > y)
                labelPreview.setMinimumHeight(int(labelPreview.size().width() / ratio));
            else
                labelPreview.setMinimumWidth(int(labelPreview.size().height() * ratio));
        }
        else {
            labelPreview.setScaledContents(false);
            labelPreview.setMinimumSize(100,100);
        }

        labelPreview.setPixmap(picture);
        labelPreviewWidthText.setText(tr("Width:"));
        labelPreviewHeightText.setText(tr("Height:"));
        labelPreviewRatioText.setText(tr("Ratio:"));
        labelPreviewWidth.setText(tr("%1 px").arg(x));
        labelPreviewHeight.setText(tr("%1 px").arg(y));
        labelPreviewRatio.setText(tr("%1").arg(ratio,0,'f',3));
    }
    else {
        labelPreview.clear();
        labelPreviewWidthText.clear();
        labelPreviewHeightText.clear();
        labelPreviewRatioText.clear();
        labelPreviewWidth.clear();
        labelPreviewHeight.clear();
        labelPreviewRatio.clear();
    }
}
