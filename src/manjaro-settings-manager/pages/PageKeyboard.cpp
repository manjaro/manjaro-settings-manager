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
    keyboardModel_(new KeyboardModel),
    keyboardProxyModel_(new QSortFilterProxyModel),
    keyboardPreview_(new KeyBoardPreview)
{
    ui->setupUi(this);
    setTitel(tr("Keyboard Settings"));
    setIcon(QPixmap(":/images/resources/keyboard.png"));
    setShowApplyButton(true);

    /* Keyboard preview widget */
    ui->KBPreviewLayout->addWidget(keyboardPreview_);
    keyboardPreview_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    /* Connect signals and slots */
    connect(ui->buttonRestore, &QPushButton::clicked,
            this, &PageKeyboard::buttonRestore_clicked);
    connect(ui->layoutsListView, &QListView::activated,
            this, &PageKeyboard::keyboardLayoutListViewActivated);
    connect(ui->variantsListView, &QListView::activated,
            this, &PageKeyboard::keyboardVariantListViewActivated);

    keyboardProxyModel_->setSourceModel(keyboardModel_);
    keyboardProxyModel_->setSortRole(KeyboardModel::DescriptionRole);
    keyboardProxyModel_->sort(1, Qt::AscendingOrder);

    /* Set model and root index to the layout and variants lists */
    ui->layoutsListView->setModel(keyboardProxyModel_);
    ui->layoutsListView->setModelColumn(1);
    ui->variantsListView->setModel(keyboardProxyModel_);
    ui->variantsListView->setModelColumn(1);
    /* Find root layout index and set it in the layoutsRootView */
    QModelIndexList layoutsRootList = keyboardProxyModel_->match(keyboardProxyModel_->index(0,0),
                                                                KeyboardModel::KeyRole,
                                                                "layouts",
                                                                Qt::MatchFixedString);
    if (layoutsRootList.size() == 1) {
        QModelIndex layoutsRoot = layoutsRootList.first();
        ui->layoutsListView->setRootIndex(layoutsRoot);
    } else {
        qDebug() << "Can't find keyboard layout list";
    }

    /* Set model and root index to the model combo box */
    ui->modelComboBox->setModel(keyboardProxyModel_);
    ui->modelComboBox->setModelColumn(1);
    QModelIndexList modelsRootList = keyboardProxyModel_->match(keyboardProxyModel_->index(0,0),
                                                       KeyboardModel::KeyRole,
                                                       "models", Qt::MatchFixedString);
    if (modelsRootList.size() == 1) {
        QModelIndex modelsRoot = modelsRootList.first();
        ui->modelComboBox->setRootModelIndex(modelsRoot);
    } else {
        qDebug() << "Can't find keyboard model list";
    }
}


PageKeyboard::~PageKeyboard()
{
    delete ui;
    delete keyboardModel_;
    delete keyboardProxyModel_;
    delete keyboardModel_;
}


void PageKeyboard::apply_clicked()
{
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
    if (dialog.processSuccess()) {
        qDebug() << "Your keyboard settings have been saved.";
    } else {
        qDebug() << "Error saving your keyboard settings.";
    }
    activated();
}


void PageKeyboard::activated()
{
    // Default focus
    ui->layoutsListView->setFocus();

    /* Detect current keyboard layout and variant */
    QString currentLayout;
    QString currentVariant;
    QString currentModel;

    if (!keyboardModel_->getCurrentKeyboardLayout(currentLayout, currentVariant, currentModel)) {
       qDebug() << "Failed to determine current keyboard layout";
    }

    /* Select current layout or default in the view */
    if (currentLayout.isEmpty()) {
        currentLayout = "us";
    }
    QModelIndexList layoutIndexList = keyboardProxyModel_->match(ui->layoutsListView->rootIndex().child(0,0),
                                                                 KeyboardModel::KeyRole,
                                                                 currentLayout,
                                                                 Qt::MatchFixedString);

    if (layoutIndexList.size() == 1) {
        QModelIndex layoutIndex = layoutIndexList.first();
        QModelIndex variantRootIndex;
        layoutIndex = keyboardProxyModel_->index(layoutIndex.row(), 1, layoutIndex.parent());
        variantRootIndex = keyboardProxyModel_->index(layoutIndex.row(), 0, layoutIndex.parent());
        ui->layoutsListView->setCurrentIndex(layoutIndex);
        ui->variantsListView->setRootIndex(variantRootIndex);
    } else {
        qDebug() << "Can't find the current keyboard layout";
    }

    /* Select current layout or default in the view */
    if (currentVariant.isEmpty()) {
        currentVariant = "default";
    }
    QModelIndexList variantIndexList = keyboardProxyModel_->match(ui->variantsListView->rootIndex().child(0,0),
                                                                  KeyboardModel::KeyRole,
                                                                  currentVariant,
                                                                  Qt::MatchFixedString);
    if (variantIndexList.size() == 1) {
        QModelIndex variantIndex = variantIndexList.first();
        variantIndex = ui->layoutsListView->model()->index(variantIndex.row(), 1, variantIndex.parent());
        ui->variantsListView->setCurrentIndex(variantIndex);
        emit(ui->variantsListView->activated(variantIndex));
    } else {
        qDebug() << "Can't find the current keyboard variant";
    }

    /* Select current model or default in the combo box */
    if (currentModel.isEmpty()) {
        currentModel = "pc105";
    }
    QModelIndexList modelIndexList = keyboardProxyModel_->match(ui->modelComboBox->rootModelIndex().child(0,0),
                                                                KeyboardModel::KeyRole,
                                                                currentModel,
                                                                Qt::MatchFixedString);
    if (modelIndexList.size() == 1) {
        QModelIndex modelIndex = modelIndexList.first();
        modelIndex = ui->modelComboBox->model()->index(modelIndex.row(), 1, modelIndex.parent());
        ui->modelComboBox->setCurrentIndex(modelIndex.row());
    } else {
        qDebug() << "Can't find the current keyboard model";
    }
}


/*
 * Slots
 */

void PageKeyboard::buttonRestore_clicked()
{
    QModelIndexList modelIndexList = keyboardProxyModel_->match(ui->modelComboBox->rootModelIndex().child(0,0),
                                                                KeyboardModel::KeyRole,
                                                                "pc105",
                                                                Qt::MatchFixedString);
    if (modelIndexList.size() == 1) {
        QModelIndex modelIndex = modelIndexList.first();
        modelIndex = keyboardProxyModel_->index(modelIndex.row(), 1, modelIndex.parent());
        ui->modelComboBox->setCurrentIndex(modelIndex.row());
    } else {
        qDebug() << "Can't find the current keyboard model";
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
            qDebug() << "Can't find the default keyboard variant";
        }
    }
}


void PageKeyboard::keyboardVariantListViewActivated(const QModelIndex &index)
{
    if (index.isValid() && index.parent().isValid()) {
        keyboardPreview_->setLayout(index.parent().data(KeyboardModel::KeyRole).toString());
        keyboardPreview_->setVariant(index.data(KeyboardModel::KeyRole).toString());
    }
}
