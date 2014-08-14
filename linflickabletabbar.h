#ifndef LINFLICKABLETABBAR_H
#define LINFLICKABLETABBAR_H

#include <QListWidget>

class LinNewsfeedWidgetItem;

class LinFlickableTabBar: public QListWidget
{
  Q_OBJECT

public:
  explicit LinFlickableTabBar(
    QWidget *parent = 0);

  bool matchesCurrentCollection(
    LinNewsfeedWidgetItem *nwi);
};

#endif // LINFLICKABLETABBAR_H
