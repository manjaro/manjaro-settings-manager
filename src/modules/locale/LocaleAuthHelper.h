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

#ifndef LOCALEAUTHHELPER_H
#define LOCALEAUTHHELPER_H

#include <KAuth>

using namespace KAuth;

/**
 * @brief The LocaleAuthHelper class, saves changes locale settings
 *        using polkit.
 */
class LocaleAuthHelper : public QObject
{
    Q_OBJECT

public Q_SLOTS:

    /**
     * @brief save save changes in the locales
     * @param args QVariantMap with
     *        isLocaleListModified: bool, if passed locales must be passed too
     *        locales: list of locales.
     *        isSystemLocalesModified": bool, if is passed localeList
     *                                  must be passed too.
     *        localeList: array of environment variables, see setSystemLocale
     * @return
     */
    ActionReply save( const QVariantMap& args );

private:
    /**
     * @brief updateLocaleGen update /etc/locale.gen, so only locales passed
     *                        are uncommented.
     * @param locales list of locales.
     * @return true if succesfull, false otherwise.
     */
    bool updateLocaleGen( const QStringList locales );

    /**
     * @brief generateLocaleGen run locale-gen system utility.
     * @return true if succesfull, false otherwise.
     */
    bool generateLocaleGen();

    /**
     * @brief setSystemLocale modify /etc/locale.conf using systemd-localed
     * @param locale consists of an array of environment-variable-assignment-
     *        like strings. The following strings are known: LANG=, LC_CTYPE=,
     *        LC_NUMERIC=, LC_TIME=, LC_COLLATE=, LC_MONETARY=, LC_MESSAGES=,
     *        LC_PAPER=, LC_NAME=, LC_ADDRESS=, LC_TELEPHONE=, LC_MEASUREMENT=,
     *        LC_IDENTIFICATION=.
     * @return true if succesfull, false otherwise.
     */
    bool setSystemLocale( const QStringList locale );
};

#endif // LOCALEAUTHHELPER_H
