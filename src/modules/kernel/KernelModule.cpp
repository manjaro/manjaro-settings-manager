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

#include "KernelListViewDelegate.h"

#include <KAboutData>
#include <KAuth>
#include <KAuthAction>

#include <QtCore/QProcess>
#include <QtCore/QSettings>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QListView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>

#include <QDebug>

#include <KPluginFactory>
K_PLUGIN_FACTORY( MsmKernelFactory,
                  registerPlugin<PageKernel>( "msm_kernel" ); )

PageKernel::PageKernel( QWidget* parent, const QVariantList& args ) :
    KCModule( parent, args ),
    m_kernelModel( new KernelModel ),
    m_kernelInfoDialog( new KernelInfoDialog )
{
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

    QHBoxLayout* hBoxLayout = new QHBoxLayout();
    this->setLayout( hBoxLayout );

    QTabWidget* tabWidget = new QTabWidget();
    hBoxLayout->addWidget( tabWidget );

    // Setup Kernel tab
    QListView* kernelListView = new QListView();
    kernelListView->setAlternatingRowColors( true );

    KernelSortFilterProxyModel* proxyKernelModel = new KernelSortFilterProxyModel( this );
    proxyKernelModel->setSourceModel( m_kernelModel );
    proxyKernelModel->setSortRole( KernelModel::VersionRole );
    proxyKernelModel->sort( 0, Qt::DescendingOrder );
    kernelListView->setModel( proxyKernelModel );

    KernelListViewDelegate* kernelListViewDelegate = new KernelListViewDelegate;
    kernelListView->setItemDelegate( kernelListViewDelegate );

    // Setup options tab
    QWidget* optionsWidget = new QWidget();
    QVBoxLayout* optionsVBox = new QVBoxLayout();
    optionsWidget->setLayout( optionsVBox );

    QGroupBox* groupBox = new QGroupBox( tr( "Notifications" ) );
    optionsVBox->addWidget( groupBox );
    optionsVBox->addStretch( 1 );

    m_checkUnsupportedKernelBox = new QCheckBox( tr( "Check unsupported kernels" ) );

    m_checkUnsupportedKernelRunningBox = new QCheckBox( tr( "Only if running an unsupported kernel" ) );
    QHBoxLayout* hBox1 = new QHBoxLayout();
    hBox1->addSpacing( 40 );
    hBox1->addWidget( m_checkUnsupportedKernelRunningBox );

    m_checkNewKernelBox = new QCheckBox( tr( "Check new kernels" ) );

    m_checkNewKernelLtsBox = new QCheckBox( tr( "Only LTS kernels" ) );
    QHBoxLayout* hBox2 = new QHBoxLayout();
    hBox2->addSpacing( 40 );
    hBox2->addWidget( m_checkNewKernelLtsBox );

    m_checkNewKernelRecommendedBox = new QCheckBox( tr( "Only recommended kernels" ) );
    QHBoxLayout* hBox3 = new QHBoxLayout();
    hBox3->addSpacing( 40 );
    hBox3->addWidget( m_checkNewKernelRecommendedBox );

    QVBoxLayout* groupVBox = new QVBoxLayout();
    groupVBox->addWidget( m_checkUnsupportedKernelBox );
    groupVBox->addLayout( hBox1 );
    groupVBox->addWidget( m_checkNewKernelBox );
    groupVBox->addLayout( hBox2 );
    groupVBox->addLayout( hBox3 );

    groupBox->setLayout( groupVBox );

    // Add tabs
    tabWidget->addTab( kernelListView, tr( "Kernels" ) );
    tabWidget->addTab( optionsWidget, tr( "Notifications" ) );

    // Connect kernel tab slots
    connect( kernelListViewDelegate, &KernelListViewDelegate::installButtonClicked,
             this, &PageKernel::installButtonClicked );
    connect( kernelListViewDelegate, &KernelListViewDelegate::infoButtonClicked,
             this, &PageKernel::infoButtonClicked );

    // Connect notifications tab slots
    connect( m_checkUnsupportedKernelBox, &QCheckBox::stateChanged,
             [=] ( int checkState )
    {
        QSettings settings( "manjaro", "manjaro-settings-manager" );
        settings.setValue( "notifications/checkUnsupportedKernel",
                           m_checkUnsupportedKernelBox->isChecked() );
        settings.sync();

        switch ( checkState )
        {
        case Qt::Unchecked:
            m_checkUnsupportedKernelRunningBox->setEnabled( false );
            break;
        case Qt::Checked:
            m_checkUnsupportedKernelRunningBox->setEnabled( true );
        }
    } );

    connect( m_checkNewKernelBox, &QCheckBox::stateChanged,
             [=] ( int checkState )
    {
        QSettings settings( "manjaro", "manjaro-settings-manager" );
        settings.setValue( "notifications/checkNewKernel",
                           m_checkNewKernelBox->isChecked() );
        settings.sync();

        switch ( checkState )
        {
        case Qt::Unchecked:
            m_checkNewKernelLtsBox->setEnabled( false );
            m_checkNewKernelRecommendedBox->setEnabled( false );
            break;
        case Qt::Checked:
            m_checkNewKernelLtsBox->setEnabled( true );
            m_checkNewKernelRecommendedBox->setEnabled( true );
        }
    } );

    connect( m_checkUnsupportedKernelRunningBox, &QCheckBox::stateChanged,
             [=] ()
    {
        QSettings settings( "manjaro", "manjaro-settings-manager" );
        settings.setValue( "notifications/checkUnsupportedKernelRunning",
                           m_checkUnsupportedKernelRunningBox->isChecked() );
        settings.sync();
    } );

    connect( m_checkNewKernelLtsBox, &QCheckBox::stateChanged,
             [=] ()
    {
        QSettings settings( "manjaro", "manjaro-settings-manager" );
        settings.setValue( "notifications/checkNewKernelLts",
                           m_checkNewKernelLtsBox->isChecked() );
        settings.sync();
    } );

    connect( m_checkNewKernelRecommendedBox, &QCheckBox::stateChanged,
             [=] ()
    {
        QSettings settings( "manjaro", "manjaro-settings-manager" );
        settings.setValue( "notifications/checkNewKernelRecommended",
                           m_checkNewKernelRecommendedBox->isChecked() );
        settings.sync();
    } );
}


PageKernel::~PageKernel()
{
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

    m_checkUnsupportedKernelBox->setChecked( checkUnsupportedKernel );
    m_checkUnsupportedKernelRunningBox->setChecked( checkUnsupportedKernelRunning );
    m_checkNewKernelBox->setChecked( checkNewKernel );
    m_checkNewKernelLtsBox->setChecked( checkNewKernelLts );
    m_checkNewKernelRecommendedBox->setChecked( checkNewKernelRecommended );

    if ( !checkUnsupportedKernel )
        m_checkUnsupportedKernelRunningBox->setEnabled( false );
    if ( !checkNewKernel )
    {
        m_checkNewKernelLtsBox->setEnabled( false );
        m_checkNewKernelRecommendedBox->setEnabled( false );
    }
}


void
PageKernel::save()
{
}


void
PageKernel::defaults()
{
    m_checkUnsupportedKernelBox->setChecked( true );
    m_checkUnsupportedKernelRunningBox->setChecked( false );
    m_checkNewKernelBox->setChecked( true );
    m_checkNewKernelLtsBox->setChecked( false );
    m_checkNewKernelRecommendedBox->setChecked( true );
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
