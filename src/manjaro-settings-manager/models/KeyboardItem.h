#ifndef KEYBOARDITEM_H
#define KEYBOARDITEM_H

#include <QtCore/QVariant>

class KeyboardItem
{
public:
    explicit KeyboardItem(const QString &key, const QString &description, KeyboardItem *parent = 0);
    ~KeyboardItem();

    void appendChild(KeyboardItem *item);

    QString key() const;
    QString description() const;

    KeyboardItem *child(int row);
    int childCount() const;
    int columnCount() const;
    int row() const;
    KeyboardItem *parent();

private:
    QString key_;
    QString description_;

    QList<KeyboardItem*> childItems_;
    KeyboardItem *parentItem_;
};

#endif // KEYBOARDITEM_H
