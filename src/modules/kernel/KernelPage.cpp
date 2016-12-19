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
#include "KernelPage.h"
#include "ui_PageKernel.h"
#include "KernelListViewDelegate.h"

#include <QtCore/QProcess>
#include <QtWidgets/QMessageBox>


KernelPage::KernelPage( QWidget* parent ) :
    PageWidget( parent ),
    ui( new Ui::PageKernel ),
    m_kernelModel( new KernelModel )
{
    ui->setupUi( this );
    setTitle( KernelCommon::getTitle() );
    setIcon( QPixmap( ":/images/resources/tux-manjaro.png" ) );
    setName( KernelCommon::getName() );

    KernelSortFilterProxyModel* proxyKernelModel = new KernelSortFilterProxyModel( this );
    proxyKernelModel->setSourceModel( m_kernelModel );
    proxyKernelModel->setSortRole( KernelModel::VersionRole );
    proxyKernelModel->sort( 0, Qt::DescendingOrder );
    ui->kernelListView->setModel( proxyKernelModel );

    KernelListViewDelegate* kernelListViewDelegate = new KernelListViewDelegate;
    ui->kernelListView->setItemDelegate( kernelListViewDelegate );
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

KernelPage::~KernelPage()
{
    delete ui;
    delete m_kernelModel;
}


void
KernelPage::load()
{
    KernelCommon::load( m_kernelModel );
}
