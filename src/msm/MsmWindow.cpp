/*
 *  Manjaro Settings Manager
 *  Ramon Buldó <ramon@manjaro.org>
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

#include "MsmWindow.h"
#include "ModuleView.h"

#include <QtCore/QPropertyAnimation>
#include <QtCore/QSettings>
#include <QtWidgets/QGraphicsOpacityEffect>

#include <KCModuleInfo>

#include <QDebug>

MsmWindow::MsmWindow(QWidget *parent) :
    QMainWindow(parent)
{
    moduleView = new ModuleView();
    setCentralWidget(moduleView);

    init();
    readPositionSettings();

    // Trigger method to setup titels and icons
    //buttonShowAllSettings_clicked();
}


MsmWindow::~MsmWindow()
{
    delete moduleView;
}

void MsmWindow::init()
{
    QStringList moduleList = QStringList() << "msm_kernel" << "msm_keyboard" << "msm_language_packages"
                                           << "msm_locale" << "msm_mhwd" << "msm_notifications"
                                           << "msm_timedate" << "msm_users";
    for (QString module : moduleList) {
        moduleInfoList.insert(module, new KCModuleInfo(module));
    }

    for (auto moduleInfo : moduleInfoList) {
        moduleView->addModule(moduleInfo);
    }
}


void MsmWindow::addPageWidget(ModuleView &page)
{
    // Add list widget item
    //ListWidgetItem *item = new ListWidgetItem(ui->listWidget);
    //item->setText(page.getTitel());
    //item->setIcon(QIcon(page.getIcon()));
    //item->setSizeHint(QSize(135, 100));
    //item->page = &page;

    // Add to stacked widget
    //ui->stackedWidget->addWidget(&page);

    //connect(&page, &PageWidget::setApplyEnabled,
            //this, &MainWindow::setApplyEnabled);
    //connect(&page, &PageWidget::closePage,
            //this, &MainWindow::closePageRequested);
}


void MsmWindow::listWidget_itemClicked(QListWidgetItem *current)
{
    /*ListWidgetItem *item = dynamic_cast<ListWidgetItem*>(current);
    if (!item || !item->page) {
        return;
    }*/

    // Show page and buttons
    //ui->stackedWidget->setCurrentWidget(item->page);
    //ui->buttonAllSettings->setVisible(true);
    //ui->buttonApply->setEnabled(true);
    //ui->buttonApply->setVisible(item->page->getShowApplyButton());

    // Setup icon and titel
    //ui->labelHeader->setText(item->page->getTitel());
    //ui->labelIcon->setPixmap(item->page->getIcon());

    // Remove list widget selection
    //ui->listWidget->clearSelection();

    // Trigger activated method of page
    //item->page->activated();
}



void MsmWindow::buttonShowAllSettings_clicked()
{
   // ModuleView *page = dynamic_cast<ModuleView*>(ui->stackedWidget->currentWidget());
    //if (page && !page->showAllSettingsRequested()) {
    //    return;
    //}

    // Remove list widget selection
    //ui->listWidget->clearSelection();

    // Setup icon and titel
    //ui->labelHeader->setText(tr("Manjaro Settings"));
    //ui->labelIcon->setPixmap(QPixmap(":/images/resources/settings.png"));

    // Hide buttons
    //ui->buttonAllSettings->setVisible(false);
    //ui->buttonApply->setVisible(false);

    // Show all settings
    //ui->stackedWidget->setCurrentIndex(0);
}



void MsmWindow::setApplyEnabled(ModuleView *page, bool enabled)
{
    //if (dynamic_cast<PageWidget*>(ui->stackedWidget->currentWidget()) != page) {
    //    return;
    //}
    //ui->buttonApply->setEnabled(enabled);
}



void MsmWindow::buttonApply_clicked()
{
    //PageWidget *page = dynamic_cast<PageWidget*>(ui->stackedWidget->currentWidget());
    //if (!page) {
    //    return;
    //}
    //page->apply_clicked();
}



void MsmWindow::closePageRequested(ModuleView *page)
{
    //if (dynamic_cast<PageWidget*>(ui->stackedWidget->currentWidget()) != page) {
    //    return;
    //}
    //buttonShowAllSettings_clicked();
}



void MsmWindow::writePositionSettings()
{
    QSettings settings("manjaro", "manjaro-settings-manager");

    settings.beginGroup("mainwindow");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("savestate", saveState());
    settings.setValue("maximized", isMaximized());
    if (!isMaximized()) {
        settings.setValue("pos", pos());
        settings.setValue("size", size());
    }

    settings.endGroup();
}



void MsmWindow::readPositionSettings()
{
    QSettings settings("manjaro", "manjaro-settings-manager");

    settings.beginGroup("mainwindow");

    restoreGeometry(settings.value("geometry", saveGeometry()).toByteArray());
    restoreState(settings.value( "savestate", saveState()).toByteArray());
    move(settings.value("pos", pos()).toPoint());
    resize(settings.value("size", size()).toSize());
    if ( settings.value("maximized", isMaximized() ).toBool())
        showMaximized();

    settings.endGroup();
}

void MsmWindow::closeEvent(QCloseEvent *)
{
    writePositionSettings();
}
