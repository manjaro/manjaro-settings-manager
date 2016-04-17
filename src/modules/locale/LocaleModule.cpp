/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Roland Singer <roland@manjaro.org>
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

#include "LocaleModule.h"
#include "ui_LocaleModule.h"

#include <KAboutData>
#include <KAuth>
#include <KAuthAction>
#include <KPluginFactory>

#include <QtCore/QTranslator>
#include <QtWidgets/QMenu>

#include <QDebug>

K_PLUGIN_FACTORY( MsmLocaleFactory,
                  registerPlugin<LocaleModule>( "msm_locale" ); )

LocaleModule::LocaleModule( QWidget* parent, const QVariantList& args ) :
    KCModule( parent, args ),
    ui( new Ui::LocaleModule ),
    enabledLocalesModel_( new EnabledLocalesModel ),
    m_languageListViewDelegate( new LanguageListViewDelegate )
{
    Q_INIT_RESOURCE( translations );
    QTranslator appTranslator;
    appTranslator.load( ":/translations/msm_" + QLocale::system().name() );
    qApp->installTranslator( &appTranslator );

    KAboutData* aboutData = new KAboutData( "msm_locale",
                                            tr( "Locale Settings", "@title" ),
                                            PROJECT_VERSION,
                                            QStringLiteral( "" ),
                                            KAboutLicense::LicenseKey::GPL_V3,
                                            "Copyright 2014-2015 Ramon Buldó" );
    aboutData->addAuthor( "Ramon Buldó",
                          tr( "Author", "@info:credit" ),
                          QStringLiteral( "ramon@manjaro.org" ) );
    aboutData->addAuthor( "Roland Singer",
                          tr( "Author", "@info:credit" ),
                          QStringLiteral( "roland@manjaro.org" ) );
    setAboutData( aboutData );
    setButtons( KCModule::Default | KCModule::Apply );

    ui->setupUi( this );

    // Set Models
    ui->localeListView->setModel( enabledLocalesModel_ );
    ui->addressComboBox->setModel( enabledLocalesModel_ );
    ui->collateComboBox->setModel( enabledLocalesModel_ );
    ui->ctypeComboBox->setModel( enabledLocalesModel_ );
    ui->identificationComboBox->setModel( enabledLocalesModel_ );
    ui->langComboBox->setModel( enabledLocalesModel_ );
    ui->measurementComboBox->setModel( enabledLocalesModel_ );
    ui->messagesComboBox->setModel( enabledLocalesModel_ );
    ui->monetaryComboBox->setModel( enabledLocalesModel_ );
    ui->nameComboBox->setModel( enabledLocalesModel_ );
    ui->numericComboBox->setModel( enabledLocalesModel_ );
    ui->paperComboBox->setModel( enabledLocalesModel_ );
    ui->telephoneComboBox->setModel( enabledLocalesModel_ );
    ui->timeComboBox->setModel( enabledLocalesModel_ );

    // Set localeListView delegate
    ui->localeListView->setItemDelegate( m_languageListViewDelegate );

    // Define QActions
    m_setRegionAndFormatsAction = new QAction( tr( "Set as default display language and format" ), ui->localeListView );
    m_setRegionAction = new QAction( tr( "Set as default display language" ), ui->localeListView );
    m_setFormatsAction = new QAction( tr( "Set as default format" ), ui->localeListView );

    // Connect top buttons signal/slot
    connect( ui->buttonRemove, &QPushButton::clicked,
             this, &LocaleModule::removeLocale );
    connect( ui->buttonRestore, &QPushButton::clicked,
             this, &LocaleModule::defaults );
    connect( ui->buttonAdd, &QPushButton::clicked,
             this, &LocaleModule::addLocale );

    // Connect "System Locales" tab signal/slots
    connect( ui->localeListView->selectionModel(), &QItemSelectionModel::currentRowChanged,
             this, &LocaleModule::disableRemoveButton );
    connect( ui->localeListView, &QListView::doubleClicked,
             [=] ( const QModelIndex &index )
    {
        enabledLocalesModel_->setLang( index );
        enabledLocalesModel_->setLanguage( index );
        enabledLocalesModel_->setCtype( index );
        enabledLocalesModel_->setCollate( index );
        enabledLocalesModel_->setMessages( index );
        enabledLocalesModel_->setAddress( index );
        enabledLocalesModel_->setIdentification( index );
        enabledLocalesModel_->setMeasurement( index );
        enabledLocalesModel_->setMonetary( index );
        enabledLocalesModel_->setName( index );
        enabledLocalesModel_->setNumeric( index );
        enabledLocalesModel_->setPaper( index );
        enabledLocalesModel_->setTelephone( index );
        enabledLocalesModel_->setTime( index );
    } );

    // Connect "Detailed Settings" tab signal/slots
    connect( ui->addressComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = enabledLocalesModel_->index( row, 0 );
        enabledLocalesModel_->setAddress( index );
    } );
    connect( ui->collateComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = enabledLocalesModel_->index( row, 0 );
        enabledLocalesModel_->setCollate( index );
    } );
    connect( ui->ctypeComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = enabledLocalesModel_->index( row, 0 );
        enabledLocalesModel_->setCtype( index );
    } );
    connect( ui->identificationComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = enabledLocalesModel_->index( row, 0 );
        enabledLocalesModel_->setIdentification( index );
    } );
    connect( ui->langComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = enabledLocalesModel_->index( row, 0 );
        enabledLocalesModel_->setLang( index );
    } );
    connect( ui->measurementComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = enabledLocalesModel_->index( row, 0 );
        enabledLocalesModel_->setMeasurement( index );
    } );
    connect( ui->messagesComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = enabledLocalesModel_->index( row, 0 );
        enabledLocalesModel_->setMessages( index );
    } );
    connect( ui->monetaryComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = enabledLocalesModel_->index( row, 0 );
        enabledLocalesModel_->setMonetary( index );
    } );
    connect( ui->nameComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = enabledLocalesModel_->index( row, 0 );
        enabledLocalesModel_->setName( index );
    } );
    connect( ui->numericComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = enabledLocalesModel_->index( row, 0 );
        enabledLocalesModel_->setNumeric( index );
    } );
    connect( ui->paperComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = enabledLocalesModel_->index( row, 0 );
        enabledLocalesModel_->setPaper( index );
    } );
    connect( ui->telephoneComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = enabledLocalesModel_->index( row, 0 );
        enabledLocalesModel_->setTelephone( index );
    } );
    connect( ui->timeComboBox, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
             [=] ( int row )
    {
        QModelIndex index = enabledLocalesModel_->index( row, 0 );
        enabledLocalesModel_->setTime( index );
    } );

    connect( enabledLocalesModel_, &QAbstractListModel::dataChanged,
             [=] ( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int>& roles )
    {
        Q_UNUSED( bottomRight );
        if ( roles.contains( EnabledLocalesModel::AddressRole ) )
        {
            ui->addressComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::CollateRole ) )
        {
            ui->collateComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::CtypeRole ) )
        {
            ui->ctypeComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::IdentificationRole ) )
        {
            ui->identificationComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::LangRole ) )
        {
            ui->langComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::MeasurementRole ) )
        {
            ui->measurementComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::MonetaryRole ) )
        {
            ui->monetaryComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::MessagesRole ) )
        {
            ui->messagesComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::NameRole ) )
        {
            ui->nameComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::NumericRole ) )
        {
            ui->numericComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::PaperRole ) )
        {
            ui->paperComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::TelephoneRole ) )
        {
            ui->telephoneComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
        else if ( roles.contains( EnabledLocalesModel::TimeRole ) )
        {
            ui->timeComboBox->setCurrentIndex( topLeft.row() );
            m_isSystemLocalesModified = true;
            emit changed();
        }
    } );

    // Context menu for the list view in "System Locales" tab
    connect( ui->localeListView, &QListView::customContextMenuRequested,
             [=] ( const QPoint &pos )
    {
        QMenu localeListViewMenu;
        localeListViewMenu.addAction( m_setRegionAndFormatsAction );
        localeListViewMenu.addAction( m_setRegionAction );
        localeListViewMenu.addAction( m_setFormatsAction );
        localeListViewMenu.exec( ui->localeListView->mapToGlobal( pos ) );
    } );

    connect( m_setRegionAndFormatsAction, &QAction::triggered,
             [=]
    {
        QModelIndex index = ui->localeListView->currentIndex();
        enabledLocalesModel_->setLang( index );
        enabledLocalesModel_->setLanguage( index );
        enabledLocalesModel_->setCtype( index );
        enabledLocalesModel_->setCollate( index );
        enabledLocalesModel_->setMessages( index );
        enabledLocalesModel_->setAddress( index );
        enabledLocalesModel_->setIdentification( index );
        enabledLocalesModel_->setMeasurement( index );
        enabledLocalesModel_->setMonetary( index );
        enabledLocalesModel_->setName( index );
        enabledLocalesModel_->setNumeric( index );
        enabledLocalesModel_->setPaper( index );
        enabledLocalesModel_->setTelephone( index );
        enabledLocalesModel_->setTime( index );
    } );

    connect( m_setRegionAction, &QAction::triggered,
             [=]
    {
        QModelIndex index = ui->localeListView->currentIndex();
        enabledLocalesModel_->setLang( index );
        enabledLocalesModel_->setLanguage( index );
        enabledLocalesModel_->setCtype( index );
        enabledLocalesModel_->setCollate( index );
        enabledLocalesModel_->setMessages( index );
    } );

    connect( m_setFormatsAction, &QAction::triggered,
             [=]
    {
        QModelIndex index = ui->localeListView->currentIndex();
        enabledLocalesModel_->setAddress( index );
        enabledLocalesModel_->setIdentification( index );
        enabledLocalesModel_->setMeasurement( index );
        enabledLocalesModel_->setMonetary( index );
        enabledLocalesModel_->setName( index );
        enabledLocalesModel_->setNumeric( index );
        enabledLocalesModel_->setPaper( index );
        enabledLocalesModel_->setTelephone( index );
        enabledLocalesModel_->setTime( index );
    } );
}


LocaleModule::~LocaleModule()
{
    delete ui;
    delete enabledLocalesModel_;
    delete m_languageListViewDelegate;
    delete m_setRegionAndFormatsAction;
    delete m_setRegionAction;
    delete m_setFormatsAction;
}


void
LocaleModule::load()
{
    ui->buttonRemove->setDisabled( true );
    enabledLocalesModel_->init();
    enabledLocalesModel_->updateSystemLocales();
    m_isLocaleListModified = false;
    m_isSystemLocalesModified = false;
}


void
LocaleModule::save()
{
    if ( m_isLocaleListModified || m_isSystemLocalesModified )
    {

        QVariantMap args;
        args["isLocaleListModified"] = m_isLocaleListModified;
        args["isSystemLocalesModified"] = m_isSystemLocalesModified;
        args["locales"] = enabledLocalesModel_->locales();

        QStringList localeList;
        if ( !enabledLocalesModel_->lang().isEmpty() )
            localeList << QString( "LANG=%1" ).arg( enabledLocalesModel_->lang() );
        if ( !enabledLocalesModel_->language().isEmpty() )
            localeList << QString( "LANGUAGE=%1" ).arg( enabledLocalesModel_->language() );
        if ( !enabledLocalesModel_->ctype().isEmpty() )
            localeList << QString( "LC_CTYPE=%1" ).arg( enabledLocalesModel_->ctype() );
        if ( !enabledLocalesModel_->numeric().isEmpty() )
            localeList << QString( "LC_NUMERIC=%1" ).arg( enabledLocalesModel_->numeric() );
        if ( !enabledLocalesModel_->time().isEmpty() )
            localeList << QString( "LC_TIME=%1" ).arg( enabledLocalesModel_->time() );
        if ( !enabledLocalesModel_->collate().isEmpty() )
            localeList << QString( "LC_COLLATE=%1" ).arg( enabledLocalesModel_->collate() );
        if ( !enabledLocalesModel_->monetary().isEmpty() )
            localeList << QString( "LC_MONETARY=%1" ).arg( enabledLocalesModel_->monetary() );
        if ( !enabledLocalesModel_->messages().isEmpty() )
            localeList << QString( "LC_MESSAGES=%1" ).arg( enabledLocalesModel_->messages() );
        if ( !enabledLocalesModel_->paper().isEmpty() )
            localeList << QString( "LC_PAPER=%1" ).arg( enabledLocalesModel_->paper() );
        if ( !enabledLocalesModel_->name().isEmpty() )
            localeList << QString( "LC_NAME=%1" ).arg( enabledLocalesModel_->name() );
        if ( !enabledLocalesModel_->address().isEmpty() )
            localeList << QString( "LC_ADDRESS=%1" ).arg( enabledLocalesModel_->address() );
        if ( !enabledLocalesModel_->telephone().isEmpty() )
            localeList << QString( "LC_TELEPHONE=%1" ).arg( enabledLocalesModel_->telephone() );
        if ( !enabledLocalesModel_->measurement().isEmpty() )
            localeList << QString( "LC_MEASUREMENT=%1" ).arg( enabledLocalesModel_->measurement() );
        if ( !enabledLocalesModel_->identification().isEmpty() )
            localeList << QString( "LC_IDENTIFICATION=%1" ).arg( enabledLocalesModel_->identification() );

        args["localeList"] = localeList;

        // TODO: Progress UI
        KAuth::Action installAction( QLatin1String( "org.manjaro.msm.locale.save" ) );
        installAction.setHelperId( QLatin1String( "org.manjaro.msm.locale" ) );
        installAction.setArguments( args );
        KAuth::ExecuteJob* job = installAction.execute();
        connect( job, &KAuth::ExecuteJob::newData,
                 [=] ( const QVariantMap &data )
        {
            qDebug() << data;
        } );
        if ( job->exec() )
        {
            // tr("You might have to restart the graphical environment to apply the new settings...")
            qDebug() << "Job Succesfull";
        }
        else
        {
            // QString(tr("Failed to set locale!")
            qDebug() << "Job Failed";
        }

        load();
    }
}


void
LocaleModule::defaults()
{
    this->load();
}


void
LocaleModule::addLocale()
{
    SelectLocalesDialog dialog( this );
    dialog.exec();

    if ( !dialog.localeAdded() )
        return;

    QString localeCode = dialog.getLocale();
    if ( enabledLocalesModel_->insertLocale( enabledLocalesModel_->rowCount( QModelIndex() ), 1, localeCode ) )
    {
        m_isLocaleListModified = true;
        emit changed();
    }
}


void
LocaleModule::removeLocale()
{
    QModelIndex localeCurrentIndex = ui->localeListView->currentIndex();
    if ( localeCurrentIndex.isValid() )
    {
        if ( enabledLocalesModel_->removeLocale( localeCurrentIndex.row(), 1 ) )
        {
            ui->localeListView->setCurrentIndex( QModelIndex() );
            m_isLocaleListModified = true;
            emit changed();
        }
    }
}


/*
 * Disables the remove button if only one locale is in the list or no locale is selected
 */
void
LocaleModule::disableRemoveButton( const QModelIndex& current, const QModelIndex& previous )
{
    if ( enabledLocalesModel_->rowCount( QModelIndex() ) == 1 )
        ui->buttonRemove->setDisabled( true );
    else
    {
        if ( current.isValid() && !previous.isValid() )
            ui->buttonRemove->setDisabled( false );
        else if ( !current.isValid() && previous.isValid() )
            ui->buttonRemove->setDisabled( true );
    }
}

#include "LocaleModule.moc"
