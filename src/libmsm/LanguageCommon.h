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

#ifndef LANGUAGECOMMON_H
#define LANGUAGECOMMON_H

#include <QtCore/QStringList>

class LanguageCommon
{
public:
    /**
     * @brief enabledLocales get a list of enabled locales in the system
     * @param clean if set to true, it cleans up the locale sufix (ex: removes .UTF-8)
     * @return list of enabled locales
     */
    static QStringList enabledLocales( bool clean = false );

    /**
     * @brief supportedLocales get a list of supported locales in the system
     * @param clean if set to true, it cleans up the locale sufix (ex: removes .UTF-8)
     * @return list of enabled locales
     */
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
