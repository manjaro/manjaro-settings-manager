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

#include "KernelCommon.h"
#include "KernelModule.h"
#include "ui_PageKernel.h"
#include "KernelListViewDelegate.h"

#include <limits>
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
    m_kernelModel( new KernelModel )
{
    Q_INIT_RESOURCE( translations );
    QTranslator* appTranslator = new QTranslator;
    appTranslator->load( ":/translations/msm_" + QLocale::system().name() );
    qApp->installTranslator( appTranslator );

    KAboutData* aboutData = new KAboutData( "msm_kernel",
                                            tr( "Kernel", "@title" ),
                                            PROJECT_VERSION,
                                            tr( "Add and remove kernels", "@comment" ),
                                            KAboutLicense::LicenseKey::GPL_V3,
                                            "(c) 2014 - 2017 Ramon Buldó" );
    aboutData->addAuthor( "Ramon Buldó",
                          tr( "Author", "@info:credit" ),
                          QStringLiteral( "ramon@manjaro.org" ) );
    aboutData->setCustomAuthorText( QString(),
                                    tr( "Please use <a href='%1'>%1</a> to report bugs." )
                                    .arg( "https://bugs.manjaro.org/" ) );
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
             [=] ( const QModelIndex& index )
    {
        KernelCommon::kernelAction( index );
        load();
    } );
    connect( kernelListViewDelegate, &KernelListViewDelegate::infoButtonClicked,
             [=] ( const QModelIndex& index )
    {
        KernelCommon::showChangelog( index );
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
    KernelCommon::load( m_kernelModel );
}


void
PageKernel::save()
{
}


void
PageKernel::defaults()
{
}

#include "KernelModule.moc"
