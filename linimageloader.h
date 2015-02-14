#ifndef LINIMAGELOADER_H
#define LINIMAGELOADER_H

#include <QObject>

#include <QPixmap>

class QNetworkAccessManager;
class QNetworkReply;
class QTreeWidgetItem;
class QString;
class LinNewsfeedWidgetItem;

class LinImageLoader: public QObject
{
  Q_OBJECT

public:
  LinImageLoader(
    LinNewsfeedWidgetItem *nwi,
    QString imageUrl,
    QNetworkAccessManager *qnam);

  LinImageLoader(
    QTreeWidgetItem *treeItem,
    QString imageUrl,
    QNetworkAccessManager *qnam);

signals:
  void imageUpdated();

private slots:
  void loadImage();

private:
  QPixmap image;

  LinNewsfeedWidgetItem *newsfeedItem;
  QTreeWidgetItem *treeItem;
  QNetworkAccessManager *qnam;
  QNetworkReply *reply;
};

#endif // LINIMAGELOADER_H
