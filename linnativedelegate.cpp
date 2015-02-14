#include "linnativedelegate.h"

#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QPixmap>
#include <QMaemo5Style>

#include <QDebug>

LinNativeDelegate::LinNativeDelegate(
  QObject *parent)
  : QStyledItemDelegate(parent)
{
}


void LinNativeDelegate::paint(
  QPainter *painter,
  const QStyleOptionViewItem &viewItem,
  const QModelIndex &index) const
{
  // Start by saving the current painter preferences:
  painter->save();

  int xOffset = 0;
  int yOffset = 5; // default padding value

  // The height depends on the font size.  Currently, assuming exactly 3
  // lines of text...
  QFontMetrics fm = painter->fontMetrics();
  int linespacing = fm.lineSpacing();
  if ((linespacing * 3) < 90)
  {
    yOffset = (100 - (linespacing * 3)) / 2; // increase the padding
  }

  // The initial icon:
  QPixmap newsPixmap = qvariant_cast<QPixmap>(index.data(Qt::UserRole + 2));
  if (!newsPixmap.isNull())
  {
    xOffset = 100; // Icon region is 100 pixels

    // Add padding above and below pixmap (which should be < 100 px tall):
    int pixmapPad = (viewItem.rect.height() - newsPixmap.height()) / 2;

    painter->drawPixmap(
      viewItem.rect.x(),
      viewItem.rect.y() + pixmapPad,
      newsPixmap);
  }

  QRect textRect = viewItem.rect;

  // The user-defined name:
  painter->setPen(QMaemo5Style::standardColor("ActiveTextColor"));
  textRect.setX(textRect.x() + xOffset);
  textRect.setY(textRect.y() + yOffset);
  textRect.setHeight(linespacing);
  painter->drawText(
    textRect,
    Qt::AlignLeft,
    index.data(Qt::UserRole).toString());

  // The title:
  yOffset += linespacing;
  painter->setPen(QMaemo5Style::standardColor("DefaultTextColor"));
//  textRect.setY(textRect.y() + linespacing);
  textRect = viewItem.rect;
  textRect.setX(textRect.x() + xOffset);
  textRect.setY(textRect.y() + yOffset);
  textRect.setHeight(linespacing);
  painter->drawText(
    textRect,
    Qt::AlignLeft,
    index.data(Qt::DisplayRole).toString());

  // The timestamp:
  yOffset += linespacing;
  painter->setPen(QMaemo5Style::standardColor("SecondaryTextColor"));
  textRect = viewItem.rect;
  textRect.setX(textRect.x() + xOffset);
  textRect.setY(textRect.y() + yOffset);
  textRect.setHeight(linespacing);
  painter->drawText(
    textRect,
    Qt::AlignLeft,
    index.data(Qt::UserRole + 1).toString());

  // Restore the painter:
  painter->restore();
}


QSize LinNativeDelegate::sizeHint(
  const QStyleOptionViewItem &viewItem,
  const QModelIndex &index) const
{
  QFontMetrics fm(viewItem.font);

  // For now, we'll assume exactly three lines of text, plus 5 px padding
  // above and below:
  int height = fm.lineSpacing() * 3 + 10;

  // Ensure at least a height of 100 px:
  if (height < 100) height = 100;

  return QSize(viewItem.rect.width(), height);
}
