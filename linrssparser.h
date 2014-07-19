#ifndef LINRSSPARSER_H
#define LINRSSPARSER_H

#include <QObject>
#include "linnewsfeedwidgetitem.h"

class QXmlStreamReader;
class QNetworkReply;
class QNetworkAccessManager;

class LinRSSParser: public QObject
{
  Q_OBJECT

public:
  LinRSSParser(
    LinNewsfeedWidgetItem *nwi,
    QString sourseUrl,
    QNetworkAccessManager *qnam);

  void startParsing();

private slots:
  void parseRSSFeed();

  void loadImage();

private:
  void parseRSSChannel(
    QXmlStreamReader &newsReader);

  void parseRSSImage(
    QXmlStreamReader &newsReader);

  void parseRSSItem(
    QXmlStreamReader &newsReader);

  void skipRSSItem(
    QXmlStreamReader &newsReader);

  QString parseRSSText(
    QString elementName,
    QXmlStreamReader &newsReader);

  LinNewsfeedWidgetItem *nwi;
  QString sourceUrl;
  QNetworkAccessManager *qnam;
  QNetworkReply *reply;

  QString channelTitle;
  QString channelLink;
  QString channelSummary;
//  QString itemTitle;
  QString itemGuid;
  QString itemPubDate;
//  QString mediaUrl;

  QString imageUrl;
//  QPixmap image;

  bool imageAlreadySeen;
};

#endif // LINRSSPARSER_H
