#include "Job.h"

#include <KAuthExecuteJob>
#include <QtCore/QRegularExpression>
#include <QDebug>

Job::Job(QObject *parent, bool b) :
    QThread(parent), Stop(b)
{
}

// run() will be called when a thread starts

void Job::setAction(KAuth::Action action)
{
    this->m_installAction = action;
}

void Job::run()
{
    KAuth::ExecuteJob* job = m_installAction.execute();
    connect( job, &KAuth::ExecuteJob::newData,
             [=] ( const QVariantMap &data )
    {
        QString output = data.value( "Data" ).toString();
        foreach ( auto line, output.split( QRegExp( "[\r\n]" ),QString::SkipEmptyParts ) )
        {
            if ( line != m_lastMessage )
            {
                QString l = line.remove( QRegularExpression( "\x1b[^m]*m" ));
                m_lastMessage = l;
                emit this->valueChanged( l );
            }
        }

    } );
    if ( job->exec() ) 
    {
        emit this->jobCompeted(true);
    }
    else 
    {
        emit this->jobCompeted(false);
    }
}
