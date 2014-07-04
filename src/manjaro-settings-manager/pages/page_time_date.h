#ifndef PAGE_TIME_DATE_H
#define PAGE_TIME_DATE_H

#include "widgets/pagewidget.h"

namespace Ui {
class PageTimeDate;
}

class PageTimeDate : public PageWidget
{
    Q_OBJECT

public:
    explicit PageTimeDate(QWidget *parent = 0);
    ~PageTimeDate();

private:
    Ui::PageTimeDate *ui;
};

#endif // PAGE_TIME_DATE_H
