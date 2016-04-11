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

#include "KernelModule.h"
#include "ui_PageKernel.h"
#include "KernelListViewDelegate.h"

#include <KAboutData>
#include <KAuth>
#include <KAuthAction>

#include <QtCore/QProcess>
#include <QtCore/QSettings>

#include <QDebug>
#include <QTranslator>

#include <KPluginFactory>
K_PLUGIN_FACTORY( MsmKernelFactory,
                  registerPlugin<PageKernel>( "msm_kernel" ); )

PageKernel::PageKernel( QWidget* parent, const QVariantList& args ) :
    KCModule( parent, args ),
    ui( new Ui::PageKernel ),
    m_kernelModel( new KernelModel ),
    m_kernelInfoDialog( new KernelInfoDialog )
{
    Q_INIT_RESOURCE( translations );
    QTranslator appTranslator;
    appTranslator.load( ":/translations/msm_" + QLocale::system().name() );
    qApp->installTranslator( &appTranslator );

    KAboutData* aboutData = new KAboutData( "msm_kernel",
                                            tr( "Kernel", "@title" ),
                                            PROJECT_VERSION,
                                            QStringLiteral( "" ),
                                            KAboutLicense::LicenseKey::GPL_V3,
                                            "Copyright 2014 Ramon Buldó", "@info:credit" );
    aboutData->addAuthor( "Ramon Buldó",
                          tr( "Author", "@info:credit" ),
                          QStringLiteral( "ramon@manjaro.org" ) );
    setAboutData( aboutData );
    setButtons( KCModule::NoAdditionalButton );

    ui->setupUi( this );

    // Setup ListView
    KernelSortFilterProxyModel* proxyKernelModel = new KernelSortFilterProxyModel( this );
    proxyKernelModel->setSourceModel( m_kernelModel );
    proxyKernelModel->setSortRole( KernelModel::VersionRole );
    proxyKernelModel->sort( 0, Qt::DescendingOrder );
    ui->kernelListView->setModel( proxyKernelModel );

    KernelListViewDelegate* kernelListViewDelegate = new KernelListViewDelegate;
    ui->kernelListView->setItemDelegate( kernelListViewDelegate );

    // Connect kernel tab slots
    connect( kernelListViewDelegate, &KernelListViewDelegate::installButtonClicked,
             this, &PageKernel::installButtonClicked );
    connect( kernelListViewDelegate, &KernelListViewDelegate::infoButtonClicked,
             this, &PageKernel::infoButtonClicked );

    // Connect notifications tab slots
    connect( ui->unsupportedKernelCheckBox, &QCheckBox::stateChanged,
             [=] ( int checkState )
    {
        QSettings settings( "manjaro", "manjaro-settings-manager" );
        settings.setValue( "notifications/checkUnsupportedKernel",
                           ui->unsupportedKernelCheckBox->isChecked() );
        settings.sync();

        switch ( checkState )
        {
        case Qt::Unchecked:
            ui->unsupportedKernelRunningCheckBox->setEnabled( false );
            break;
        case Qt::Checked:
            ui->unsupportedKernelRunningCheckBox->setEnabled( true );
        }
    } );

    connect( ui->newKernelCheckBox, &QCheckBox::stateChanged,
             [=] ( int checkState )
    {
        QSettings settings( "manjaro", "manjaro-settings-manager" );
        settings.setValue( "notifications/checkNewKernel",
                           ui->newKernelCheckBox->isChecked() );
        settings.sync();

        switch ( checkState )
        {
        case Qt::Unchecked:
            ui->newKernelLtsCheckBox->setEnabled( false );
            ui->newKernelRecommendedCheckBox->setEnabled( false );
            break;
        case Qt::Checked:
            ui->newKernelLtsCheckBox->setEnabled( true );
            ui->newKernelRecommendedCheckBox->setEnabled( true );
        }
    } );

    connect( ui->unsupportedKernelRunningCheckBox, &QCheckBox::stateChanged,
             [=] ()
    {
        QSettings settings( "manjaro", "manjaro-settings-manager" );
        settings.setValue( "notifications/checkUnsupportedKernelRunning",
                           ui->unsupportedKernelRunningCheckBox->isChecked() );
        settings.sync();
    } );

    connect( ui->newKernelLtsCheckBox, &QCheckBox::stateChanged,
             [=] ()
    {
        QSettings settings( "manjaro", "manjaro-settings-manager" );
        settings.setValue( "notifications/checkNewKernelLts",
                           ui->newKernelLtsCheckBox->isChecked() );
        settings.sync();
    } );

    connect( ui->newKernelRecommendedCheckBox, &QCheckBox::stateChanged,
             [=] ()
    {
        QSettings settings( "manjaro", "manjaro-settings-manager" );
        settings.setValue( "notifications/checkNewKernelRecommended",
                           ui->newKernelRecommendedCheckBox->isChecked() );
        settings.sync();
    } );
}


PageKernel::~PageKernel()
{
    delete ui;
    delete m_kernelModel;
}


void
PageKernel::load()
{
    m_kernelModel->update();

    QSettings settings( "manjaro", "manjaro-settings-manager" );
    bool checkUnsupportedKernel = settings.value( "notifications/checkUnsupportedKernel", true ).toBool();
    bool checkUnsupportedKernelRunning = settings.value( "notifications/checkUnsupportedKernelRunning", false ).toBool();
    bool checkNewKernel = settings.value( "notifications/checkNewKernel", true ).toBool();
    bool checkNewKernelLts = settings.value( "notifications/checkNewKernelLts", false ).toBool();
    bool checkNewKernelRecommended = settings.value( "notifications/checkNewKernelRecommended", true ).toBool();

    ui->unsupportedKernelCheckBox->setChecked( checkUnsupportedKernel );
    ui->unsupportedKernelRunningCheckBox->setChecked( checkUnsupportedKernelRunning );
    ui->newKernelCheckBox->setChecked( checkNewKernel );
    ui->newKernelLtsCheckBox->setChecked( checkNewKernelLts );
    ui->newKernelRecommendedCheckBox->setChecked( checkNewKernelRecommended );

    if ( !checkUnsupportedKernel )
        ui->unsupportedKernelRunningCheckBox->setEnabled( false );
    if ( !checkNewKernel )
    {
        ui->newKernelLtsCheckBox->setEnabled( false );
        ui->newKernelRecommendedCheckBox->setEnabled( false );
    }
}


void
PageKernel::save()
{
}


void
PageKernel::defaults()
{
    ui->unsupportedKernelCheckBox->setChecked( true );
    ui->unsupportedKernelRunningCheckBox->setChecked( false );
    ui->newKernelCheckBox->setChecked( true );
    ui->newKernelLtsCheckBox->setChecked( false );
    ui->newKernelRecommendedCheckBox->setChecked( true );
}


void
PageKernel::installButtonClicked( const QModelIndex& index )
{
    bool isInstalled = qvariant_cast<bool>( index.data( KernelModel::IsInstalledRole ) );
    if ( isInstalled )
        removeKernel( index );
    else
        installKernel( index );
}


void
PageKernel::installKernel( const QModelIndex& index )
{
    QStringList modules = qvariant_cast<QStringList>( index.data( KernelModel::InstalledModulesRole ) );
    QString kernel = qvariant_cast<QString>( index.data( KernelModel::PackageRole ) );
    QString version = qvariant_cast<QString>( index.data( KernelModel::VersionRole ) );
    QStringList packageList;
    packageList << kernel << modules;

    QString title = QString( tr( "Install Linux %1" ) ).arg( version );
    QString message = QString( tr( "The following packages will be installed:" ) );
    QString information = QString();
    for ( QString p : packageList )
    {
        information.append( p );
        information.append( "\n" );
    }

    QStringList arguments;
    arguments << "--noconfirm" << "--noprogressbar" << "-S" << packageList;
    QVariantMap args;
    args["arguments"] = arguments;
    KAuth::Action installAction( QLatin1String( "org.manjaro.msm.kernel.install" ) );
    installAction.setHelperId( QLatin1String( "org.manjaro.msm.kernel" ) );
    installAction.setArguments( args );

    ActionDialog actionDialog;
    actionDialog.setInstallAction( installAction );
    actionDialog.setWindowTitle( title );
    actionDialog.setMessage( message );
    actionDialog.setInformation( information );
    actionDialog.exec();
    if ( actionDialog.isJobSuccesful() )
        m_kernelModel->update();
}


void
PageKernel::removeKernel( const QModelIndex& index )
{
    QStringList modules = qvariant_cast<QStringList>( index.data( KernelModel::InstalledModulesRole ) );
    QString kernel = qvariant_cast<QString>( index.data( KernelModel::PackageRole ) );
    QString version = qvariant_cast<QString>( index.data( KernelModel::VersionRole ) );
    QStringList packageList;
    packageList << kernel << modules;

    QString title = QString( tr( "Remove Linux %1" ) ).arg( version );
    QString message = QString( tr( "The following packages will be removed:" ) );
    QString information = QString();
    for ( QString p : packageList )
    {
        information.append( p );
        information.append( "\n" );
    }

    QStringList arguments;
    arguments << "--noconfirm" << "--noprogressbar" << "-R" << packageList;
    QVariantMap args;
    args["arguments"] = arguments;
    KAuth::Action installAction( QLatin1String( "org.manjaro.msm.kernel.remove" ) );
    installAction.setHelperId( QLatin1String( "org.manjaro.msm.kernel" ) );
    installAction.setArguments( args );

    ActionDialog actionDialog;
    actionDialog.setInstallAction( installAction );
    actionDialog.setWindowTitle( title );
    actionDialog.setMessage( message );
    actionDialog.setInformation( information );
    actionDialog.exec();
    if ( actionDialog.isJobSuccesful() )
        m_kernelModel->update();
}


void
PageKernel::infoButtonClicked( const QModelIndex& index )
{
    QString package = qvariant_cast<QString>( index.data( KernelModel::PackageRole ) );
    QString majorVersion = qvariant_cast<QString>( index.data( KernelModel::MajorVersionRole ) );
    QString minorVersion = qvariant_cast<QString>( index.data( KernelModel::MinorVersionRole ) );
    QString title = QString( tr( "Linux %1.%2 changelog" ) ).arg( majorVersion, minorVersion );
    m_kernelInfoDialog->setWindowTitle( title );
    m_kernelInfoDialog->setPackage( package );
    m_kernelInfoDialog->exec();
}

#include "KernelModule.moc"
