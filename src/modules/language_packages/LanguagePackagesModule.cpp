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

#include "LanguagePackagesCommon.h"
#include "LanguagePackagesModule.h"
#include "MsmCommon.h"
#include "ui_PageLanguagePackages.h"

#include <KAboutData>
#include <KPluginFactory>

#include <QtCore/QDebug>
#include <QtCore/QTranslator>

K_PLUGIN_FACTORY( MsmLanguagePackagesFactory,
                  registerPlugin<LanguagePackagesModule>( LanguagePackagesCommon::getName() ); )

LanguagePackagesModule::LanguagePackagesModule( QWidget* parent, const QVariantList& args ) :
    KCModule( parent, args ),
    ui( new Ui::PageLanguagePackages )
{
    Q_INIT_RESOURCE( language_packages );
    Q_INIT_RESOURCE( translations );
    QTranslator* appTranslator = new QTranslator;
    appTranslator->load( ":/translations/msm_" + QLocale::system().name() );
    qApp->installTranslator( appTranslator );

    KAboutData* aboutData = new KAboutData( LanguagePackagesCommon::getName(),
                                            LanguagePackagesCommon::getTitle(),
                                            PROJECT_VERSION,
                                            LanguagePackagesCommon::getDescription(),
                                            KAboutLicense::LicenseKey::GPL_V3,
                                            "(c) 2014 - 2017 Manjaro Settings Manager developers" );
    aboutData->addAuthor( "Ramon Buldó",
                          QStringLiteral( "rbuldo@gmail.com" ) );
    aboutData->addAuthor( "Roland Singer",
                          QStringLiteral( "roland@manjaro.org" ) );
    aboutData->setCustomAuthorText( QString(), MsmCommon::getBugReportLink() );
    setAboutData( aboutData );
    setButtons( KCModule::NoAdditionalButton );

    ui->setupUi( this );

    ui->treeWidgetAvailable->setColumnWidth( 0, 250 );
    ui->treeWidgetAvailable->setColumnWidth( 1, 300 );
    ui->treeWidgetAvailable->setColumnWidth( 2, 30 );

    ui->treeWidgetInstalled->setColumnWidth( 0, 300 );
    ui->treeWidgetInstalled->setColumnWidth( 1, 300 );

    connect( ui->installPackagesButton, &QPushButton::clicked,
             [=] ( bool checked )
    {
        Q_UNUSED( checked )
        LanguagePackagesCommon::installPackages( ui );
        load();
    } );
}


LanguagePackagesModule::~LanguagePackagesModule()
{
    delete ui;
}


void
LanguagePackagesModule::load()
{
    LanguagePackagesCommon::load( ui );
}


void
LanguagePackagesModule::save()
{
}


void
LanguagePackagesModule::defaults()
{
}

#include "LanguagePackagesModule.moc"
