#include "LocaleAuthHelper.h"

#include <QtDBus/QDBusInterface>
#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtCore/QTextStream>
#include <QtCore/QSet>

ActionReply LocaleAuthHelper::save(const QVariantMap& args)
{
    if (args["isLocaleListModified"].toBool()) {
        updateLocaleGen(args["locales"].toStringList());
        generateLocaleGen();
    }
    if (args["isSystemLocalesModified"].toBool()) {
        setSystemLocale(args["localeList"].toStringList());
    }
    return ActionReply::SuccessReply();
}


// Update locale.gen file
// Return true if successful
bool LocaleAuthHelper::updateLocaleGen(QStringList locales) {

    const QString localeGen = "/etc/locale.gen";
    QFile file(localeGen);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        /*QMessageBox::warning(this,
                             tr("Error!"),
                             tr("Failed to open file '%1'!").arg(localeGen),
                             QMessageBox::Ok, QMessageBox::Ok);*/
        return false;
    }

    QStringList content;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        content.append(line);
        line = line.trimmed();

        bool found = false;

        for (QString locale : locales) {
            if (line.startsWith(locale + " ")) {
                found = true;
                locales.removeAll(locale);
                break;
            }

            if (line.startsWith("#" + locale + " ")) {
                content.removeLast();
                content.append(line.remove(0, 1));
                found = true;
                locales.removeAll(locale);
                break;
            }
        }

        if (!found && !line.split("#", QString::KeepEmptyParts).first().trimmed().isEmpty()) {
            content.removeLast();
            content.append("#" + line);
        }
    }
    file.close();

    // Add missing locales in the file
    for (QString locale : locales) {
        QString str = localeToValidLocaleGenString(locale);

        if (str.isEmpty()) {
            /*QMessageBox::warning(this,
                                 tr("Error!"),
                                 tr("Failed to obtain valid locale string for locale '%1'!").arg(locale),
                                 QMessageBox::Ok, QMessageBox::Ok);*/
            continue;
        }

        content.append(str);
    }


    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        /*QMessageBox::warning(this,
                             tr("Error!"),
                             tr("Failed to open file '%1'!").arg(localeGen),
                             QMessageBox::Ok, QMessageBox::Ok);*/
        return false;
    }

    QTextStream out(&file);
    out << content.join("\n");
    file.close();
    return true;
}


// Run local-gen
// Return true if successful
bool LocaleAuthHelper::generateLocaleGen()
{
    QProcess *localeGen = new QProcess();
    localeGen->start("/usr/bin/local-gen");
    connect(localeGen, &QProcess::readyRead,
            [=] ()
    {
        QString data = QString::fromUtf8(localeGen->readAll()).trimmed();
        if (!data.isEmpty()) {
            QVariantMap map;
            map.insert(QLatin1String("Data"), data);
            HelperSupport::progressStep(map);
        }
    });
    localeGen->waitForStarted();
    localeGen->waitForFinished(-1);

    return true;
}


// Modify /etc/locale.conf using systemd-localed
bool LocaleAuthHelper::setSystemLocale(const QStringList localeList)
{
    QDBusInterface dbusInterface("org.freedesktop.locale1",
                                 "/org/freedesktop/locale1",
                                 "org.freedesktop.locale1",
                                 QDBusConnection::systemBus());
    /*
     * asb
     * array_string -> locale
     * boolean -> arg_ask_password
     */
    QDBusMessage reply;
    reply = dbusInterface.call("SetLocale", localeList, true);
    if (reply.type() == QDBusMessage::ErrorMessage) {
        return false;
    }
    return true;
}

QString LocaleAuthHelper::localeToValidLocaleGenString(const QString locale)
{
    QSet<QString> localeList;

    QFile localeGen("/etc/locale.gen");
    QString lines;
    if ( localeGen.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        QTextStream in(&localeGen);
        lines.append(in.readAll());
    }

    QFile localeGenPacnew("/etc/locale.gen.pacnew");
    if ( localeGenPacnew.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        QTextStream in(&localeGenPacnew);
        lines.append(in.readAll());
    }
    for (const QString line : lines.split('\n'))
    {
        if (line.startsWith( "# " ) || line.simplified() == "#" || line.isEmpty()) {
            continue;
        }

        QString lineString = line.simplified();

        if (lineString.startsWith("#")) {
            lineString.remove( '#' );
        }

        localeList.insert(lineString);
    }

    for (const QString line : localeList) {
        if (line.startsWith(locale + " ")) {
            return line;
        }
    }

    return "";
}

KAUTH_HELPER_MAIN("org.manjaro.msm.locale", LocaleAuthHelper)
#include "moc_LocaleAuthHelper.cpp"
