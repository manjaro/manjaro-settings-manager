/*
 *  Manjaro Settings Manager
 *  Ramon Buldó <ramon@manjaro.org>
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

#include "PageTimeDate.h"
#include "ui_PageTimeDate.h"

#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QTimeZone>
#include <QtCore/QDateTime>

PageTimeDate::PageTimeDate(QWidget *parent) :
    PageWidget(parent),
    ui(new Ui::PageTimeDate),
    timeDate(new TimeDate)
{
    ui->setupUi(this);
    setTitel(tr("Time and Date"));
    setIcon(QPixmap(":/images/resources/help.png"));

    ui->localDateTimeEdit->setDateTime( timeDate->localDateTime() );
    ui->utcDateTimeEdit->setDateTime( timeDate->utcDateTime() );
    ui->rtcDateTimeEdit->setDateTime( timeDate->rtcDateTime() );
    ui->timeZonelineEdit->setText( timeDate->timeZone() );
    ui->canNtpCheckBox->setChecked( timeDate->canNtp() );
    ui->isNtpEnabledCheckBox->setChecked( timeDate->isNtpEnabled() );
    ui->isNtpSyncCheckBox->setChecked( timeDate->isNtpSynchronized() );
    ui->isRtcInLocalTimezoneCheckBox->setChecked( timeDate->isRtcInLocalTimeZone() );

    qDebug() << timeDate->isDstActive();
    qDebug() << timeDate->lastDstChange();
    qDebug() << timeDate->nextDstChange();

    QList<QByteArray> ids = QTimeZone::availableTimeZoneIds();
    QStringList ids2;
    int current;
    for (QByteArray i : ids) {
        if ( i.startsWith("UTC") ) {
            continue;
        }
        ids2.append(i);
        if (&ids2.last() == ui->timeZonelineEdit->text() ) {
            current = ids2.length() - 1;
        }
    }

    QStringListModel *model = new QStringListModel();
    model->setStringList(ids2);
    ui->listView->setModel( model );
    QModelIndex index =  model->index( current );
    if ( index.isValid() ) {
        ui->listView->setCurrentIndex( index );
    }

    /*
    dbusInterface.connection().connect( "org.freedesktop.timedate1",
                                "/org/freedesktop/timedate1",
                                "org.freedesktop.DBus.Properties",
                                "PropertiesChanged", this,
                                SLOT(timedateChanged() ) );
                                */
}

PageTimeDate::~PageTimeDate()
{
    delete ui;
    delete timeDate;
}

void PageTimeDate::timedateChanged() {
    qDebug() << "hi";
}
