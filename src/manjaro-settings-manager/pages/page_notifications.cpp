#include "page_notifications.h"
#include "ui_page_notifications.h"

PageNotifications::PageNotifications(QWidget *parent) :
    PageWidget(parent),
    ui(new Ui::PageNotifications)
{
    ui->setupUi(this);
    setTitel(tr("Notifications"));
    setIcon(QPixmap(":/images/resources/notifications.png"));
    setShowApplyButton(true);

    connect(ui->checkUnsupportedKernelBox, SIGNAL(stateChanged(int)), this, SLOT(unsupportedKernelStateBoxChanged(int)));
    connect(ui->checkNewKernelBox, SIGNAL(stateChanged(int)), this, SLOT(newKernelStateBoxChanged(int)));
}

PageNotifications::~PageNotifications()
{
    delete ui;
}


/***********
 * PRIVATE
 **********/
void PageNotifications::activated()
{
    QSettings settings("manjaro", "manjaro-settings-manager");
    bool checkLanguagePackage = settings.value("notifications/checkLanguagePackages", true).toBool();
    bool checkUnsupportedKernel = settings.value("notifications/checkUnsupportedKernel", true).toBool();
    bool checkUnsupportedKernelRunning = settings.value("notifications/checkUnsupportedKernelRunning", false).toBool();
    bool checkNewKernel = settings.value("notifications/checkNewKernel", true).toBool();
    bool checkNewKernelLts = settings.value("notifications/checkNewKernelLts", false).toBool();
    bool checkNewKernelRecommended = settings.value("notifications/checkNewKernelRecommended", false).toBool();

    ui->checkLanguagePackage->setChecked(checkLanguagePackage);
    ui->checkUnsupportedKernelBox->setChecked(checkUnsupportedKernel);
    ui->checkUnsupportedKernelRunningBox->setChecked(checkUnsupportedKernelRunning);
    ui->checkNewKernelBox->setChecked(checkNewKernel);
    ui->checkNewKernelLtsBox->setChecked(checkNewKernelLts);
    ui->checkNewKernelRecommendedBox->setChecked(checkNewKernelRecommended);

    if(!checkUnsupportedKernel)
        ui->checkUnsupportedKernelRunningBox->setEnabled(false);
    if(!checkNewKernel) {
        ui->checkNewKernelLtsBox->setEnabled(false);
        ui->checkNewKernelRecommendedBox->setEnabled(false);
    }
}


void PageNotifications::apply_clicked()
{
    bool checkLanguagePackage = ui->checkLanguagePackage->isChecked();
    bool checkUnsupportedKernel = ui->checkUnsupportedKernelBox->isChecked();
    bool checkUnsupportedKernelRunning = ui->checkUnsupportedKernelRunningBox->isChecked();
    bool checkNewKernel = ui->checkNewKernelBox->isChecked();
    bool checkNewKernelLts = ui->checkNewKernelLtsBox->isChecked();
    bool checkNewKernelRecommended = ui->checkNewKernelRecommendedBox->isChecked();

    QSettings settings("manjaro", "manjaro-settings-manager");
    settings.setValue("notifications/checkLanguagePackages", checkLanguagePackage);
    settings.setValue("notifications/checkUnsupportedKernel", checkUnsupportedKernel);
    settings.setValue("notifications/checkUnsupportedKernelRunning", checkUnsupportedKernelRunning);
    settings.setValue("notifications/checkNewKernel", checkNewKernel);
    settings.setValue("notifications/checkNewKernelLts", checkNewKernelLts);
    settings.setValue("notifications/checkNewKernelRecommended", checkNewKernelRecommended);

    emit closePage(this);
}


/***********
 * PROTECTED SLOTS
 **********/
void PageNotifications::unsupportedKernelStateBoxChanged(int checkState) {
    switch (checkState) {
        case Qt::Unchecked : ui->checkUnsupportedKernelRunningBox->setEnabled(false);
                             break;
        case Qt::Checked   : ui->checkUnsupportedKernelRunningBox->setEnabled(true);
    }
}


void PageNotifications::newKernelStateBoxChanged(int checkState)
{
    switch (checkState) {
        case Qt::Unchecked : ui->checkNewKernelLtsBox->setEnabled(false);
                             ui->checkNewKernelRecommendedBox->setEnabled(false);
                             break;
        case Qt::Checked   : ui->checkNewKernelLtsBox->setEnabled(true);
                             ui->checkNewKernelRecommendedBox->setEnabled(true);
    }
}
