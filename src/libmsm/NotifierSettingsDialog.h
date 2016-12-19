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

#ifndef NOTIFIERSETTINGSDIALOG_H
#define NOTIFIERSETTINGSDIALOG_H

#include <QDialog>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QPushButton>
#include <QTextEdit>
#include <QMessageBox>
#include <QtCore/QSettings>

namespace Ui
{
class NotifierSettingsDialog;
}


class NotifierSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NotifierSettingsDialog( QWidget* parent = 0 );
    ~NotifierSettingsDialog();

    void load();
    QSettings::Status save();

private:
    Ui::NotifierSettingsDialog* ui;


protected slots:
    void buttonQuit_clicked();
    void buttonApply_clicked();
    void unsupportedKernelStateBoxChanged( int );
    void newKernelStateBoxChanged( int );
};

#endif // NOTIFIERSETTINGSDIALOG_H
