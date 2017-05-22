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
#include "ClickableLabel.h"
#include "Job.h"

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
    
    x = 400;
    y = 100;
    
    this->resize( x, y );

    m_messageLabel = new QLabel();
    vBoxLayout->addWidget( m_messageLabel );
    m_messageLabel->setText( tr( "Do you really want to continue?" ) );
    
    m_progressBar = new QProgressBar();
    m_progressBar->setMaximum(0);
    m_progressBar->setMinimum(0);
    vBoxLayout->addWidget(m_progressBar);
    m_progressBar->hide();

    m_informationLabel = new QLabel();
    vBoxLayout->addWidget( m_informationLabel );
    QFont f = font();
    f.setItalic(true);
    m_informationLabel->setVisible( false );
    m_informationLabel->setFont(f);

    m_terminal = new QTextEdit();
    vBoxLayout->addWidget( m_terminal );
    m_terminal->setReadOnly( true );
    m_terminal->hide();
    m_detailOn = false;

    m_showDetails = new ClickableLabel();
    vBoxLayout->addWidget( m_showDetails );
    m_showDetails->setText( tr( "Show Details" ));

    ClickableLabel::connect( m_showDetails, &ClickableLabel::clicked,
                             this, &ActionDialog::showDetails );

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
    m_terminal->append(information);
}

void
ActionDialog::showDetails()
{
    if (m_detailOn) 
    {
        m_detailOn = false;
        m_terminal -> hide();
        m_showDetails->setText( tr( "Show Details" ));
        this->resize( x, y );
        m_informationLabel->show();
    }
    else
    {
        m_detailOn = true;
        m_terminal -> show();
        m_showDetails->setText( tr( "Hide Details" ));
        m_informationLabel->hide();
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
    
    if (!m_detailOn) {
        m_informationLabel->show();
    }

    Job* job = new Job (this);
    job->setAction (m_installAction);
    job->connect(job, &Job::valueChanged,
                this, &ActionDialog::updateInfo);
    
    job->connect(job, &Job::jobCompeted,
                 this, &ActionDialog::jobDone);
    
    job->start();
}

void
ActionDialog::jobDone (bool success)
{
    if (success)
    {
        m_jobSuccesful = true;
        m_messageLabel->setText( tr ( "Changes were made successfully" ) );
    }
    else 
    {
        m_jobSuccesful = false;
        m_messageLabel->setText( tr ( "Changes failed, click on 'Show Details' for more information" ) );
    }
    
    m_terminal->append( QString( "\n" ) );
    m_terminal->append( QString( tr( "Done ..." ) ) );
    m_informationLabel->setText( QString( tr( "Done ..." ) ) );
    m_buttonBox->setEnabled( true );
    m_progressBar->setMaximum(100);
    m_progressBar->setValue(100);
    m_progressBar->setFormat("");
}

bool
ActionDialog::isJobSuccesful() const
{
    return m_jobSuccesful;
}

void
ActionDialog::updateInfo(const QString& data)
{
    writeToTerminal(data);
    m_informationLabel->setText(data);
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
