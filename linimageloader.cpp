#include "linimageloader.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QListWidgetItem>
#include <QTreeWidgetItem>
#include <QString>
#include <QByteArray>

#include <QDebug>

LinImageLoader::LinImageLoader(
  QListWidgetItem *i,
  QString imageUrl,
  QNetworkAccessManager *q)
  : listItem(i),
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
  : listItem(0),
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

  image.loadFromData(ba);

  if (listItem)
  {
    listItem->setIcon(QIcon(image));
  }

  if (treeItem)
  {
    treeItem->setIcon(0, QIcon(image));
  }

  // Clean up the reply:
  reply->deleteLater();
  reply = 0;
}
