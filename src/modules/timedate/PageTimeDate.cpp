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

#include "PageTimeDate.h"
#include "ui_PageTimeDate.h"

#include "TimeZoneDialog.h"

#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QTimeZone>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>


PageTimeDate::PageTimeDate( QWidget* parent ) :
    PageWidget( parent ),
    ui( new Ui::PageTimeDate ),
    m_timeDate( new TimeDate )
{
    ui->setupUi( this );
    setTitle( tr( "Time and Date" ) );
    setIcon( QPixmap( ":/images/resources/timedate.png" ) );
    setShowApplyButton( true );

    connect( ui->isNtpEnabledCheckBox, &QCheckBox::toggled, this, &PageTimeDate::isNtpEnabledToggled );
    connect( ui->timeZonePushButton, &QPushButton::clicked, this, &PageTimeDate::timeZoneClicked );

    connect( ui->timeEdit, &QTimeEdit::timeChanged,
             [this] ()
    {
        m_isTimeEdited = true;
        this -> setApplyEnabled( this, true );
    } );
    connect( ui->dateEdit, &QTimeEdit::dateChanged,
             [this] ()
    {
        m_isDateEdited = true;
        this -> setApplyEnabled( this, true );
    } );
    connect( ui->isRtcLocalCheckBox, &QCheckBox::toggled,
             [this] ()
    {
        this -> setApplyEnabled( this, true );
    } );
}


PageTimeDate::~PageTimeDate()
{
    delete ui;
    delete m_timeDate;
}


void
PageTimeDate::load()
{
    m_isTimeEdited = false;
    m_isDateEdited = false;
    m_timeZone = m_timeDate->timeZone();
    updateFields();
    updateTimeFields();
    QTimer* timer = new QTimer( this );
    connect( timer, &QTimer::timeout, this, &PageTimeDate::updateTimeFields );
    timer->start( 1000 );
}


void
PageTimeDate::save()
{
    if ( ui->isNtpEnabledCheckBox->isChecked() != m_timeDate->isNtpEnabled() )
        m_timeDate->setNtp( ui->isNtpEnabledCheckBox->isChecked() );

    // Only possible to modify date if ntp is disabled
    if ( ( m_isTimeEdited || m_isDateEdited ) && !ui->isNtpEnabledCheckBox->isChecked() )
    {
        QDateTime time( ui->dateEdit->date(), ui->timeEdit->time() );
        m_timeDate->setTime( time );
    }

    if ( ui->isRtcLocalCheckBox->isChecked() != m_timeDate->isRtcInLocalTimeZone() )
        m_timeDate->setLocalRtc( ui->isRtcLocalCheckBox->isChecked() );

    if ( !m_timeZone.isEmpty() && QTimeZone( m_timeZone.toLatin1() ).isValid() )
    {
        if ( m_timeZone != m_timeDate->timeZone() )
            m_timeDate->setTimeZone( m_timeZone );
    }

    m_isTimeEdited = false;
    m_isDateEdited = false;
    updateFields();
    updateTimeFields();
}


void
PageTimeDate::updateFields()
{
    if ( m_timeDate->canNtp() )
        ui->isNtpEnabledCheckBox->setChecked( m_timeDate->isNtpEnabled() );
    else
    {
        ui->isNtpEnabledCheckBox->setChecked( false );
        ui->isNtpEnabledCheckBox->setEnabled( false );
    }

    ui->isRtcLocalCheckBox->setChecked( m_timeDate->isRtcInLocalTimeZone() );

    QTimeZone timeZone = QTimeZone( m_timeZone.toLatin1() );
    if ( timeZone.isValid() )
    {
        ui->timeZoneLabel_2->setText( m_timeZone );
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
}


void
PageTimeDate::updateTimeFields()
{
    if ( !m_isTimeEdited )
    {
        ui->timeEdit->blockSignals( true );
        ui->timeEdit->setTime( m_timeDate->localDateTime().time() );
        ui->timeEdit->blockSignals( false );
    }
    if ( !m_isDateEdited )
    {
        ui->dateEdit->blockSignals( true );
        ui->dateEdit->setDate( m_timeDate->localDateTime().date() );
        ui->dateEdit->blockSignals( false );
    }
    ui->utcTimeLabel->setText( m_timeDate->utcDateTime().toString( "dddd yyyy-MM-dd HH:mm:ss" ) );
    ui->rtcTimeLabel->setText( m_timeDate->rtcDateTime().toString( "dddd yyyy-MM-dd HH:mm:ss" ) );
}


void
PageTimeDate::isNtpEnabledToggled()
{
    if ( ui->isNtpEnabledCheckBox->isChecked() )
    {
        ui->timeEdit->setEnabled( false );
        ui->dateEdit->setEnabled( false );
    }
    else
    {
        ui->timeEdit->setEnabled( true );
        ui->dateEdit->setEnabled( true );
    }
    this -> setApplyEnabled( this, true );
}


void
PageTimeDate::timeZoneClicked()
{
    TimeZoneDialog dialog( this );
    QString region = m_timeZone.split( "/" ).value( 0 );
    QString zone = m_timeZone.split( "/" ).value( 1 );
    dialog.init( region, zone );
    dialog.exec();
    if ( dialog.currentLocation() != m_timeZone )
    {
        m_timeZone = dialog.currentLocation();
        updateFields();
        updateTimeFields();
        this -> setApplyEnabled( this, true );
    }
}
