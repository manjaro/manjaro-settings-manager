/*
 *  This file is part of Manjaro Settings Manager.
 *
 *  Roland Singer <roland@manjaro.org>
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

#ifndef APPLYDIALOG_H
#define APPLYDIALOG_H

#include <QDialog>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>
#include <QMessageBox>


namespace Ui
{
class ApplyDialog;
}


class ApplyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ApplyDialog( QWidget* parent = 0 );
    ~ApplyDialog();

    bool processSuccess();
    using QDialog::exec;

public slots:
    int exec( QString cmd, QStringList arguments, QString infoText = "",bool skipCloseTimer = false );

private:
    Ui::ApplyDialog* ui;
    QProcess process;
    QTimer closeTimer;
    int closeSec;
    bool skipCloseTimer;
    QString lastMessage;

protected slots:
    void buttonCancel_clicked();
    void processFinished( int exitCode );
    void closeTimer_timeout();
    void process_readyRead();
};

#endif // APPLYDIALOG_H
