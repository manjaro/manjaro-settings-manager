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

#include "LanguageCommon.h"
#include "LanguagePackages.h"
#include "Notifier.h"
#include "NotifierApp.h"
#include "Kernel.h"
#include "KernelModel.h"
#include "PacmanUtils.h"

#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtCore/QSettings>

#include <QDebug>

Notifier::Notifier( QObject* parent ) :
    QObject( parent )
{


    m_tray = new QSystemTrayIcon( this );
    m_tray->setIcon( QIcon::fromTheme( "manjaro-settings-manager" ) );

    QMenu* menu = new QMenu();
    m_tray->setContextMenu( menu );
    QAction* msmKernel = new QAction( QIcon( ":/icons/tux-manjaro.png" ),
                                      QString( tr ( "Kernels" ) ),
                                      menu );
    QAction* msmLanguagePackages = new QAction(
        QIcon( ":/icons/language.png" ),
        QString( tr ( "Language packages" ) ),
        menu );
    QAction* quitAction = new QAction(
        QIcon::fromTheme( "application-exit"  ),
        QString( tr ( "Quit" ) ),
        menu );
    QAction* optionsAction = new QAction(
        QIcon::fromTheme( "gtk-preferences"  ),
        QString( tr ( "Options" ) ),
        menu );

    menu->addAction( msmKernel );
    menu->addAction( msmLanguagePackages );
    menu->addAction( optionsAction );
    menu->addSeparator();
    menu->addAction( quitAction );

    connect( msmKernel, &QAction::triggered, this, [msmKernel, this]()
    {
        QProcess::startDetached( "manjaro-settings-manager", QStringList() << "-m" << "msm_kernel" );
        m_tray->hide();
    } );
    connect( msmLanguagePackages, &QAction::triggered, this, [msmLanguagePackages, this]()
    {
        QProcess::startDetached( "manjaro-settings-manager", QStringList() << "-m" << "msm_language_packages" );
        m_tray->hide();
    } );

    connect( quitAction, &QAction::triggered, this, [quitAction, this]()
    {
        qApp->quit();
    } );

    connect( optionsAction, &QAction::triggered, this, [optionsAction, this]()
    {
        m_settingsDialog = new NotifierSettingsDialog(NULL);
        m_settingsDialog->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose, true);
        m_settingsDialog->exec();
    } );

    m_timer = new QTimer( this );
    // 1 min
    m_timer->setInterval( 60 * 1000 );
    m_timer->start();

    connect( m_timer, &QTimer::timeout, [=] ()
    {
        loadConfiguration();
        if ( !PacmanUtils::isPacmanUpdating() && PacmanUtils::hasPacmanEverSynced() )
        {
            if ( m_checkLanguagePackage )
                cLanguagePackage();

            if ( m_checkKernel )
                cKernel();

            // 12 hours
            m_timer->setInterval( 12 * 60 * 60 * 1000 );
        }
        else
        {
            // 30 minutes
            m_timer->setInterval( 30 * 60 * 1000 );
        }
    } );
}


Notifier::~Notifier()
{

}


void
Notifier::cLanguagePackage()
{
    int packageNumber = 0;
    QStringList packages;
    LanguagePackages languagePackages;
    QList<LanguagePackagesItem> lpiList { languagePackages.languagePackages() };
    QStringList locales { LanguageCommon::enabledLocales( true ) };
    foreach ( const QString locale, locales )
    {
        QStringList split = locale.split( "_", QString::SkipEmptyParts );
        if ( split.size() != 2 )
            continue;
        QByteArray language = QString( split.at( 0 ) ).toUtf8();
        QByteArray territory = QString( split.at( 1 ) ).toUtf8();

        foreach ( const auto item, lpiList )
        {
            if ( item.parentPkgInstalled().length() == 0 )
                continue;
            if ( !item.languagePackage().contains( "%" ) )
                continue;

            QList<QByteArray> checkPkgs;
            // Example: firefox-i18n-% -> firefox-i18n-en-US
            checkPkgs << item.languagePackage().replace( "%", language.toLower() + "-" + territory );
            // Example: firefox-i18n-% -> firefox-i18n-en-us
            checkPkgs << item.languagePackage().replace( "%", language.toLower() + "-" + territory.toLower() );
            // Example: firefox-i18n-% -> firefox-i18n-en_US
            checkPkgs << item.languagePackage().replace( "%", language.toLower() + "_" + territory );
            // Example: firefox-i18n-% -> firefox-i18n-en_us
            checkPkgs << item.languagePackage().replace( "%", language.toLower() + "_" + territory.toLower() );
            // Example: firefox-i18n-% -> firefox-i18n-en
            checkPkgs << item.languagePackage().replace( "%", language.toLower() );

            foreach ( const auto checkPkg, checkPkgs )
            {
                if ( item.languagePkgInstalled().contains( checkPkg ) )
                    continue;
                if ( item.languagePkgAvailable().contains( checkPkg ) )
                {
                    ++packageNumber;
                    if ( !isPackageIgnored( checkPkg, "language_package" ) )
                        packages << checkPkg;
                }
            }
        }
    }
    foreach ( const auto item, lpiList )
    {
        if ( item.parentPkgInstalled().length() == 0 )
            continue;
        if ( !item.languagePackage().contains( "%" )
                && !item.languagePkgInstalled().contains( item.languagePackage() ) )
        {
            qDebug() << item.languagePackage();
            ++packageNumber;
            if ( !isPackageIgnored( item.languagePackage(), "language_package" ) )
                packages << item.languagePackage();
        }
    }

    if ( !packages.isEmpty() )
    {
        qDebug() << "Missing language packages found, notifying user...";
        m_tray->show();
        m_tray->showMessage( tr( "Manjaro Settings Manager" ),
                             QString( tr( "%n new additional language package(s) available", "", packageNumber ) ),
                             QSystemTrayIcon::Information,
                             10000 );

        // Add to Config
        foreach ( const QString package, packages )
            addToConfig( package, "language_package" );
    }
}


void
Notifier::cKernel()
{
    KernelModel kernelModel;
    kernelModel.update();

    QList< Kernel > unsupportedKernels = kernelModel.unsupportedKernels();
    if ( m_checkUnsupportedKernel && !unsupportedKernels.isEmpty() )
    {
        bool foundRunning = false;
        bool found = false;
        foreach ( Kernel kernel, unsupportedKernels )
        {
            if ( isPackageIgnored( kernel.package(), "unsupported_kernel" ) )
                continue;

            if ( m_checkUnsupportedKernelRunning && kernel.isRunning() )
                foundRunning = true;
            else
                found = true;

            addToConfig( kernel.package(), "unsupported_kernel" );
        }

        if ( foundRunning )
        {
            m_tray->show();
            m_tray->showMessage( QString( tr( "Manjaro Settings Manager" ) ),
                                 QString( tr( "Running an unsupported kernel, please update." ) ),
                                 QSystemTrayIcon::Warning,
                                 10000 );
        }
        else if ( found )
        {
            m_tray->show();
            m_tray->showMessage( QString( tr( "Manjaro Settings Manager" ) ),
                                 QString( tr( "Unsupported kernel installed in your system, please remove it." ) ),
                                 QSystemTrayIcon::Information,
                                 10000 );
        }
    }

    QList<Kernel> newKernels = kernelModel.newerKernels( kernelModel.latestInstalledKernel() );
    QList<Kernel> newLtsRecommendedKernels;
    QList<Kernel> newLtsKernels;
    QList<Kernel> newRecommendedKernels;
    QList<Kernel> newNotIgnoredKernels;
    if ( m_checkNewKernel )
    {
        foreach ( Kernel kernel, newKernels )
        {
            if ( isPackageIgnored( kernel.package(), "new_kernel" ) )
                continue;

            newNotIgnoredKernels << kernel;
            if ( kernel.isRecommended() && kernel.isLts() )
            {
                newLtsRecommendedKernels << kernel;
                newLtsKernels << kernel;
                newRecommendedKernels << kernel;
            }
            else if ( kernel.isLts() )
                newLtsKernels << kernel;
            else if ( kernel.isRecommended() )
                newRecommendedKernels << kernel;

            addToConfig( kernel.package(), "new_kernel" );
        }

        if ( m_checkNewKernelLts && m_checkNewKernelRecommended && !newLtsRecommendedKernels.isEmpty() )
            showNewKernelNotification();
        else if ( m_checkNewKernelLts && !newLtsKernels.isEmpty() )
            showNewKernelNotification();
        else if ( m_checkNewKernelRecommended && !newRecommendedKernels.isEmpty() )
            showNewKernelNotification();
        else if ( !newNotIgnoredKernels.isEmpty() )
            showNewKernelNotification();
    }
}


void Notifier::showNewKernelNotification()
{
    m_tray->show();
    m_tray->showMessage( QString( tr( "Manjaro Settings Manager" ) ),
                         QString( tr( "Newer kernel is available, please update." ) ),
                         QSystemTrayIcon::Information,
                         10000 );
}


void
Notifier::loadConfiguration()
{
    QSettings settings( "manjaro", "manjaro-settings-manager" );
    m_checkLanguagePackage = settings.value( "notifications/checkLanguagePackages", true ).toBool();
    m_checkUnsupportedKernel = settings.value( "notifications/checkUnsupportedKernel", true ).toBool();
    m_checkUnsupportedKernelRunning = settings.value( "notifications/checkUnsupportedKernelRunning", false ).toBool();
    m_checkNewKernel = settings.value( "notifications/checkNewKernel", true ).toBool();
    m_checkNewKernelLts = settings.value( "notifications/checkNewKernelLts", false ).toBool();
    m_checkNewKernelRecommended = settings.value( "notifications/checkNewKernelRecommended", true ).toBool();
    m_checkKernel = m_checkUnsupportedKernel | m_checkNewKernel;
}


bool
Notifier::isPackageIgnored( const QString package, const QString group )
{
    QSettings settings( "manjaro", "manjaro-settings-manager-Notifier" );
    settings.beginGroup( group );
    int value = settings.value( "notify_count_" + package, "0" ).toInt();
    settings.endGroup();
    return ( value < 2 ) ? false : true;
}


void
Notifier::addToConfig( const QString package, const QString group )
{
    QSettings settings( "manjaro", "manjaro-settings-manager-Notifier" );
    settings.beginGroup( group );
    int value = settings.value( "notify_count_" + package, "0" ).toInt();
    ++value;
    if ( value < 3 )
        settings.setValue( "notify_count_" + package, value );
    settings.endGroup();
}
