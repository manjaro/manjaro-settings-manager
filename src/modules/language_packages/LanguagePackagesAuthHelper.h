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

#ifndef LANGUAGEPACKAGESAUTHHELPER_H
#define LANGUAGEPACKAGESAUTHHELPER_H

#include <KAuth>

using namespace KAuth;

class LanguagePackagesAuthHelper : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    ActionReply install( const QVariantMap& args );
    ActionReply remove( const QVariantMap& args );
};

#endif // LANGUAGEPACKAGESAUTHHELPER_H
