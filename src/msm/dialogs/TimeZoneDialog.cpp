#include "TimeZoneDialog.h"
#include "ui_TimeZoneDialog.h"

TimeZoneDialog::TimeZoneDialog( QWidget* parent ) :
    QDialog( parent ),
    ui( new Ui::TimeZoneDialog ),
    blockTimeZoneWidgetSet( false )
{
    ui->setupUi( this );
    timeZoneWidget = new TimeZoneWidget( this );
    ui->timeZoneLayout->addStretch();
    ui->timeZoneLayout->addWidget( timeZoneWidget );
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
        if ( !blockTimeZoneWidgetSet )
            timeZoneWidget->setCurrentLocation( ui->regionComboBox->currentText(), current );
    } );

    connect( timeZoneWidget, &TimeZoneWidget::locationChanged,
             [this]( LocaleGlobal::Location location )
    {
        blockTimeZoneWidgetSet = true;

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

        blockTimeZoneWidgetSet = false;
    } );
}


TimeZoneDialog::~TimeZoneDialog()
{
    delete ui;
    delete timeZoneWidget;
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
        timeZoneWidget->setCurrentLocation( initialRegion, initialZone );
    else
        timeZoneWidget->setCurrentLocation( "Europe", "Berlin" );
    emit timeZoneWidget->locationChanged( timeZoneWidget->getCurrentLocation() );
}

QString
TimeZoneDialog::getCurrentLocation()
{
    return QString( "%1/%2" )
           .arg( timeZoneWidget->getCurrentLocation().region )
           .arg( timeZoneWidget->getCurrentLocation().zone );
}
