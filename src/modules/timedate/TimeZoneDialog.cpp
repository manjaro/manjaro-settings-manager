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

#include "TimeZoneDialog.h"
#include "ui_TimeZoneDialog.h"

TimeZoneDialog::TimeZoneDialog( QWidget* parent ) :
    QDialog( parent ),
    ui( new Ui::TimeZoneDialog ),
    m_blockTimeZoneWidgetSet( false )
{
    ui->setupUi( this );
    m_timeZoneWidget = new TimeZoneWidget( this );
    ui->timeZoneLayout->addStretch();
    ui->timeZoneLayout->addWidget( m_timeZoneWidget );
    ui->timeZoneLayout->addStretch();

    connect( ui->regionComboBox,
             static_cast< void ( QComboBox::* )( const QString& ) >( &QComboBox::currentIndexChanged ),
             [this]( const QString& current )
    {
        QHash< QString, QList< LocaleGlobal::Location > > regions = LocaleGlobal::getLocations();
        if ( !regions.contains( current ) )
            return;

        ui->zoneComboBox->blockSignals( true );

        ui->zoneComboBox->clear();

        QList< LocaleGlobal::Location > zones = regions.value( current );
        foreach ( const LocaleGlobal::Location& zone, zones )
            ui->zoneComboBox->addItem( zone.zone );

        ui->zoneComboBox->model()->sort( 0 );

        ui->zoneComboBox->blockSignals( false );

        ui->zoneComboBox->currentIndexChanged( ui->zoneComboBox->currentText() );
    } );

    connect( ui->zoneComboBox,
             static_cast< void ( QComboBox::* )( const QString& ) >( &QComboBox::currentIndexChanged ),
             [this]( const QString& current )
    {
        if ( !m_blockTimeZoneWidgetSet )
            m_timeZoneWidget->setCurrentLocation( ui->regionComboBox->currentText(), current );
    } );

    connect( m_timeZoneWidget, &TimeZoneWidget::locationChanged,
             [this]( LocaleGlobal::Location location )
    {
        m_blockTimeZoneWidgetSet = true;

        // Set region index
        int index = ui->regionComboBox->findText( location.region );
        if ( index < 0 )
            return;

        ui->regionComboBox->setCurrentIndex( index );

        // Set zone index
        index = ui->zoneComboBox->findText( location.zone );
        if ( index < 0 )
            return;

        ui->zoneComboBox->setCurrentIndex( index );

        m_blockTimeZoneWidgetSet = false;
    } );
}


TimeZoneDialog::~TimeZoneDialog()
{
    delete ui;
    delete m_timeZoneWidget;
}


void
TimeZoneDialog::init( const QString& initialRegion, const QString& initialZone )
{
    ui->regionComboBox->blockSignals( true );
    ui->zoneComboBox->blockSignals( true );

    // Setup locations
    QHash< QString, QList< LocaleGlobal::Location > > regions = LocaleGlobal::getLocations();

    QStringList keys = regions.keys();
    keys.sort();

    foreach ( const QString& key, keys )
        ui->regionComboBox->addItem( key );

    ui->regionComboBox->blockSignals( false );
    ui->zoneComboBox->blockSignals( false );

    ui->regionComboBox->currentIndexChanged( ui->regionComboBox->currentText() );

    // Default location
    auto containsLocation = []( const QList< LocaleGlobal::Location >& locations,
                                const QString& zone ) -> bool
    {
        foreach ( const LocaleGlobal::Location& location, locations )
        {
            if ( location.zone == zone )
                return true;
        }
        return false;
    };

    if ( keys.contains( initialRegion ) &&
            containsLocation( regions.value( initialRegion ), initialZone ) )
        m_timeZoneWidget->setCurrentLocation( initialRegion, initialZone );
    else
        m_timeZoneWidget->setCurrentLocation( "Europe", "Berlin" );
    emit m_timeZoneWidget->locationChanged( m_timeZoneWidget->currentLocation() );
}


QString
TimeZoneDialog::currentLocation()
{
    return QString( "%1/%2" )
           .arg( m_timeZoneWidget->currentLocation().region )
           .arg( m_timeZoneWidget->currentLocation().zone );
}
