#include "LanguageCommon.h"

#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QDebug>

QStringList LanguageCommon::enabledLocales()
{
    QStringList locales;
    QFile localeGen("/etc/locale.gen");
    if (!localeGen.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "error: failed to open '" << "/etc/locale.gen" << "'!";
        return QStringList();
    }

    QTextStream in(&localeGen);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.isEmpty() || line.startsWith("#")) {
            continue;
        }
        locales << QString(line.split(" ", QString::SkipEmptyParts).value(0));
    }
    localeGen.close();
    return locales;
}


QStringList LanguageCommon::supportedLocales()
{
    QFile localeGen("/etc/locale.gen");
    QString lines;
    if (localeGen.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&localeGen);
        lines.append(in.readAll());
    }

    QFile localeGenPacnew("/etc/locale.gen.pacnew");
    if (localeGenPacnew.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&localeGenPacnew);
        lines.append(in.readAll());
    }

    QSet<QString> localeList;
    for (const QString line : lines.split('\n')) {
        if (line.startsWith( "# " ) || line.simplified() == "#" ||
                line.isEmpty()) {
            continue;
        }

        QString lineTmp = line.simplified();
        if (lineTmp.startsWith("#")) {
            lineTmp.remove( '#' );
        }

        localeList.insert(lineTmp.split(" ", QString::SkipEmptyParts)
                                 .first()
                                 .trimmed());
    }
    return localeList.toList();
}
