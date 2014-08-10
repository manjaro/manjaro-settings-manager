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

#include <QDebug>

PageKeyboard::PageKeyboard(QWidget *parent) :
    PageWidget(parent),
    ui(new Ui::PageKeyboard),
    keyboardModel(new KeyboardModel)
{
    ui->setupUi(this);
    setTitel(tr("Keyboard Settings"));
    setIcon(QPixmap(":/images/resources/keyboard.png"));
    setShowApplyButton(true);

    /* Keyboard preview widget */
    ui->KBPreviewLayout->addWidget(&keyboardPreview);

    /* Connect signals and slots */
    connect(ui->buttonRestore, &QPushButton::clicked,
            this, &PageKeyboard::buttonRestore_clicked);
    connect(ui->layoutsListView, &QListView::activated,
            this, &PageKeyboard::keyboardLayoutListViewActivated);
    connect(ui->variantsListView, &QListView::activated,
            this, &PageKeyboard::keyboardVariantListViewActivated);

    QSortFilterProxyModel *keyboardProxyModel = new QSortFilterProxyModel(this);
    keyboardProxyModel->setSourceModel(keyboardModel);
    keyboardProxyModel->setSortRole(KeyboardModel::DescriptionRole);
    keyboardProxyModel->sort(1, Qt::AscendingOrder);

    /* Set model, root index and default selection to the layout and variants lists */
    ui->layoutsListView->setModel(keyboardProxyModel);
    ui->layoutsListView->setModelColumn(1);
    ui->variantsListView->setModel(keyboardProxyModel);
    ui->variantsListView->setModelColumn(1);
    /* Find root layout index and set it in the layoutsRootView */
    QModelIndexList layoutsRootList = keyboardProxyModel->match(keyboardProxyModel->index(0,0),
                                                                KeyboardModel::KeyRole,
                                                                "layouts",
                                                                Qt::MatchFixedString);
    if (layoutsRootList.size() == 1) {
        QModelIndex layoutsRoot = layoutsRootList.first();
        ui->layoutsListView->setRootIndex(layoutsRoot);
        QModelIndexList defaultLayoutList = keyboardProxyModel->match(layoutsRoot.child(0,0),
                                                                      KeyboardModel::KeyRole,
                                                                      "us", Qt::MatchFixedString);
        if (defaultLayoutList.size() == 1) {
            QModelIndex defaultLayout = defaultLayoutList.first();
            defaultLayout = keyboardProxyModel->index(defaultLayout.row(), 1, defaultLayout.parent());
            ui->layoutsListView->setCurrentIndex(defaultLayout);
            emit(ui->layoutsListView->activated(defaultLayout));
        } else {
            qDebug() << "Can't find default layout 'us'";
        }
    } else {
        qDebug() << "Can't find layout root index";
    }

    /* Set model, root index and default selection to the model combo box */
    ui->modelComboBox->setModel(keyboardProxyModel);
    ui->modelComboBox->setModelColumn(1);
    QModelIndexList modelsRootList = keyboardProxyModel->match(keyboardProxyModel->index(0,0),
                                                       KeyboardModel::KeyRole,
                                                       "models", Qt::MatchFixedString);
    if (modelsRootList.size() == 1) {
        QModelIndex modelsRoot = modelsRootList.first();
        ui->modelComboBox->setRootModelIndex(modelsRoot);
        QModelIndexList defaultModelList = keyboardProxyModel->match(modelsRoot.child(0,0),
                                                            KeyboardModel::KeyRole,
                                                            "pc105", Qt::MatchFixedString);
        if (defaultModelList.size() == 1) {
            QModelIndex defaultModel = defaultModelList.first();
            ui->modelComboBox->setCurrentIndex(defaultModel.row());
        } else {
            qDebug() << "Can't find default model 'pc105'";
        }
    } else {
        qDebug() << "Can't find model root index";
    }
}


PageKeyboard::~PageKeyboard()
{
    delete ui;
}


void PageKeyboard::apply_clicked() {

    QString model = ui->modelComboBox->itemData(ui->modelComboBox->currentIndex(), KeyboardModel::KeyRole).toString();
    QString layout = ui->layoutsListView->currentIndex().data(KeyboardModel::KeyRole).toString();
    QString variant = ui->variantsListView->currentIndex().data(KeyboardModel::KeyRole).toString();

    if (QString::compare(variant, "default") == 0) {
        variant = "";
    }

    // Set Xorg keyboard layout
    system(QString("setxkbmap -model \"%1\" -layout \"%2\" -variant \"%3\"").arg(model, layout, variant).toUtf8());

    ApplyDialog dialog(this);
    dialog.exec("keyboardctl", QStringList() << "--set-layout" << model << layout << variant, tr("Setting new keyboard layout..."), true);

    emit closePage(this);
}


void PageKeyboard::activated() {
    // Default focus
    ui->layoutsListView->setFocus();


    /* Detect current keyboard layout and variant */
    QString currentLayout;
    QString currentVariant;
    QString currentModel;

    if (!keyboardModel->getCurrentKeyboardLayout(currentLayout, currentVariant, currentModel)) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to determine current keyboard layout!"), QMessageBox::Ok, QMessageBox::Ok);
    }

    /* Select current layout in the view */
    QModelIndexList layoutIndexList = ui->layoutsListView->model()->match(ui->layoutsListView->rootIndex().child(0,0),
                                                                          KeyboardModel::KeyRole,
                                                                          currentLayout,
                                                                          Qt::MatchFixedString);

    if (layoutIndexList.size() == 1) {
        QModelIndex layoutIndex = layoutIndexList.first();
        layoutIndex = ui->layoutsListView->model()->index(layoutIndex.row(), 1, layoutIndex.parent());
        ui->layoutsListView->setCurrentIndex(layoutIndex);
        emit(ui->layoutsListView->activated(layoutIndex));
    } else {
        qDebug() << "Can't find the current layout in the model";
    }

    /* Select current layout in the view */
    if (!currentVariant.isEmpty()) {
        QModelIndexList variantIndexList = ui->layoutsListView->model()->match(ui->variantsListView->rootIndex().child(0,0),
                                                            KeyboardModel::KeyRole,
                                                            currentVariant,
                                                            Qt::MatchFixedString);
        if (variantIndexList.size() == 1) {
            QModelIndex variantIndex = variantIndexList.first();
            variantIndex = ui->layoutsListView->model()->index(variantIndex.row(), 1, variantIndex.parent());
            ui->variantsListView->setCurrentIndex(variantIndex);
            emit(ui->variantsListView->activated(variantIndex));
        } else {
            qDebug() << "Can't find the current variant in the model";
        }
    }

    /* Select current model in the combo box */
    if (!currentModel.isEmpty()) {
        QModelIndexList modelIndexList = ui->modelComboBox->model()->match(ui->modelComboBox->rootModelIndex().child(0,0),
                                                            KeyboardModel::KeyRole,
                                                            currentModel,
                                                            Qt::MatchFixedString);
        if (modelIndexList.size() == 1) {
            QModelIndex modelIndex = modelIndexList.first();
            modelIndex = ui->modelComboBox->model()->index(modelIndex.row(), 1, modelIndex.parent());
            ui->modelComboBox->setCurrentIndex(modelIndex.row());
        } else {
            qDebug() << "Can't find the current keyboard model in the model";
        }
    }
}


/*
 * Slots
 */

void PageKeyboard::buttonRestore_clicked() {
    QModelIndexList modelIndexList = ui->modelComboBox->model()->match(ui->modelComboBox->rootModelIndex().child(0,0),
                                                        KeyboardModel::KeyRole,
                                                        "pc105",
                                                        Qt::MatchFixedString);
    if (modelIndexList.size() == 1) {
        QModelIndex modelIndex = modelIndexList.first();
        modelIndex = ui->modelComboBox->model()->index(modelIndex.row(), 1, modelIndex.parent());
        ui->modelComboBox->setCurrentIndex(modelIndex.row());
    } else {
        qDebug() << "Can't find the current keyboard model in the model";
    }
}


/*
 * Change variant list view root to the selected layout root
 * and select the "Default" keyboard
 */
void PageKeyboard::keyboardLayoutListViewActivated(const QModelIndex &index)
{
    if (index.isValid()) {
        /* Change QModelIndex column to 0 from 1 */
        QModelIndex newIndex = index.model()->index(index.row(), 0, index.parent());
        ui->variantsListView->setRootIndex(newIndex);
        /* Select "Default" keyboard layout */
        QModelIndexList variantDefaultList = index.model()->match(newIndex.child(0,0),
                                                          KeyboardModel::KeyRole,
                                                          "default",
                                                          Qt::MatchFixedString);
        if (variantDefaultList.size() == 1) {
            QModelIndex variantDefault = variantDefaultList.first();
            variantDefault = index.model()->index(variantDefault.row(), 1, variantDefault.parent());
            ui->variantsListView->setCurrentIndex(variantDefault);
            /* Emit activated(), to update keyboardPreview */
            emit(ui->variantsListView->activated(variantDefault));
        } else {
            qDebug() << "Can't find the current default variant in the model";
        }
    }
}


void PageKeyboard::keyboardVariantListViewActivated(const QModelIndex &index)
{
    if (index.isValid() && index.parent().isValid()) {
        keyboardPreview.setLayout(index.parent().data(KeyboardModel::KeyRole).toString());
        keyboardPreview.setVariant(index.data(KeyboardModel::KeyRole).toString());
    }
}
