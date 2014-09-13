#ifndef ENABLEDLOCALESMODEL_H
#define ENABLEDLOCALESMODEL_H

#include "LocaleItem.h"

#include <QtCore/QAbstractTableModel>
#include <QtCore/QObject>
#include <QtCore/QVariant>

#include "unicode/locid.h"
#include "unicode/brkiter.h"

class EnabledLocalesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum EnabledLocalesRoles {
        KeyRole = Qt::UserRole + 1,
        CountryRole,
        LanguageRole,
        IsSystemLang
    };

    struct SystemLocales {
        QString lang;
        QString language;
        QString ctype;
        QString numeric;
        QString time;
        QString collate;
        QString monetary;
        QString messages;
        QString paper;
        QString name;
        QString address;
        QString telephone;
        QString measurement;
        QString identification;
    };

    explicit EnabledLocalesModel(QObject *parent = 0);
    void init();
    void updateSystemLocales();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool insertLocale(int row, int count, const LocaleItem &localeItem);
    bool removeLocale(int row, int count);
    bool setLang(const QModelIndex &index);

    bool contains(const LocaleItem &localeItem) const;
    QStringList locales() const;

    static icu::UnicodeString qStringToUnicodeString(const QString &sourceStr);
    static QString unicodeStringToQString(const icu::UnicodeString &sourceStr);

    SystemLocales systemLocales;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<LocaleItem> locales_;

signals:

public slots:

};

#endif // ENABLEDLOCALESMODEL_H
