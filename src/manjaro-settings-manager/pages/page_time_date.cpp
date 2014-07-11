#include "page_time_date.h"
#include "ui_page_time_date.h"

PageTimeDate::PageTimeDate(QWidget *parent) :
    PageWidget(parent),
    ui(new Ui::PageTimeDate)
{
    ui->setupUi(this);
    setTitel(tr("Time and Date"));
    setIcon(QPixmap(":/images/resources/locale.png"));
}

PageTimeDate::~PageTimeDate()
{
    delete ui;
}
