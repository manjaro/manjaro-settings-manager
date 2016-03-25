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

#ifndef LANGUAGECOMMON_H
#define LANGUAGECOMMON_H

#include <QtCore/QStringList>

class LanguageCommon
{
public:
    static QStringList enabledLocales( bool clean = false );
    static QStringList supportedLocales( bool clean = false );

    /**
     * @brief localeToLocaleGenFormat converts a locale string to the format
     *                                used by /etc/locale.gen
     * @param locale locale string
     * @return converted locale, or empty if it fails
     */
    static QString localeToLocaleGenFormat( const QString locale );
};

#endif // LANGUAGECOMMON_H
