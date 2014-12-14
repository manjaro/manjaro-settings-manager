#include "KeyboardAuthHelper.h"

#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtDBus/QDBusInterface>

#include <QDebug>

ActionReply KeyboardAuthHelper::save(const QVariantMap& args)
{
    QString model = args["model"].toString();
    QString layout = args["layout"].toString();
    QString variant = args["variant"].toString();

    // localed d-bus interface to set X11 keyboard layout
    QDBusInterface dbusInterface("org.freedesktop.locale1",
                                 "/org/freedesktop/locale1",
                                 "org.freedesktop.locale1",
                                 QDBusConnection::systemBus());
    QString options = dbusInterface.property("X11Options").toString();

    bool isKeyboardctlInstalled = QFile::exists("/usr/bin/keyboardctl");
    if (isKeyboardctlInstalled) {
        QProcess *keyboardctl = new QProcess(this);
        keyboardctl->start("/usr/bin/keyboardctl", QStringList() << "--set-layout" << model << layout << variant);
        keyboardctl->waitForStarted();
        keyboardctl->waitForFinished();
        if (keyboardctl->exitStatus()) {
            qDebug() << tr("Failed to set keyboard layout");
            return ActionReply::HelperErrorReply();
        } else {
            dbusInterface.call("SetX11Keyboard", layout, model, variant, options, true, false);
        }
    } else {
        // remove leftover keyboardctl files
        const QString keyboardctlXorgConf("/etc/X11/xorg.conf.d/20-keyboard.conf");
        const QString keyboardctlConf("/etc/keyboard.conf");
        if (QFile::exists(keyboardctlXorgConf)) {
            QFile::remove(keyboardctlXorgConf);
        }
        if (QFile::exists(keyboardctlConf)) {
            QFile::remove(keyboardctlConf);
        }

        /* ssssbb
         * string -> layout
         * string -> model
         * string -> variant
         * string -> options
         * boolean -> convert (set vconsole keyboard too)
         * boolean -> arg_ask_password
         */
        QDBusMessage reply;
        reply = dbusInterface.call("SetX11Keyboard", layout, model, variant, options, true, true);
        if (reply.type() == QDBusMessage::ErrorMessage) {
            qDebug() << QString(tr("Failed to set keyboard layout") + "\n" + reply.errorMessage());
            return ActionReply::HelperErrorReply();
        }
    }
    return ActionReply::SuccessReply();
}

KAUTH_HELPER_MAIN("org.manjaro.msm.keyboard", KeyboardAuthHelper)
#include "moc_KeyboardAuthHelper.cpp"
