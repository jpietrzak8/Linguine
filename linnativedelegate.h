#ifndef LINNATIVEDELEGATE_H
#define LINNATIVEDELEGATE_H

#include <QStyledItemDelegate>

class LinNativeDelegate: public QStyledItemDelegate
{
public:
  LinNativeDelegate(
    QObject *parent = 0);

  void paint(
    QPainter *painter,
    const QStyleOptionViewItem &viewItem,
    const QModelIndex &index) const;

  QSize sizeHint(
    const QStyleOptionViewItem &viewItem,
    const QModelIndex &index) const;
};

#endif // LINNATIVEDELEGATE_H
