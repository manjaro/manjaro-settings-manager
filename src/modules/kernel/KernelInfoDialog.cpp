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

#include "KernelInfoDialog.h"

#include <QtCore/QFile>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QVBoxLayout>

KernelInfoDialog::KernelInfoDialog( QWidget* parent ) :
    QDialog( parent )
{
    this->resize( 800, 600 );

    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    this->setLayout( vBoxLayout );
    m_textBrowser = new QTextBrowser();
    vBoxLayout->addWidget( m_textBrowser );
    m_textBrowser->setOpenExternalLinks( true );

    QDialogButtonBox* buttonBox = new QDialogButtonBox();
    vBoxLayout->addWidget( buttonBox );
    buttonBox->setOrientation( Qt::Horizontal );
    buttonBox->setStandardButtons( QDialogButtonBox::Close );

    QDialogButtonBox::connect( buttonBox, &QDialogButtonBox::accepted,
                               this, &KernelInfoDialog::accept );
    QDialogButtonBox::connect( buttonBox, &QDialogButtonBox::rejected,
                               this, &KernelInfoDialog::reject );
}


KernelInfoDialog::~KernelInfoDialog()
{
}


void
KernelInfoDialog::setPackage( const QString& package )
{
    m_package = package;
}


int
KernelInfoDialog::exec()
{
    QUrl kernelLogUrl( QString( "qrc:///changelogs/%1.html" ).arg( m_package ) );
    m_textBrowser->setSource( kernelLogUrl );
    m_textBrowser->show();
    return QDialog::exec();
}
