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

#include "SupportedLocalesModel.h"

#include "models/KeyboardItem.h"

#include <QtCore/QFile>

#include <QDebug>

SupportedLocalesModel::SupportedLocalesModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new LocaleItem(QString("key"));
    init(m_rootItem);
}

SupportedLocalesModel::~SupportedLocalesModel()
{
    delete m_rootItem;
}


QVariant SupportedLocalesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    KeyboardItem *item = static_cast<KeyboardItem*>(index.internalPointer());

    switch (role) {
    case Qt::DisplayRole :
        switch (index.column()) {
        case 0:
            return item->key();
        case 1:
            return item->description();
        }
        break;
    case KeyRole :
        return item->key();
    }

    return QVariant();
}


Qt::ItemFlags SupportedLocalesModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return 0;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant SupportedLocalesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            return m_rootItem->key();
        }
    }

    return QVariant();
}


QModelIndex SupportedLocalesModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    LocaleItem *parentItem;

    if (!parent.isValid()) {
        parentItem = m_rootItem;
    } else {
        parentItem = static_cast<LocaleItem*>(parent.internalPointer());
    }

    LocaleItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}


QModelIndex SupportedLocalesModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    LocaleItem *childItem = static_cast<LocaleItem*>(index.internalPointer());
    LocaleItem *parentItem = childItem->parent();

    if (parentItem == m_rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}


int SupportedLocalesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0) {
        return 0;
    }

    LocaleItem *parentItem;
    if (!parent.isValid()) {
        parentItem = m_rootItem;
    } else {
        parentItem = static_cast<LocaleItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}


int SupportedLocalesModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return static_cast<KeyboardItem*>(parent.internalPointer())->columnCount();
    } else {
        return m_rootItem->columnCount();
    }
}


QHash<int, QByteArray> SupportedLocalesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[KeyRole] = "key";
    return roles;
}


void SupportedLocalesModel::init(LocaleItem *parent)
{
    QFile file("/usr/share/i18n/SUPPORTED");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Faile to open /usr/share/i18n/SUPPORTED";
        return;
    }

    QTextStream in(&file);
    bool found = false;
    while (!in.atEnd()) {
        QString line = in.readLine().split("#", QString::KeepEmptyParts).first().trimmed();
        if (line.startsWith("SUPPORTED-LOCALES=")) {
            found = true;
            continue;
        }

        if (found && !line.isEmpty()) {
            QString localeCode = line.mid(0, line.indexOf("/")).remove("\\").trimmed();
            QString localeGen = line.replace("/", " ").replace(" \\", "");

            Locale systemLocale = Locale();
            Locale locale(localeCode.toLatin1());

            /* Get language and country in current system locale */
            UnicodeString uDisplayLanguage;
            UnicodeString uDisplayCountry;
            locale.getDisplayLanguage(systemLocale, uDisplayLanguage);
            locale.getDisplayCountry(systemLocale, uDisplayCountry);

            /* Capitalize language and country */
            UErrorCode status;
            BreakIterator *titleIterator = BreakIterator::createTitleInstance(systemLocale, status);
            uDisplayLanguage = uDisplayLanguage.toTitle(titleIterator);
            uDisplayCountry = uDisplayCountry.toTitle(titleIterator);

            QString displayLanguage = unicodeStringToQString(uDisplayLanguage);
            QString displayCountry = unicodeStringToQString(uDisplayCountry);

            if (systemLocale == locale) {
                m_currentLanguage = displayLanguage;
            }

            /* Search if we already added this language to the tree */
            QModelIndexList languageIndexList = match(index(0,0),
                                                       KeyRole,
                                                       displayLanguage,
                                                       Qt::MatchFixedString);
            LocaleItem *languageItem;
            QModelIndex languageIndex;
            if (languageIndexList.count() == 0) {
                /* Not found, add the language to the root*/
                languageItem = new LocaleItem(displayLanguage, parent);
                parent->appendChild(languageItem);
            } else {
                Q_ASSERT(languageIndexList.count() == 1);
                /* Found, convert index to a item */
                languageIndex = languageIndexList.first();
                languageItem = static_cast<LocaleItem*>(languageIndex.internalPointer());
            }

            /* Search if we already added this country to this language */
            QModelIndexList countryIndexList = match(languageIndex.child(0,0),
                                                    KeyRole,
                                                    displayCountry,
                                                    Qt::MatchFixedString);
            LocaleItem *countryItem;
            QModelIndex countryIndex;
            if (countryIndexList.count() == 0) {
                /* Not found, add the country to the language */
                countryItem = new LocaleItem(displayCountry, languageItem);
                languageItem->appendChild(countryItem);
            } else {
                Q_ASSERT(countryIndexList.count() == 1);
                /* Found, convert index to a item */
                countryIndex = countryIndexList.first();
                countryItem = static_cast<LocaleItem*>(countryIndex.internalPointer());
            }

            /* Add the locale code to the language */
            LocaleItem *localeItem = new LocaleItem(localeCode, countryItem);
            countryItem->appendChild(localeItem);
        }
    }
    file.close();
}


QString SupportedLocalesModel::currentLanguage()
{
    return m_currentLanguage;
}


icu::UnicodeString SupportedLocalesModel::qStringToUnicodeString(const QString &sourceStr)
{
    return UnicodeString(static_cast<const UChar *>(sourceStr.utf16()),
                         sourceStr.length());
}


QString SupportedLocalesModel::unicodeStringToQString(const icu::UnicodeString &sourceStr)
{
    return QString(reinterpret_cast<const QChar *>(sourceStr.getBuffer()),
                   sourceStr.length());
}
