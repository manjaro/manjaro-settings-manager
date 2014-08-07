/*
 *  Manjaro Settings Manager
 *  Roland Singer <roland@manjaro.org>
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


#include "PageKeyboard.h"
#include "ui_PageKeyboard.h"

#include <QtCore/QProcess>
#include <QtCore/QMapIterator>
#include <QtWidgets/QMessageBox>

PageKeyboard::PageKeyboard(QWidget *parent) :
    PageWidget(parent),
    ui(new Ui::PageKeyboard)
{
    ui->setupUi(this);
    setTitel(tr("Keyboard Settings"));
    setIcon(QPixmap(":/images/resources/keyboard.png"));
    setShowApplyButton(true);

    // Keyboard Preview
    ui->KBPreviewLayout->addWidget(&keyboardPreview);

    // Connect signals and slots
    connect(ui->listLayout, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*))   ,   this, SLOT(listLayout_currentItemChanged(QListWidgetItem*,QListWidgetItem*)));
    connect(ui->listVariant, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*))   ,   this, SLOT(listVariant_currentItemChanged(QListWidgetItem*,QListWidgetItem*)));
    connect(ui->buttonRestore, SIGNAL(clicked())  ,   this, SLOT(buttonRestore_clicked()));
}



PageKeyboard::~PageKeyboard()
{
    delete ui;
}



void PageKeyboard::apply_clicked() {
    LayoutItem *layoutItem = dynamic_cast<LayoutItem*>(ui->listLayout->currentItem());
    LayoutItem *variantItem = dynamic_cast<LayoutItem*>(ui->listVariant->currentItem());

    if (!layoutItem || !variantItem)
        return;

    QString model = models.value(ui->comboBoxModel->currentText(), "pc105");
    QString layout = layoutItem->data;
    QString variant = variantItem->data;

    // Set Xorg keyboard layout
    system(QString("setxkbmap -model \"%1\" -layout \"%2\" -variant \"%3\"").arg(model, layout, variant).toUtf8());

    ApplyDialog dialog(this);
    dialog.exec("keyboardctl", QStringList() << "--set-layout" << model << layout << variant, tr("Setting new keyboard layout..."), true);

    emit closePage(this);
}



void PageKeyboard::activated() {
    // Clean up first
    ui->listLayout->clear();
    ui->listVariant->clear();
    ui->comboBoxModel->clear();

    // Default focus
    ui->listLayout->setFocus();


    //### Detect current keyboard layout and variant
    QString currentLayout;
    QString currentVariant;
    QString currentModel;

    if (!Global::getCurrentXorgKeyboardLayout(currentLayout, currentVariant, currentModel))
        QMessageBox::warning(this, tr("Error"), tr("Failed to determine current Xorg keyboard layout!"), QMessageBox::Ok, QMessageBox::Ok);


    //### Models
    models = Global::getKeyboardModels();
    QMapIterator<QString, QString> mi(models);

    ui->comboBoxModel->blockSignals(true);

    defaultIndex = -1;
    int pc105Index;
    while (mi.hasNext()) {
        mi.next();
        if (mi.value() == "pc105") {
            pc105Index = ui->comboBoxModel->count();
        }
        if (mi.value() == currentModel) {
            defaultIndex = ui->comboBoxModel->count();
        }
        ui->comboBoxModel->addItem(mi.key());
    }

    ui->comboBoxModel->blockSignals(false);

    /* If model its not defined, set to default value pc105 */
    if (defaultIndex == -1) {
        defaultIndex = pc105Index;
    }
    ui->comboBoxModel->setCurrentIndex(defaultIndex);




    //### Layouts and Variants

    // Block signals
    ui->listLayout->blockSignals(true);

    QMap<QString, Global::KeyboardInfo> layouts = Global::getKeyboardLayouts();
    QMapIterator<QString, Global::KeyboardInfo> li(layouts);
    LayoutItem *currentLayoutItem = NULL;

    if (layouts.isEmpty())
        return;

    while (li.hasNext()) {
        li.next();

        LayoutItem *item = new LayoutItem(ui->listLayout);
        Global::KeyboardInfo info = li.value();

        item->setText(info.description);
        item->data = li.key();
        item->info = info;

        // Find current layout index
        if (li.key() == currentLayout)
            currentLayoutItem = item;
    }

    ui->listLayout->sortItems();

    // Set current layout
    if (currentLayoutItem)
        ui->listLayout->setCurrentItem(currentLayoutItem);
    else if (ui->listLayout->count() > 0)
        ui->listLayout->setCurrentRow(0);

    // Trigger signal
    listLayout_currentItemChanged(ui->listLayout->currentItem(), ui->listLayout->currentItem());

    // Set current variant
    if (!currentVariant.isEmpty()) {
        for(int i = 0; i < ui->listVariant->count(); ++i) {
            LayoutItem *item = dynamic_cast<LayoutItem*>(ui->listVariant->item(i));
            if (item && item->data == currentVariant) {
                ui->listVariant->setCurrentItem(item);
                break;
            }
        }
    }

    // Unblock signals
    ui->listLayout->blockSignals(false);
}



void PageKeyboard::updateApplyEnabledState() {
    emit setApplyEnabled(this, ui->listVariant->selectedItems().size() > 0);
}




//###
//### Slots
//###



void PageKeyboard::listLayout_currentItemChanged(QListWidgetItem * current, QListWidgetItem *) {
    LayoutItem *item = dynamic_cast<LayoutItem*>(current);
    if (!item)
        return;

    // Block signals
    ui->listVariant->blockSignals(true);

    QMap< QString, QString > variants = item->info.variants;
    QMapIterator<QString, QString> li(variants);
    LayoutItem *defaultItem = NULL;

    ui->listVariant->clear();

    while (li.hasNext()) {
        li.next();

        item = new LayoutItem(ui->listVariant);
        item->setText(li.key());
        item->data = li.value();

        if (li.value() == "")
            defaultItem = item;
    }

    // Set to default value
    if (defaultItem)
        ui->listVariant->setCurrentItem(defaultItem);

    // Trigger signals
    listVariant_currentItemChanged(ui->listVariant->currentItem(), ui->listVariant->currentItem());

    // Unblock signals
    ui->listVariant->blockSignals(false);

    updateApplyEnabledState();
}



void PageKeyboard::listVariant_currentItemChanged(QListWidgetItem * current, QListWidgetItem *) {
    LayoutItem *layoutItem = dynamic_cast<LayoutItem*>(ui->listLayout->currentItem());
    LayoutItem *variantItem = dynamic_cast<LayoutItem*>(current);

    if (!layoutItem || !variantItem)
        return;

    keyboardPreview.setLayout(layoutItem->data);
    keyboardPreview.setVariant(variantItem->data);

    updateApplyEnabledState();
}



void PageKeyboard::buttonRestore_clicked() {
    // Set to default value pc105
    ui->comboBoxModel->setCurrentIndex(defaultIndex);
}
