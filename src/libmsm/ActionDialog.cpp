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

#include "ActionDialog.h"

#include <KAuth/KAuthExecuteJob>

#include <QtCore/QRegularExpression>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

#include <QDebug>

ActionDialog::ActionDialog( QWidget* parent ) :
    QDialog( parent )
{
    this->setWindowFlags( Qt::SubWindow | Qt::Dialog | Qt::WindowCloseButtonHint );
    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    this->setLayout( vBoxLayout );

    x = 400;
    y = 160;

    this->resize( x, y );
    window()->setFixedSize( x, y );

    m_messageLabel = new QLabel();
    vBoxLayout->addWidget( m_messageLabel );
    m_messageLabel->setText( tr( "Do you really want to continue?" ) );
    m_messageLabel->setWordWrap( true );

    m_progressBar = new QProgressBar();
    m_progressBar->setMaximum( 0 );
    m_progressBar->setMinimum( 0 );
    vBoxLayout->addWidget( m_progressBar );
    m_progressBar->hide();

    m_informationLabel = new QLabel();
    m_informationLabel->setFixedWidth( x );
    vBoxLayout->addWidget( m_informationLabel );
    QFont f = font();
    f.setItalic( true );
    m_informationLabel->setVisible( false );
    m_informationLabel->setFont( f );

    m_terminal = new QTextEdit();
    vBoxLayout->addWidget( m_terminal );
    m_terminal->setReadOnly( true );
    m_terminal->hide();

    m_showDetails = new QLabel();
    m_showDetails->setTextFormat( Qt::RichText );
    m_showDetails->setTextInteractionFlags( Qt::TextBrowserInteraction );
    vBoxLayout->addWidget( m_showDetails );
    QLabel::connect( m_showDetails, &QLabel::linkActivated,
                     this, &ActionDialog::showDetails );
    this->showDetails( "#hide" );

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
ActionDialog::writeToTerminal( const QString& information )
{
    m_terminal->append( information );
}

void
ActionDialog::showDetails( const QString& link )
{
    if ( link == "#hide" )
    {
        m_terminal -> hide();
        m_showDetails->setText( "<a href=\"#show\">" + tr( "Show Details" ) + "</a>" );
        m_informationLabel->show();
        window()->setFixedSize( x, y );
        this->resize( x, y );
    }
    else if ( link == "#show" )
    {
        m_terminal -> show();
        m_showDetails->setText( "<a href=\"#hide\">" + tr( "Hide Details" ) + "</a>" );
        m_informationLabel->hide();
        window()->setFixedSize( x, y + 100 );
        this->resize( x, y + 100 );
    }
}

void
ActionDialog::startJob()
{
    m_buttonBox->setStandardButtons( QDialogButtonBox::Close );
    m_buttonBox->setDisabled( true );
    m_progressBar->show();

    m_messageLabel->setText( tr ( "Please wait while your system is being modified" ) );
    m_informationLabel->setText( tr ( "Starting" ) );
    m_terminal->append( tr ( "Starting" ) );

    KAuth::ExecuteJob* kAuthJob = m_installAction.execute();

    connect( kAuthJob, &KAuth::ExecuteJob::newData,
             [=] ( const QVariantMap &data )
    {
        QString output = data.value( "Data" ).toString();
        foreach ( auto line, output.split( QRegExp( "[\r\n]" ),QString::SkipEmptyParts ) )
        {
            if ( line != m_lastMessage )
            {
                QString l = line.remove( QRegularExpression( "\x1b[^m]*m" ) );
                m_lastMessage = l;
                updateInfo ( l );
            }
        }

    } );

    connect( kAuthJob, &KAuth::ExecuteJob::result,
             [=] ( KJob *kjob )
    {
        auto job = qobject_cast<KAuth::ExecuteJob*>( kjob );
        if ( job->error() == 0 )
            jobDone( true, tr ( "Changes failed, click on 'Show Details' for more information" ) );
        else
            jobDone( false, tr ( "Changes failed, click on 'Show Details' for more information" ) );
    } );

    connect( kAuthJob, &KAuth::ExecuteJob::statusChanged,
             [=] ( KAuth::Action::AuthStatus status )
    {
//         switch (status) {
//             case KAuth::Action::AuthStatus::DeniedStatus : {
//             }
//         };
        // some error with switch

        if ( status == KAuth::Action::AuthStatus::DeniedStatus )
            jobDone ( false, tr ( "You are not authorised to make these changes" ) );
        else if ( status == KAuth::Action::AuthStatus::ErrorStatus )
            jobDone ( false, tr ( "Some Error occurred during authorization" ) );
        else if ( status == KAuth::Action::AuthStatus::InvalidStatus )
            jobDone ( false, tr ( "Invalid authorization status" ) );
        else if ( status == KAuth::Action::AuthStatus::UserCancelledStatus )
            jobDone ( false, tr ( "Authorization canceled by you" ) );
        else if ( status == KAuth::Action::AuthStatus::AuthorizedStatus )
            qDebug() << "All good";
    } );

    kAuthJob->start();
}

void
ActionDialog::jobDone ( bool success, QString message )
{
    if ( success )
    {
        m_jobSuccesful = true;
        if ( message != NULL )
            m_messageLabel->setText( tr ( "Changes were made successfully" ) );
        else
            m_messageLabel->setText( message );
    }
    else
    {
        m_jobSuccesful = false;
        if ( message != NULL )
            m_messageLabel->setText( tr ( "Changes failed, click on 'Show Details' for more information" ) );
        else
            m_messageLabel->setText( message );
    }

    m_terminal->append( QString( "\n" ) );
    m_terminal->append( QString( tr( "Done ..." ) ) );
    m_informationLabel->setText( QString( tr( "Done ..." ) ) );
    m_buttonBox->setEnabled( true );
    m_progressBar->setMaximum( 100 );
    m_progressBar->setValue( 100 );
}

bool
ActionDialog::isJobSuccesful() const
{
    return m_jobSuccesful;
}

void
ActionDialog::updateInfo( const QString& data )
{
    writeToTerminal( data );
    m_informationLabel->setText( data );
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
