#ifndef PAGE_NOTIFICATIONS_H
#define PAGE_NOTIFICATIONS_H

#include <QSettings>
#include <QWidget>
#include "widgets/pagewidget.h"

namespace Ui {
class PageNotifications;
}

class PageNotifications : public PageWidget
{
    Q_OBJECT

public:
    explicit PageNotifications(QWidget *parent = 0);
    ~PageNotifications();

    void activated();
    void apply_clicked();

private:
    Ui::PageNotifications *ui;

protected slots:
    void unsupportedKernelStateBoxChanged(int);
    void newKernelStateBoxChanged(int);
};

#endif // PAGE_NOTIFICATIONS_H
