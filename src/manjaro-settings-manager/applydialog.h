/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
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

#ifndef APPLYDIALOG_H
#define APPLYDIALOG_H

#include <QDialog>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>


namespace Ui {
class ApplyDialog;
}



class ApplyDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ApplyDialog(QWidget *parent = 0);
    ~ApplyDialog();

    int exec(QString cmd, QStringList arguments, QString infoText = "",bool skipCloseTimer = false);
    bool processSuccess();
    
private:
    Ui::ApplyDialog *ui;
    QProcess process;
    QTimer closeTimer;
    int closeSec;
    bool skipCloseTimer;

protected slots:
    void buttonCancel_clicked();
    void processFinished(int exitCode);
    void closeTimer_timeout();
    void process_readyRead();

};

#endif // APPLYDIALOG_H
