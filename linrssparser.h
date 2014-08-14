//
// linrssparser.h
//
// Copyright 2014 by John Pietrzak (jpietrzak8@gmail.com)
//
// This file is part of Linguine.
//
// Linguine is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// Linguine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Linguine; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

#ifndef LINRSSPARSER_H
#define LINRSSPARSER_H

#include <QObject>
#include "linnewsfeedwidgetitem.h"

class QXmlStreamReader;
class QNetworkReply;
class QNetworkAccessManager;
class LinImageLoader;

class LinRSSParser: public QObject
{
  Q_OBJECT

public:
  LinRSSParser(
    LinNewsfeedWidgetItem *nwi,
    QString sourseUrl,
    QNetworkAccessManager *qnam);

  ~LinRSSParser();

  void startParsing();

private slots:
  void parseRSSFeed();

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
//  QString itemPubDate;
//  QString mediaUrl;
//  QString itemSummary;

  QString imageUrl;
//  QPixmap image;
  LinImageLoader *imageLoader;

  bool imageAlreadySeen;
};

#endif // LINRSSPARSER_H
