/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Ramon Buldó <ramon@manjaro.org>
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


#include "TimeDateCommon.h"
#include "TimeZoneDialog.h"

#include <QtCore/QTimeZone>
#include <QDebug>


QString
TimeDateCommon::getName()
{
    return QString( "msm_timedate" );
}


QString
TimeDateCommon::getTitle()
{
    return QString( tr ( "Time and Date" ) );
}


QString
TimeDateCommon::getDescription()
{
    return QString( tr ( "Time and date configuration" ) );
}


// TODO: return true if succesfull and do the right thing with the ui if not.
void
TimeDateCommon::save( Ui::PageTimeDate* ui, TimeDateService* timeDateService,
                      bool isTimeEdited, bool isDateEdited, QString timeZone )
{
    if ( ui->isNtpEnabledCheckBox->isChecked() != timeDateService->isNtpEnabled() )
        timeDateService->setNtp( ui->isNtpEnabledCheckBox->isChecked() );

    // Only possible to modify date if ntp is disabled
    if ( ( isTimeEdited || isDateEdited ) && !ui->isNtpEnabledCheckBox->isChecked() )
    {
        QDateTime time( ui->dateEdit->date(), ui->timeEdit->time() );
        timeDateService->setTime( time );
    }

    if ( ui->isRtcLocalCheckBox->isChecked() != timeDateService->isRtcInLocalTimeZone() )
        timeDateService->setLocalRtc( ui->isRtcLocalCheckBox->isChecked() );

    if ( !timeZone.isEmpty() && QTimeZone( timeZone.toLatin1() ).isValid() )
    {
        if ( timeZone != timeDateService->timeZone() )
            timeDateService->setTimeZone( timeZone );
    }
}


QString
TimeDateCommon::showTimeZoneSelector( QString& currentTimeZone )
{
    TimeZoneDialog dialog( nullptr );
    QString region = currentTimeZone.split( "/" ).value( 0 );
    QString zone = currentTimeZone.split( "/" ).value( 1 );
    dialog.init( region, zone );
    dialog.exec();
    if ( dialog.currentLocation() != currentTimeZone )
        return dialog.currentLocation();
    else
        return QString();
}


void
TimeDateCommon::updateUi( Ui::PageTimeDate* ui, TimeDateService* timeDateService,
                          bool isTimeEdited, bool isDateEdited, QString currentTimeZone )
{
    if ( timeDateService->canNtp() )
        ui->isNtpEnabledCheckBox->setChecked( timeDateService->isNtpEnabled() );
    else
    {
        ui->isNtpEnabledCheckBox->setChecked( false );
        ui->isNtpEnabledCheckBox->setEnabled( false );
    }

    ui->isRtcLocalCheckBox->setChecked( timeDateService->isRtcInLocalTimeZone() );

    QTimeZone timeZone = QTimeZone( currentTimeZone.toLatin1() );
    if ( timeZone.isValid() )
    {
        ui->timeZoneLabel_2->setText( currentTimeZone );
        ui->countryLabel_2->setText( QLocale::countryToString( timeZone.country() ) );

        QIcon yesIcon = QIcon();
        yesIcon.addFile( ":/images/yes.svg", QSize( 16, 16 ) );
        QIcon noIcon = QIcon();
        noIcon.addFile( ":/images/no.svg", QSize( 16, 16 ) );
        if ( timeZone.hasDaylightTime() )
            ui->hasDaylightTimeIcon->setPixmap( yesIcon.pixmap( QSize( 16, 16 ) ) );
        else
            ui->hasDaylightTimeIcon->setPixmap( noIcon.pixmap( QSize( 16, 16 ) ) );
        if ( timeZone.isDaylightTime( QDateTime::currentDateTime() ) )
            ui->isDaylightTimeIcon->setPixmap( yesIcon.pixmap( QSize( 16, 16 ) ) );
        else
            ui->isDaylightTimeIcon->setPixmap( noIcon.pixmap( QSize( 16, 16 ) ) );
        if ( timeZone.hasTransitions() )
            ui->hasTransitionsIcon->setPixmap( yesIcon.pixmap( QSize( 16, 16 ) ) );
        else
            ui->hasTransitionsIcon->setPixmap( noIcon.pixmap( QSize( 16, 16 ) ) );

        QTimeZone::OffsetData offset = timeZone.nextTransition( QDateTime::currentDateTime() );
        if ( offset.atUtc != QDateTime() )
        {
            ui->nextTransitionLabel->setEnabled( true );
            ui->nextTransitionTimeLabel->setEnabled( true );
            ui->nextTransitionTimeLabel->setText( offset.atUtc.toString( "dddd yyyy-MM-dd HH:mm:ss" ) );
        }
        else
        {
            ui->nextTransitionLabel->setEnabled( false );
            ui->nextTransitionTimeLabel->setEnabled( false );
            ui->nextTransitionTimeLabel->setText( tr( "none" ) );
        }
    }

    updateTimeFields( ui, timeDateService, isTimeEdited, isDateEdited );
}


void
TimeDateCommon::updateTimeFields( Ui::PageTimeDate* ui, TimeDateService* timeDateService,
                                  bool isTimeEdited, bool isDateEdited )
{
    if ( !isTimeEdited )
    {
        ui->timeEdit->blockSignals( true );
        ui->timeEdit->setTime( timeDateService->localDateTime().time() );
        ui->timeEdit->blockSignals( false );
    }
    if ( !isDateEdited )
    {
        ui->dateEdit->blockSignals( true );
        ui->dateEdit->setDate( timeDateService->localDateTime().date() );
        ui->dateEdit->blockSignals( false );
    }
    ui->utcTimeLabel->setText( timeDateService->utcDateTime().toString( "dddd yyyy-MM-dd HH:mm:ss" ) );
    ui->rtcTimeLabel->setText( timeDateService->rtcDateTime().toString( "dddd yyyy-MM-dd HH:mm:ss" ) );
}
