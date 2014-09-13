#include "LanguageListViewDelegate.h"

#include "models/EnabledLocalesModel.h"

#include <QDebug>

LanguageListViewDelegate::LanguageListViewDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

LanguageListViewDelegate::~LanguageListViewDelegate()
{

}

void LanguageListViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    painter->save();

    /* Draw light blue rectangle when hoover */
    if ((option.state & QStyle::State_MouseOver) &&
        !(option.state & QStyle::State_HasFocus)) {
        painter->fillRect(option.rect, QColor("#F7F7F9"));
    }

    /* Draw line separating delegates */
    painter->setPen(QColor("#ddd"));
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

    /* Draw text */
    QString key = qvariant_cast<QString>(index.data(EnabledLocalesModel::KeyRole));
    QString country = qvariant_cast<QString>(index.data(EnabledLocalesModel::CountryRole));
    QString language = qvariant_cast<QString>(index.data(EnabledLocalesModel::LanguageRole));
    bool isSystemLang = qvariant_cast<bool>(index.data(EnabledLocalesModel::IsSystemLang));


    QFont textFont = option.font;
    painter->setPen(QColor(Qt::black));
    painter->setFont(textFont);
    painter->drawText(option.rect,
                      Qt::AlignCenter,
                      QString("%1 (%2)\n").arg(language).arg(country));
    painter->setPen(QColor(Qt::darkGray));
    painter->drawText(option.rect,
                          Qt::AlignCenter,
                          QString("\n%1").arg(key));

    /* Write "Default" if set as system LANG= */
    if (isSystemLang) {
        textFont.setItalic(true);
        painter->setPen(QColor(Qt::gray));
        painter->setFont(textFont);
        painter->drawText(option.rect,
                          Qt::AlignVCenter | Qt::AlignRight,
                          QString(tr("Default     ")));
    }

    painter->restore();
}

QSize LanguageListViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    Q_UNUSED(option)
    return QSize(200, 50);
}
