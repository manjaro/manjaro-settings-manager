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

#include <KAboutData>
#include <KPluginFactory>

#include <QtCore/QStringList>
#include <QtCore/QTimeZone>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtWidgets/QHBoxLayout>

K_PLUGIN_FACTORY( MsmTimeDateFactory,
                  registerPlugin<TimeDateKcm>( "msm_timedate" ); )

TimeDateKcm::TimeDateKcm( QWidget* parent, const QVariantList& args ) :
    KCModule( parent, args ),
    m_timeDateModule ( new TimeDateModule( this ) )
{
    KAboutData* aboutData = new KAboutData( *m_timeDateModule->aboutData() );
    setAboutData( aboutData );
    setButtons( KCModule::Default | KCModule::Apply );

    QHBoxLayout* hBoxLayout = new QHBoxLayout( this );
    this->setLayout( hBoxLayout );

    hBoxLayout->addWidget( m_timeDateModule );

    connect( m_timeDateModule, &TimeDateModule::changed, [this] ()
    {
        emit changed();
    } );
}


TimeDateKcm::~TimeDateKcm()
{

}

void TimeDateKcm::load()
{
    m_timeDateModule->load();
}

void TimeDateKcm::save()
{
    m_timeDateModule->save();
}

void TimeDateKcm::defaults()
{
    m_timeDateModule->defaults();
}

#include "TimeDateKcm.moc"
