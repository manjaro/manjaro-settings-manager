/* === This file is part of Calamares - <http://github.com/calamares> ===
 *
 *   Copyright 2014, Teo Mrnjavac <teo@kde.org>
 *   Copyright 2014, Kevin Kofler <kevin.kofler@chello.at>
 *
 *   Portions from systemd (localed.c):
 *   Copyright 2011 Lennart Poettering
 *   Copyright 2013 Kay Sievers
 *   (originally under LGPLv2.1+, used under the LGPL to GPL conversion clause)
 *
 *   Calamares is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Calamares is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Calamares. If not, see <http://www.gnu.org/licenses/>.
 */

#include "SetKeyboardLayoutJob.h"

#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QSettings>

#include <QDebug>

SetKeyboardLayoutJob::SetKeyboardLayoutJob( const QString& model,
        const QString& layout,
        const QString& variant )
    : m_model( model )
    , m_layout( layout )
    , m_variant( variant )
{
}


QString
SetKeyboardLayoutJob::findLegacyKeymap() const
{
    int bestMatching = 0;
    QString name;

    QFile file( ":/kbd-model-map" );
    file.open( QIODevice::ReadOnly | QIODevice::Text );
    QTextStream stream( &file );
    while ( !stream.atEnd() )
    {
        QString line = stream.readLine().trimmed();
        if ( line.isEmpty() || line.startsWith( '#' ) )
            continue;

        QStringList mapping = line.split( '\t', QString::SkipEmptyParts );
        if ( mapping.size() < 5 )
            continue;

        int matching = 0;

        // Determine how well matching this entry is
        // We assume here that we have one X11 layout. If the UI changes to
        // allow more than one layout, this should change too.
        if ( m_layout == mapping[1] )
            // If we got an exact match, this is best
            matching = 10;
        // Look for an entry whose first layout matches ours
        else if ( mapping[1].startsWith( m_layout + ',' ) )
            matching = 5;

        if ( matching > 0 )
        {
            if ( m_model.isEmpty() || m_model == mapping[2] )
                matching++;

            QString mappingVariant = mapping[3];
            if ( mappingVariant == "-" )
                mappingVariant = QString();
            else if ( mappingVariant.startsWith( ',' ) )
                mappingVariant.remove( 1, 0 );

            if ( m_variant == mappingVariant )
                matching++;

            // We ignore mapping[4], the xkb options, for now. If we ever
            // allow setting options in the UI, we should match them here.
        }

        // The best matching entry so far, then let's save that
        if ( matching >= qMax( bestMatching, 1 ) )
        {
            qDebug() << "Found legacy keymap" << mapping[0]
                     << "with score" << matching;

            if ( matching > bestMatching )
            {
                bestMatching = matching;
                name = mapping[0];
            }
        }
    }

    return name;
}


bool
SetKeyboardLayoutJob::writeVConsoleData( const QString& vconsoleConfPath ) const
{
    QString keymap = findLegacyKeymap();
    if ( keymap.isEmpty() )
    {
        qDebug() << "Trying to use X11 layout" << m_layout
                 << "as the virtual console layout";
        keymap = m_layout;
    }

    QStringList existingLines;

    // Read in the existing vconsole.conf, if it exists
    QFile file( vconsoleConfPath );
    if ( file.exists() )
    {
        file.open( QIODevice::ReadOnly | QIODevice::Text );
        QTextStream stream( &file );
        while ( !stream.atEnd() )
            existingLines << stream.readLine();
        file.close();
        if ( stream.status() != QTextStream::Ok )
            return false;
    }

    // Write out the existing lines and replace the KEYMAP= line
    file.open( QIODevice::WriteOnly | QIODevice::Text );
    QTextStream stream( &file );
    bool found = false;
    foreach ( const QString& existingLine, existingLines )
    {
        if ( existingLine.trimmed().startsWith( "KEYMAP=" ) )
        {
            stream << "KEYMAP=" << keymap << '\n';
            found = true;
        }
        else
            stream << existingLine << '\n';
    }
    // Add a KEYMAP= line if there wasn't any
    if ( !found )
        stream << "KEYMAP=" << keymap << '\n';
    stream.flush();
    file.close();

    return ( stream.status() == QTextStream::Ok );
}


bool
SetKeyboardLayoutJob::writeX11Data( const QString& keyboardConfPath ) const
{
    QFile file( keyboardConfPath );
    file.open( QIODevice::WriteOnly | QIODevice::Text );
    QTextStream stream( &file );

    stream << "Section \"InputClass\"\n"
           "        Identifier \"system-keyboard\"\n"
           "        MatchIsKeyboard \"on\"\n";

    if ( !m_layout.isEmpty() )
        stream << "        Option \"XkbLayout\" \"" << m_layout << "\"\n";

    if ( !m_model.isEmpty() )
        stream << "        Option \"XkbModel\" \"" << m_model << "\"\n";

    if ( !m_variant.isEmpty() )
        stream << "        Option \"XkbVariant\" \"" << m_variant << "\"\n";

    stream << "EndSection\n";
    stream.flush();

    file.close();

    return ( stream.status() == QTextStream::Ok );
}


bool
SetKeyboardLayoutJob::exec()
{
    QString vconsoleConfPath = { "/etc/vconsole.conf" };
    QString keyboardConfPath = { "/etc/X11/xorg.conf.d/00-keyboard.conf" };


    if ( !writeVConsoleData( vconsoleConfPath ) )
        return false;

    if ( !writeX11Data( keyboardConfPath ) )
        return false;

    return true;
}
