#ifndef TIMEZONEDIALOG_H
#define TIMEZONEDIALOG_H

#include <QDialog>
#include "TimeZoneWidget.h"

namespace Ui {
class TimeZoneDialog;
}

class TimeZoneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TimeZoneDialog(QWidget *parent = 0);
    ~TimeZoneDialog();
    void init(const QString& initialRegion, const QString& initialZone);
    QString getCurrentLocation();

private:
    Ui::TimeZoneDialog *ui;
    TimeZoneWidget *timeZoneWidget;

    bool blockTimeZoneWidgetSet;
};

#endif // TIMEZONEDIALOG_H
