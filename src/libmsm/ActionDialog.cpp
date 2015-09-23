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

#include "ActionDialog.h"

#include <KAuth/KAuthExecuteJob>

#include <QtCore/QRegularExpression>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

#include <QDebug>

ActionDialog::ActionDialog( QWidget* parent ) :
    QDialog( parent )
{
    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    this->setLayout( vBoxLayout );
    this->resize( 600,400 );

    m_messageLabel = new QLabel();
    vBoxLayout->addWidget( m_messageLabel );
    m_messageLabel->setText( tr( "Do you really want to continue?" ) );

    m_informationLabel = new QLabel();
    vBoxLayout->addWidget( m_informationLabel );
    m_informationLabel->setVisible( false );

    m_terminal = new QTextEdit();
    vBoxLayout->addWidget( m_terminal );
    m_terminal->setReadOnly( true );

    m_buttonBox = new QDialogButtonBox();
    vBoxLayout->addWidget( m_buttonBox );
    m_buttonBox->setOrientation( Qt::Horizontal );
    m_buttonBox->setStandardButtons( QDialogButtonBox::Yes | QDialogButtonBox::Cancel );

    QDialogButtonBox::connect( m_buttonBox, &QDialogButtonBox::accepted,
                               this, &ActionDialog::startJob );
    QDialogButtonBox::connect( m_buttonBox, &QDialogButtonBox::rejected,
                               this, &ActionDialog::reject );
}


void
ActionDialog::startJob()
{
    m_buttonBox->setStandardButtons( QDialogButtonBox::Cancel );

    KAuth::ExecuteJob* job = m_installAction.execute();
    connect( job, &KAuth::ExecuteJob::newData,
             [=] ( const QVariantMap &data )
    {
        QString output = data.value( "Data" ).toString();
        m_terminal->append( output.remove( QRegularExpression( "\x1b[^m]*m" ) ) );
    } );
    if ( job->exec() )
        m_jobSuccesful = true;
    else
        m_jobSuccesful = false;
    m_terminal->append( QString( "\n" ) );
    m_terminal->append( QString( tr( "Done ..." ) ) );
    m_buttonBox->setStandardButtons( QDialogButtonBox::Close );
}


bool
ActionDialog::isJobSuccesful() const
{
    return m_jobSuccesful;
}


KAuth::Action
ActionDialog::installAction() const
{
    return m_installAction;
}


void
ActionDialog::setInstallAction( const KAuth::Action& installAction )
{
    m_installAction = installAction;
}


QString
ActionDialog::message() const
{
    return m_message;
}


void
ActionDialog::setMessage( const QString& message )
{
    m_message = message;
    m_messageLabel->setText( message );
}


QString
ActionDialog::information() const
{
    return m_information;
}


void
ActionDialog::setInformation( const QString& information )
{
    m_information = information;
    m_informationLabel->setText( information );
    m_informationLabel->setVisible( true );
}
