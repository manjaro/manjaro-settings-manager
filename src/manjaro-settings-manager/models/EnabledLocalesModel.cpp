#include "EnabledLocalesModel.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtDBus/QDBusInterface>

#include <QDebug>

EnabledLocalesModel::EnabledLocalesModel(QObject *parent) :
    QAbstractListModel(parent)
{

}


int EnabledLocalesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return locales_.size();
}


int EnabledLocalesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return roleNames().size();
}


QVariant EnabledLocalesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= locales_.count()) {
        return QVariant();
    }
    const LocaleItem &localeItem = locales_[index.row()];

    Locale systemLocale = Locale();
    Locale locale(localeItem.key().toLatin1());

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

    switch (role) {
    case KeyRole:
        return localeItem.key();
    case CountryRole:
        return displayCountry;
    case LanguageRole:
        return displayLanguage;
    case IsSystemLang:
        return (localeItem.key() == systemLocales.lang);
    }

    return QVariant();
}


bool EnabledLocalesModel::insertLocale(int row, int count, const LocaleItem &localeItem)
{
    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int c=0; c < count; c++) {
        locales_.insert(row, localeItem);
    }

    endInsertRows();
    return true;
}


bool EnabledLocalesModel::removeLocale(int row, int count)
{
    /* Don't delete last locale */
    if (rowCount(QModelIndex()) == 1) {
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int c=0; c < count; c++) {
        locales_.removeAt(row);
    }

    endRemoveRows();
    return true;
}


bool EnabledLocalesModel::setLang(const QModelIndex &index)
{
    if (systemLocales.lang != index.data(EnabledLocalesModel::KeyRole).toString()) {
        systemLocales.lang = index.data(EnabledLocalesModel::KeyRole).toString();
        emit dataChanged(index, index);
        return true;
    }
    return false;
}


bool EnabledLocalesModel::contains(const LocaleItem &localeItem) const
{
    return (locales_.contains(localeItem));
}


/*
 * Return a QStringlist of enabled locales keys (es_ES.UTF-8, en_US.UTF-8, ...)
 */
QStringList EnabledLocalesModel::locales() const
{
    QStringList locales;
    for (LocaleItem localeItem : locales_) {
        locales << localeItem.key();
    }
    return locales;
}


icu::UnicodeString EnabledLocalesModel::qStringToUnicodeString(const QString &sourceStr)
{
    return UnicodeString(static_cast<const UChar *>(sourceStr.utf16()),
                         sourceStr.length());
}


QString EnabledLocalesModel::unicodeStringToQString(const icu::UnicodeString &sourceStr)
{
    return QString(reinterpret_cast<const QChar *>(sourceStr.getBuffer()),
                   sourceStr.length());
}


QHash<int, QByteArray> EnabledLocalesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[KeyRole] = "key";
    roles[CountryRole] = "country";
    roles[LanguageRole] = "language";
    roles[IsSystemLang] = "isSystemLang";
    return roles;
}


/*
 * Read uncommented locales from /etc/locale.gen
 */
void EnabledLocalesModel::init()
{
    beginResetModel();
    locales_.clear();
    QFile file("/etc/locale.gen");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "error: failed to open '" << "/etc/locale.gen" << "'!";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.isEmpty() || line.startsWith("#")) {
            continue;
        }
        locales_ << LocaleItem(line.split(" ", QString::SkipEmptyParts).value(0));
    }
    file.close();
    endResetModel();
}


/*
 * Read systemLocales using org.freedesktop.locale1 dbus interface
 */
void EnabledLocalesModel::updateSystemLocales()
{
    QDBusInterface dbusInterface("org.freedesktop.locale1",
                                 "/org/freedesktop/locale1",
                                 "org.freedesktop.locale1",
                                 QDBusConnection::systemBus());

    QStringList localeEnviromentVariables = dbusInterface.property("Locale").toStringList();
    for (QString lev : localeEnviromentVariables) {
        if (lev.startsWith("LANG=")) {
            systemLocales.lang = lev.section('=', 1);
        } else if (lev.startsWith("LANGUAGE=")) {
            systemLocales.language = lev.section('=', 1);
        } else if (lev.startsWith("LC_CTYPE=")) {
            systemLocales.ctype = lev.section('=', 1);
        } else if (lev.startsWith("LC_NUMERIC=")) {
            systemLocales.numeric = lev.section('=', 1);
        } else if (lev.startsWith("LC_TIME=")) {
            systemLocales.time = lev.section('=', 1);
        } else if (lev.startsWith("LC_COLLATE=")) {
            systemLocales.collate = lev.section('=', 1);
        } else if (lev.startsWith("LC_MONETARY=")) {
            systemLocales.monetary = lev.section('=', 1);
        } else if (lev.startsWith("LC_MESSAGES=")) {
            systemLocales.messages = lev.section('=', 1);
        } else if (lev.startsWith("LC_PAPER=")) {
            systemLocales.paper = lev.section('=', 1);
        } else if (lev.startsWith("LC_NAME=")) {
            systemLocales.name = lev.section('=', 1);
        } else if (lev.startsWith("LC_ADDRESS=")) {
            systemLocales.address = lev.section('=', 1);
        } else if (lev.startsWith("LC_TELEPHONE=")) {
            systemLocales.telephone = lev.section('=', 1);
        } else if (lev.startsWith("LC_MEASUREMENT=")) {
            systemLocales.measurement = lev.section('=', 1);
        } else if (lev.startsWith("LC_IDENTIFICATION=")) {
            systemLocales.identification = lev.section('=', 1);
        }
    }
}
