#include "kernel_list_view_delegate.h"

KernelListViewDelegate::KernelListViewDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
     stateInstallButton_ = QStyle::State_Enabled;
     stateInfoButton_ = QStyle::State_Enabled;
}

KernelListViewDelegate::~KernelListViewDelegate()
{
}

//alocate each item size in listview.
QSize KernelListViewDelegate::sizeHint(const QStyleOptionViewItem & option,
                              const QModelIndex & index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(300, 70);
}

void KernelListViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
 {
    QStyledItemDelegate::paint(painter,option,index);

    painter->save();

    int const padding = 5;

    QString package = qvariant_cast<QString>(index.data(KernelModel::PackageRole));
    QString version = qvariant_cast<QString>(index.data(KernelModel::VersionRole));
    QString name = ("Linux " + version);
    bool isAvailable = qvariant_cast<bool>(index.data(KernelModel::IsAvailableRole));
    bool isInstalled = qvariant_cast<bool>(index.data(KernelModel::IsInstalledRole));
    bool isLts = qvariant_cast<bool>(index.data(KernelModel::IsLtsRole));
    bool isRecommended = qvariant_cast<bool>(index.data(KernelModel::IsRecommendedRole));
    bool isRunning = qvariant_cast<bool>(index.data(KernelModel::IsRunningRole));

    if ((index.row() % 2) != 0)
        painter->fillRect(option.rect, QColor("#F7F7F9"));

    painter->setPen(QColor("#ddd"));
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

    /* draw name */
    QFont nameFont = QFont();
    nameFont.setPointSize(nameFont.pointSize() * 1.75);
    QFontMetrics nameFontMetrics(nameFont);
    QSize nameSize = nameFontMetrics.size(Qt::TextSingleLine, name);
    QRectF nameRect(QPointF(), nameSize);

    painter->setPen(QColor("#333"));
    nameRect.moveTopLeft(option.rect.topLeft() + QPoint(padding, padding));
    painter->setFont(nameFont);
    painter->drawText(nameRect, Qt::TextSingleLine, name);

    /* draw package */
    QFont packageFont = QFont();
    packageFont.setPointSize(packageFont.pointSize() * 0.8);
    QFontMetrics packageFontMetrics(packageFont);
    QSize packageSize = packageFontMetrics.size(Qt::TextSingleLine, package);
    QRectF packageRect(QPointF(), packageSize);

    painter->setPen(QColor("#333"));
    packageRect.moveTopLeft(nameRect.bottomLeft());
    painter->setFont(packageFont);
    painter->drawText(packageRect, Qt::TextSingleLine, package);

    /* draw middle labels */
    QString ltsStr(tr("LTS"));
    QString recommendedStr(tr("Recommended"));
    QString runningStr(tr("Running"));
    QString installedStr(tr("Installed"));
    QString unsupportedStr(tr("Unsupported"));
    QString customStr(tr("Custom"));
    QStringList labelStringList = (QStringList() << ltsStr << recommendedStr
                                    << runningStr << installedStr << unsupportedStr << customStr);

    QFont labelFont = QFont();
    labelFont.setPointSize(labelFont.pointSize());
    QFontMetrics labelFontMetrics(labelFont);

    int labelWidth = 0;
    int labelHeight = 0;
    for (auto str : labelStringList) {
        QSize labelSize = labelFontMetrics.size(Qt::TextSingleLine, str);
        if (labelSize.width() > labelWidth)
            labelWidth = labelSize.width();
        if (labelSize.height() > labelHeight)
            labelHeight = labelSize.height();
    }
    QRectF labelRect(QPointF(), QSize(labelWidth + padding*2, labelHeight));

    /* draw first column (lts, recommended) */
    labelRect.moveTopRight(QPointF(option.rect.center().x(), option.rect.top() + padding));
    painter->setFont(labelFont);
    if (isLts) {        
        painter->fillRect(labelRect, QColor("#d9edf7"));
        painter->setPen(QColor("#bce8f1"));
        painter->drawRect(labelRect);
        painter->setPen(QColor("#31708f"));
        labelRect.moveTopLeft(labelRect.topLeft() + QPoint(0, 2));
        painter->drawText(labelRect, Qt::AlignCenter, ltsStr);
        labelRect.moveTopLeft(labelRect.topLeft() + QPoint(0, labelHeight + 2));
    }

    if (isRecommended) {
        painter->fillRect(labelRect, QColor("#d9edf7"));
        painter->setPen(QColor("#bce8f1"));
        painter->drawRect(labelRect);
        labelRect.moveTopLeft(labelRect.topLeft() + QPoint(0, 2));
        painter->setPen(QColor("#31708f"));
        painter->drawText(labelRect, Qt::AlignCenter, recommendedStr);
    }

    /* draw second column (running, installed, unsupported) */
    labelRect.moveTopLeft(QPointF(option.rect.center().x() + 5, option.rect.top() + padding));
    if (isRunning) {
        painter->fillRect(labelRect, QColor("#d6e9c6"));
        painter->setPen(QColor("#dff0d8"));
        painter->drawRect(labelRect);
        painter->setPen(QColor("#3c763d"));
        labelRect.moveTopLeft(labelRect.topLeft() + QPoint(0, 2));
        painter->drawText(labelRect, Qt::AlignCenter, runningStr);
        labelRect.moveTopLeft(labelRect.topLeft() + QPoint(0, labelHeight + 2));
    }

    if (isInstalled) {
        painter->fillRect(labelRect, QColor("#d6e9c6"));
        painter->setPen(QColor("#dff0d8"));
        painter->drawRect(labelRect);
        painter->setPen(QColor("#3c763d"));
        labelRect.moveTopLeft(labelRect.topLeft() + QPoint(0, 2));
        painter->drawText(labelRect, Qt::AlignCenter, installedStr);
        labelRect.moveTopLeft(labelRect.topLeft() + QPoint(0, labelHeight + 2));
    }

    if (!isAvailable && isInstalled) {
        painter->fillRect(labelRect, QColor("#f2dede"));
        painter->setPen(QColor("#ebccd1"));
        painter->drawRect(labelRect);
        painter->setPen(QColor("#a94442"));
        labelRect.moveTopLeft(labelRect.topLeft() + QPoint(0, 2));
        painter->drawText(labelRect, Qt::AlignCenter, unsupportedStr);
    }

    /* draw right side buttons */
    QString removeStr(tr("Remove"));
    QString installStr(tr("Install"));
    QString infoStr(tr("Changelog"));
    QStringList buttonStringList = (QStringList() << removeStr << installStr << infoStr);

    QFont buttonFont = QFont();
    QFontMetrics buttonFontMetrics(buttonFont);

    int buttonWidth = 0;
    int buttonHeight = 0;
    for (auto str : buttonStringList) {
        QSize buttonSize = buttonFontMetrics.size(Qt::TextSingleLine, str);
        if (buttonSize.width() > buttonWidth)
            buttonWidth = buttonSize.width();
        if (buttonSize.height() > buttonHeight)
            buttonHeight = buttonSize.height();
    }

    /* Draw remove/install button */
    QRectF buttonRect(QPointF(), QSize(buttonWidth + 20, buttonHeight + 8));
    buttonRect.moveTopRight(QPointF(option.rect.right() - padding,
                                    option.rect.center().y() - buttonHeight - 10));

    QStyleOptionButton button;
    button.rect = buttonRect.toRect();
    if (isInstalled) {
        button.text = removeStr;
    } else {
        button.text = installStr;
    }
    button.state = stateInstallButton_ | QStyle::State_Enabled;
    painter->setFont(buttonFont);
    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);

    /* Draw changelog/information button */
    buttonRect.moveTopRight(QPointF(option.rect.right() - padding,
                                    option.rect.center().y() + 2));
    QStyleOptionButton infoButton;
    infoButton.rect = buttonRect.toRect();
    infoButton.text = infoStr;
    infoButton.state = stateInfoButton_ | QStyle::State_Enabled;
    painter->setFont(buttonFont);
    QString changelog = QString(":/kernel_changelogs/resources/%1.html").arg(package);
    if (QFile(changelog).exists()) {
        QApplication::style()->drawControl(QStyle::CE_PushButton, &infoButton, painter);
    }

    painter->restore();
 }


bool KernelListViewDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index)
{
    Q_UNUSED(model)
    if (event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::MouseButtonRelease) {
    } else {
        stateInstallButton_ = QStyle::State_Raised;
        stateInfoButton_ = QStyle::State_Raised;
        return true;
    }

    /* Calculate where the buttons are */
    QString removeStr(tr("Remove"));
    QString installStr(tr("Install"));
    QString infoStr(tr("Changelog"));
    QStringList buttonStringList = (QStringList() << removeStr << installStr << infoStr);

    QFont buttonFont = QFont();
    QFontMetrics buttonFontMetrics(buttonFont);

    int buttonWidth = 0;
    int buttonHeight = 0;
    for (auto str : buttonStringList) {
        QSize buttonSize = buttonFontMetrics.size(Qt::TextSingleLine, str);
        if (buttonSize.width() > buttonWidth)
            buttonWidth = buttonSize.width();
        if (buttonSize.height() > buttonHeight)
            buttonHeight = buttonSize.height();
    }
    int const padding = 5;
    QRectF installButtonRect(QPointF(), QSize(buttonWidth + 20, buttonHeight + 8));
    installButtonRect.moveTopRight(QPointF(option.rect.right() - padding,
                                    option.rect.center().y() - buttonHeight - 10));

    QRectF infoButtonRect(QPointF(), QSize(buttonWidth + 20, buttonHeight + 8));
    infoButtonRect.moveTopRight(QPointF(option.rect.right() - padding,
                                    option.rect.center().y() + 2));

    /* Raise or sunk buttons, and emit signals */
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
    if (!installButtonRect.contains(mouseEvent->pos()) && !infoButtonRect.contains(mouseEvent->pos())) {
        stateInstallButton_ = QStyle::State_Raised;
        stateInfoButton_ = QStyle::State_Raised;
        return true;
    }
    if (event->type() == QEvent::MouseButtonPress) {
        if (installButtonRect.contains(mouseEvent->pos()))
            stateInstallButton_ = QStyle::State_Sunken;
        if (infoButtonRect.contains(mouseEvent->pos()))
            stateInfoButton_ = QStyle::State_Sunken;
    } else if (event->type() == QEvent::MouseButtonRelease) {
        stateInstallButton_ = QStyle::State_Raised;
        stateInfoButton_ = QStyle::State_Raised;
        if (installButtonRect.contains(mouseEvent->pos()))
            emit installButtonClicked(index);
        if (infoButtonRect.contains(mouseEvent->pos()))
            emit infoButtonClicked(index);
    }

    return true;
}
