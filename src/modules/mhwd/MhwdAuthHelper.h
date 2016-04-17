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

#ifndef MHWDAUTHHELPER_H
#define MHWDAUTHHELPER_H

#include <KAuth>

using namespace KAuth;

/**
 * @brief The MhwdAuthHelper class, calls mhwd to install or remove drivers.
 *        there are two functions so the polkit dialog can have different
 *        strings in each case, but the remove function just calls install.
 */
class MhwdAuthHelper : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    /**
     * @brief install calls mhwd process and send back process output
     *                to the helper
     * @param args QVariantMap with "arguments" for the process
     * @return returns SuccessReply() if success or HelperErrorReply() if
     *         fails
     */
    ActionReply install( const QVariantMap& args );

    /**
     * @brief remove calls install function
     * @param args QVariantMap with "arguments" for the process
     * @return returns SuccessReply() if success or HelperErrorReply() if
     *         fails
     */
    ActionReply remove( const QVariantMap& args );
};

#endif // MHWDAUTHHELPER_H
