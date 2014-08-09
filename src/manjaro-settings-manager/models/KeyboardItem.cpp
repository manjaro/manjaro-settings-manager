#include "KeyboardItem.h"

KeyboardItem::KeyboardItem(const QString &key, const QString &description, KeyboardItem *parent)
{
    parentItem_ = parent;
    key_ = key;
    description_ = description;
}

KeyboardItem::~KeyboardItem()
{
    qDeleteAll(childItems_);
}

void KeyboardItem::appendChild(KeyboardItem *item)
{
    childItems_.append(item);
}

QString KeyboardItem::key() const
{
    return key_;
}

QString KeyboardItem::description() const
{
    return description_;
}

KeyboardItem *KeyboardItem::child(int row)
{
    return childItems_.value(row);
}

int KeyboardItem::childCount() const
{
    return childItems_.count();
}

int KeyboardItem::columnCount() const
{
    return 2;
}

int KeyboardItem::row() const
{
    if (parentItem_)
        return parentItem_->childItems_.indexOf(const_cast<KeyboardItem*>(this));

    return 0;
}

KeyboardItem *KeyboardItem::parent()
{
    return parentItem_;
}
