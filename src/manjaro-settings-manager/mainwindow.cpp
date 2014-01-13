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

#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Qt4 fix
    // Center window on the screen
    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            size(),
            qApp->desktop()->availableGeometry()
        ));

    // Trigger method to setup titels and icons
    buttonShowAllSettings_clicked();

    ui->listWidget->addSeparator(tr("System"));
    addPageWidget(page_LanguagePackages);
    addPageWidget(page_Language);
    addPageWidget(page_Users);

    //
    // Add printer page!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    ui->listWidget->addSeparator(tr("Hardware"));
    addPageWidget(page_Keyboard);
    addPageWidget(page_MHWD);


    // Connect signals and slots
    connect(ui->buttonQuit, SIGNAL(clicked())   ,   qApp, SLOT(quit()));
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*))   ,   this, SLOT(listWidget_itemClicked(QListWidgetItem*)));
    connect(ui->buttonAllSettings, SIGNAL(clicked())    ,   this, SLOT(buttonShowAllSettings_clicked()));
    connect(ui->buttonApply, SIGNAL(clicked())  ,   this, SLOT(buttonApply_clicked()));

    // Check passed application arguments
    checkAppArguments();
}



MainWindow::~MainWindow()
{
    delete ui;
}



//###
//### Private
//###


/*void MainWindow::checkDaemonAutostart() {
    if (QFile::exists(QDir::homePath() + QString(DAEMON_AUTOSTART_FILE)))
        return;

    if (QMessageBox::Yes != QMessageBox::question(this, tr("Manjaro Settings Daemon"),
                                                  tr("No autostart file found for the Manjaro Settings Daemon. Should it be created automatically?"),
                                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
        return;

    QFile file(QDir::homePath() + QString(DAEMON_AUTOSTART_FILE));

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Manjaro Settings Daemon"),
                             tr("Failed to create the Manjaro Settings Daemon autostart file!"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    QTextStream out(&file);
    out << "[Desktop Entry]\n";
    out << "Encoding=UTF-8\n";
    out << "Version=0.9.4\n";
    out << "Type=Application\n";
    out << "Name=Manjaro Settings Manager Daemon\n";
    out << "Comment=\n";
    out << "Exec=manjaro-settings-manager-daemon\n";
    out << "StartupNotify=false\n";
    out << "Terminal=false\n";
    out << "Hidden=false";

    file.close();
}*/


void MainWindow::checkAppArguments() {
    foreach (QString arg, qApp->arguments()) {
        if (arg == "--page-language-packages") {
            // Show language packages page
            for(int i = 0; i < ui->listWidget->count(); i++) {
                ListWidgetItem *item = dynamic_cast<ListWidgetItem*>(ui->listWidget->item(i));
                if (!item || !item->page || item->page != &page_LanguagePackages)
                    continue;

                listWidget_itemClicked(item);
                break;
            }
        }
    }
}



void MainWindow::addPageWidget(PageWidget &page) {
    // Add list widget item
    ListWidgetItem *item = new ListWidgetItem(ui->listWidget);
    item->setText(page.getTitel());
    item->setIcon(QIcon(page.getIcon()));
    item->setSizeHint(QSize(135, 100));
    item->page = &page;

    // Add to stacked widget
    ui->stackedWidget->addWidget(&page);

    connect(&page, SIGNAL(setApplyEnabled(PageWidget*, bool))    ,   this, SLOT(setApplyEnabled(PageWidget*, bool)));
    connect(&page, SIGNAL(closePage(PageWidget*))  ,   this, SLOT(closePageRequested(PageWidget*)));
}



//###
//### Slots
//###


void MainWindow::listWidget_itemClicked(QListWidgetItem *current) {
    ListWidgetItem *item = dynamic_cast<ListWidgetItem*>(current);
    if (!item || !item->page)
        return;

    // Show page and buttons
    ui->stackedWidget->setCurrentWidget(item->page);
    ui->buttonAllSettings->setVisible(true);
    ui->buttonApply->setEnabled(true);
    ui->buttonApply->setVisible(item->page->getShowApplyButton());

    // Setup icon and titel
    ui->labelHeader->setText(item->page->getTitel());
    ui->labelIcon->setPixmap(item->page->getIcon());

    // Remove list widget selection
    ui->listWidget->clearSelection();

    // Trigger activated method of page
    item->page->activated();
}



void MainWindow::buttonShowAllSettings_clicked() {
    PageWidget *page = dynamic_cast<PageWidget*>(ui->stackedWidget->currentWidget());
    if (page && !page->showAllSettingsRequested())
        return;

    // Remove list widget selection
    ui->listWidget->clearSelection();

    // Setup icon and titel
    ui->labelHeader->setText(tr("Manjaro Settings"));
    ui->labelIcon->setPixmap(QPixmap(":/images/resources/settings.png"));

    // Hide buttons
    ui->buttonAllSettings->setVisible(false);
    ui->buttonApply->setVisible(false);

    // Show all settings
    ui->stackedWidget->setCurrentIndex(0);
}



void MainWindow::setApplyEnabled(PageWidget *page, bool enabled) {
    if (dynamic_cast<PageWidget*>(ui->stackedWidget->currentWidget()) != page)
        return;

    ui->buttonApply->setEnabled(enabled);
}



void MainWindow::buttonApply_clicked() {
    PageWidget *page = dynamic_cast<PageWidget*>(ui->stackedWidget->currentWidget());
    if (!page)
        return;

    page->apply_clicked();
}



void MainWindow::closePageRequested(PageWidget *page) {
    if (dynamic_cast<PageWidget*>(ui->stackedWidget->currentWidget()) != page)
        return;

    buttonShowAllSettings_clicked();
}



void MainWindow::writePositionSettings()
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



void MainWindow::readPositionSettings()
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

void MainWindow::closeEvent(QCloseEvent *)
{
    writePositionSettings();
}
