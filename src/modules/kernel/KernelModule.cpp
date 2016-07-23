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
    QTranslator* appTranslator = new QTranslator;
    appTranslator->load( ":/translations/msm_" + QLocale::system().name() );
    qApp->installTranslator( appTranslator );

    KAboutData* aboutData = new KAboutData( "msm_kernel",
                                            tr( "Kernel", "@title" ),
                                            PROJECT_VERSION,
                                            QStringLiteral( "" ),
                                            KAboutLicense::LicenseKey::GPL_V3,
                                            "Copyright 2014 Ramon Buldó" );
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
}


void
PageKernel::save()
{
}


void
PageKernel::defaults()
{
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
    foreach ( const QString p, packageList )
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
    foreach ( const QString p, packageList )
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
