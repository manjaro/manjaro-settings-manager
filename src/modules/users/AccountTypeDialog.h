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

#ifndef ACCOUNTTYPEDIALOG_H
#define ACCOUNTTYPEDIALOG_H

#include "UsersCommon.h"

#include <QtWidgets/QDialog>
#include <QtWidgets/QTreeWidgetItem>

namespace Ui
{
class AccountTypeDialog;
}

class AccountTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AccountTypeDialog( QWidget* parent = 0 );
    ~AccountTypeDialog();

    bool userGroupsChanged() const;
    using QDialog::exec;

public slots:
    int exec( QString m_username, QList<UsersCommon::Group> groups);

private:
    Ui::AccountTypeDialog* ui;
    QString m_username;
    bool m_userGroupDataChanged;
    void checkSudoersFile();
    QString m_adminGroup;

private slots:
    void buttonApply_clicked();
    void checkBoxShowGroups_toggled( bool toggled );
    void treeWidget_itemChanged( QTreeWidgetItem* item, int column );
    void comboBoxAccountType_currentIndexChanged( int index );

};

#endif // ACCOUNTTYPEDIALOG_H
