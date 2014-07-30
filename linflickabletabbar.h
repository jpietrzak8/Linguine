#ifndef LINFLICKABLETABBAR_H
#define LINFLICKABLETABBAR_H

#include <QListWidget>

class LinNewsfeedWidgetItem;

class LinFlickableTabBar: public QListWidget
{
  Q_OBJECT

public:
  LinFlickableTabBar(
    QWidget *parent);

  bool matchesCurrentCategory(
    LinNewsfeedWidgetItem *nwi);
};

#endif // LINFLICKABLETABBAR_H
