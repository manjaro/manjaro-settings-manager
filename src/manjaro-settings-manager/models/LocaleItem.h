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

#ifndef LOCALESITEM_H
#define LOCALESITEM_H

#include <QtCore/QVariant>

class LocaleItem
{
public:
    explicit LocaleItem(const QString &key, LocaleItem *parent = 0);
    ~LocaleItem();

    void appendChild(LocaleItem *item);

    QString key() const;

    LocaleItem *child(int row);
    int childCount() const;
    int columnCount() const;
    int row() const;
    LocaleItem *parent();

    bool operator==(const LocaleItem &other);

private:
    QString key_;

    QList<LocaleItem*> childItems_;
    LocaleItem *parentItem_;
};

#endif // LOCALESITEM_H
