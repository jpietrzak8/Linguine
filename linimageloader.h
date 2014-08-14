#ifndef LINIMAGELOADER_H
#define LINIMAGELOADER_H

#include <QObject>

#include <QPixmap>

class QNetworkAccessManager;
class QNetworkReply;
class QListWidgetItem;
class QTreeWidgetItem;
class QString;

class LinImageLoader: public QObject
{
  Q_OBJECT

public:
  LinImageLoader(
    QListWidgetItem *listItem,
    QString imageUrl,
    QNetworkAccessManager *qnam);

  LinImageLoader(
    QTreeWidgetItem *treeItem,
    QString imageUrl,
    QNetworkAccessManager *qnam);

private slots:
  void loadImage();

private:
  QPixmap image;

  QListWidgetItem *listItem;
  QTreeWidgetItem *treeItem;
  QNetworkAccessManager *qnam;
  QNetworkReply *reply;
};

#endif // LINIMAGELOADER_H
