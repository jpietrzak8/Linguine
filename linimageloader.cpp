#include "linimageloader.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QTreeWidgetItem>
#include <QString>
#include <QByteArray>
#include "linnewsfeedwidgetitem.h"

#include <QDebug>

LinImageLoader::LinImageLoader(
  LinNewsfeedWidgetItem *nwi,
  QString imageUrl,
  QNetworkAccessManager *q)
  : newsfeedItem(nwi),
    treeItem(0),
    qnam(q),
    reply(0)
{
  reply = qnam->get(QNetworkRequest(QUrl(imageUrl)));

  connect(
    reply,
    SIGNAL(finished()),
    this,
    SLOT(loadImage()));
}


LinImageLoader::LinImageLoader(
  QTreeWidgetItem *t,
  QString imageUrl,
  QNetworkAccessManager *q)
  : newsfeedItem(0),
    treeItem(t),
    qnam(q),
    reply(0)
{
  reply = qnam->get(QNetworkRequest(QUrl(imageUrl)));

  connect(
    reply,
    SIGNAL(finished()),
    this,
    SLOT(loadImage()));
}


void LinImageLoader::loadImage()
{
//qDebug() << "error() " << reply->error();
  // support some amount of redirection here:
  QUrl redirectUrl = 
    reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

  if (!redirectUrl.isEmpty())
  {
    // Follow the redirect:
//qDebug() << "redirect: " << redirectUrl;
    reply->deleteLater();
    reply = qnam->get(QNetworkRequest(redirectUrl));

    connect(
      reply,
      SIGNAL(finished()),
      this,
      SLOT(loadImage()));

    return;
  }

  QByteArray ba = reply->readAll();

  if (newsfeedItem)
  {
    newsfeedItem->setImage(ba);
    emit imageUpdated();
  }

  if (treeItem)
  {
    image.loadFromData(ba);
    treeItem->setIcon(0, QIcon(image));
  }

  // Clean up the reply:
  reply->deleteLater();
  reply = 0;
}
