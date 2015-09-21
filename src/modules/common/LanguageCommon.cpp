#include "LanguageCommon.h"

#include <QtCore/QRegularExpression>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QDebug>

QStringList LanguageCommon::enabledLocales(bool clean)
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
        line = QString(line.split(" ", QString::SkipEmptyParts)
                                   .first()
                                   .trimmed());
        // Remove .UTF-8, @euro ...
        if (clean) {
            line = line.split(QRegularExpression("[ .@]"),
                              QString::SkipEmptyParts)
                       .first();
        }
        locales << line;
    }
    localeGen.close();
    return locales;
}


QStringList LanguageCommon::supportedLocales(bool clean)
{
    QFile localeGen("/etc/locale.gen");
    QString lines;
    if (localeGen.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&localeGen);
        lines.append(in.readAll());
    } else {
        qDebug() << "error: failed to open '" << "/etc/locale.gen" << "'!";
    }

    QFile localeGenPacnew("/etc/locale.gen.pacnew");
    if (localeGenPacnew.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&localeGenPacnew);
        lines.append(in.readAll());
    } else {
        qDebug() << "warning: failed to open '" << "/etc/locale.gen.pacnew" << "'!";
    }

    QSet<QString> localeList;
    for (QString line : lines.split('\n')) {
        if (line.startsWith( "# " ) || line.simplified() == "#" ||
                line.isEmpty()) {
            continue;
        }

        line = line.simplified();
        if (line.startsWith("#")) {
            line.remove( '#' );
        }

        line = line.split(" ", QString::SkipEmptyParts)
                   .first()
                   .trimmed();
        // Remove .UTF-8, @euro ...
        if (clean) {
            line = line.split(QRegularExpression("[ .@]"),
                              QString::SkipEmptyParts)
                       .first();
        }
        localeList.insert(line);
    }
    return localeList.toList();
}
