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

#include "dialogs/TimeZoneDialog.h"

#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QTimeZone>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>

PageTimeDate::PageTimeDate(QWidget *parent) :
    PageWidget(parent),
    ui(new Ui::PageTimeDate),
    timeDate(new TimeDate)
{
    ui->setupUi(this);
    setTitel(tr("Time and Date"));
    setIcon(QPixmap(":/images/resources/timedate.png"));
    setShowApplyButton(true);

    connect(ui->timeEdit, &QTimeEdit::timeChanged, this, &PageTimeDate::timeEdited);
    connect(ui->dateEdit, &QTimeEdit::dateChanged, this, &PageTimeDate::dateEdited);
    connect(ui->isNtpEnabledCheckBox, &QCheckBox::clicked, this, &PageTimeDate::isNtpEnabledClicked);
    connect(ui->timeZonePushButton, &QPushButton::clicked, this, &PageTimeDate::timeZoneClicked);
}

PageTimeDate::~PageTimeDate()
{
    delete ui;
    delete timeDate;
}

void PageTimeDate::activated()
{
    isTimeEdited_ = false;
    isDateEdited_ = false;
    timeZone_ = timeDate->timeZone();
    updateFields();
    updateTimeFields();
    isNtpEnabledClicked();
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PageTimeDate::updateTimeFields);
    timer->start(1000);
}

void PageTimeDate::apply_clicked()
{
    if (ui->isNtpEnabledCheckBox->isChecked() != timeDate->isNtpEnabled()) {
        timeDate->setNtp(ui->isNtpEnabledCheckBox->isChecked());
    }

    // Only possible to modify date if ntp is disabled
    if ((isTimeEdited_ || isDateEdited_) && !ui->isNtpEnabledCheckBox->isChecked()) {
        QDateTime time(ui->dateEdit->date(), ui->timeEdit->time());
        timeDate->setTime(time);
    }

    if (ui->isRtcLocalCheckBox->isChecked() != timeDate->isRtcInLocalTimeZone()) {
        timeDate->setLocalRtc(ui->isRtcLocalCheckBox->isChecked());
    }

    if (!timeZone_.isEmpty() && QTimeZone(timeZone_.toLatin1()).isValid()) {
        if (timeZone_ != timeDate->timeZone()) {
            timeDate->setTimeZone(timeZone_);
        }
    }

    isTimeEdited_ = false;
    isDateEdited_ = false;
    updateFields();
    updateTimeFields();
}

void PageTimeDate::updateFields()
{
    if (timeDate->canNtp()) {
        ui->isNtpEnabledCheckBox->setChecked(timeDate->isNtpEnabled());
    } else {
        ui->isNtpEnabledCheckBox->setChecked(false);
        ui->isNtpEnabledCheckBox->setEnabled(false);
    }

    ui->isRtcLocalCheckBox->setChecked(timeDate->isRtcInLocalTimeZone());

    QTimeZone timeZone = QTimeZone(timeZone_.toLatin1());
    if (timeZone.isValid()) {
        ui->timeZoneLabel->setText(tr("<b>Time Zone:</b> %1").arg(timeZone_));
        ui->countryLabel->setText(tr("<b>Country:</b> ") + QLocale::countryToString(timeZone.country()));
        ui->hasDaylightTimeCheckBox->setChecked(timeZone.hasDaylightTime());
        ui->isDaylightTimeCheckBox->setChecked(timeZone.isDaylightTime(QDateTime::currentDateTime()));
        ui->hasTransitionsCheckBox->setChecked(timeZone.hasTransitions());

        QTimeZone::OffsetData offset = timeZone.nextTransition(QDateTime::currentDateTime());
        if (offset.atUtc != QDateTime()) {
            ui->nextTransitionLabel->setEnabled(true);
            ui->nextTransitionLabel->setText(tr("<b>Next transition:</b> %1")
                                             .arg(offset.atUtc.toString("ddd yyyy-MM-dd HH:mm:ss")));
        } else {
            ui->nextTransitionLabel->setEnabled(false);
            ui->nextTransitionLabel->setText(tr("<b>Next transition:</b> none"));
        }
    }
}

void PageTimeDate::updateTimeFields() {
    if (!isTimeEdited_) {
        ui->timeEdit->blockSignals(true);
        ui->timeEdit->setTime(timeDate->localDateTime().time());
        ui->timeEdit->blockSignals(false);
    }
    if (!isDateEdited_) {
        ui->dateEdit->blockSignals(true);
        ui->dateEdit->setDate(timeDate->localDateTime().date());
        ui->dateEdit->blockSignals(false);
    }
    ui->utcTimeLabel->setText(tr("<b>Universal time:</b> %1")
                               .arg(timeDate->utcDateTime().toString("ddd yyyy-MM-dd HH:mm:ss")));
    ui->rtcTimeLabel->setText(tr("<b>RTC time:</b> %1")
                               .arg(timeDate->rtcDateTime().toString("ddd yyyy-MM-dd HH:mm:ss")));
}

void PageTimeDate::timeEdited()
{
    isTimeEdited_ = true;
}

void PageTimeDate::dateEdited()
{
    isDateEdited_ = true;
}

void PageTimeDate::isNtpEnabledClicked()
{
    if (ui->isNtpEnabledCheckBox->isChecked()) {
        ui->timeEdit->setEnabled(false);
        ui->dateEdit->setEnabled(false);
    } else {
        ui->timeEdit->setEnabled(true);
        ui->dateEdit->setEnabled(true);
    }
}

void PageTimeDate::timeZoneClicked()
{
    TimeZoneDialog dialog(this);
    QString region = timeZone_.split("/").value(0);
    QString zone = timeZone_.split("/").value(1);
    dialog.init(region, zone);
    dialog.exec();
    if (dialog.getCurrentLocation() != timeZone_) {
        timeZone_ = dialog.getCurrentLocation();
        updateFields();
        updateTimeFields();
    }
}
