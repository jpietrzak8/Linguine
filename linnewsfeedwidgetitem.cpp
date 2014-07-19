#include "linnewsfeedwidgetitem.h"

#include <QSettings>
#include "LinRSSParser.h"

#include <QDebug>


LinNewsfeedWidgetItem::LinNewsfeedWidgetItem(
  QString n,
  QString sourceUrl,
  Frequency f,
  ContentType c,
  MediaType m,
  QNetworkAccessManager *qnam)
  : name(n),
    frequency(f),
    category(c),
    media(m)
{
  parser = new LinRSSParser(this, sourceUrl, qnam);
}


LinNewsfeedWidgetItem::LinNewsfeedWidgetItem(
  QString n,
  QString sourceUrl,
  Frequency f,
  ContentType c,
  MediaType m,
  QSettings &settings,
  QNetworkAccessManager *qnam)
  : name(n),
    frequency(f),
    category(c),
    media(m)
{
  parser = new LinRSSParser(this, sourceUrl, qnam);

  settings.setValue("name", name);
  settings.setValue("url", sourceUrl);
  settings.setValue("frequency", frequency);
  settings.setValue("category", category);
  settings.setValue("media", media);
}


void LinNewsfeedWidgetItem::parseRSS()
{
  parser->startParsing();
}


void LinNewsfeedWidgetItem::setImage(
  const QByteArray &ba)
{
  image.loadFromData(ba);

  setIcon(QIcon(image));
}


void LinNewsfeedWidgetItem::setItemTitle(
  QString s)
{
  itemTitle = s;
  setText(itemTitle);
}


void LinNewsfeedWidgetItem::setMediaUrl(
  QString s)
{
  mediaUrl = s;
}


QPixmap LinNewsfeedWidgetItem::getImage()
{
  return image;
}


QString LinNewsfeedWidgetItem::getItemTitle()
{
  return itemTitle;
}


QString LinNewsfeedWidgetItem::getMediaUrl()
{
  return mediaUrl;
}
