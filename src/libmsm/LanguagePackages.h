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

#ifndef LANGUAGEPACKAGES_H
#define LANGUAGEPACKAGES_H

#include "LanguagePackagesItem.h"

#include <QtCore/QString>
#include <QtCore/QObject>

class LanguagePackages : public QObject
{
    Q_OBJECT
public:
    LanguagePackages( QObject* parent = 0 );

    QList<LanguagePackagesItem> languagePackages();

private:
    const QList<QByteArray> intersect( const QList<QByteArray>& packages1, const QList<QByteArray>& packages2 );
    const QList<QByteArray> filterLanguagePackage( QByteArray package, const QList<QByteArray>& packages );
    const QList<QByteArray> installedPackages();
    const QList<QByteArray> availablePackages();
};


#endif // LANGUAGEPACKAGES_H
