//
// linrssparser.cpp
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

#include "linrssparser.h"

#include <QXmlStreamReader>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QMaemo5InformationBox>

#include <QDebug>

#define ITUNES_NS "http://www.itunes.com/dtds/podcast-1.0.dtd"

LinRSSParser::LinRSSParser(
  LinNewsfeedWidgetItem *item,
  QString s,
  QNetworkAccessManager *manager)
  : nwi(item),
    sourceUrl(s),
    qnam(manager),
    reply(0),
    imageAlreadySeen(false)
{
}


void LinRSSParser::startParsing()
{
  reply = qnam->get(QNetworkRequest(QUrl(sourceUrl)));

  connect(
    reply,
    SIGNAL(finished()),
    this,
    SLOT(parseRSSFeed()));
}


void LinRSSParser::parseRSSFeed()
{
//qDebug() << "parsing reply";
  QXmlStreamReader newsReader(reply);

  while (!newsReader.atEnd())
  {
    newsReader.readNext();

    if (newsReader.isStartElement())
    {
      if (newsReader.name() == "channel")
      {
//qDebug() << "found channel";
        parseRSSChannel(newsReader);
      }
    }
  }

  if (newsReader.hasError())
  {
    QString err;
    err.append("QXmlStreamReader returned error: ");
    err.append(newsReader.errorString());
    err.append("\nFor URL: ");
    err.append(sourceUrl);
    QMaemo5InformationBox::information(0, err, 0);
    qDebug() << err;
    return;
  }

  // No longer need the current reply:
  reply->deleteLater();

  // Retrieve the image:
  reply = qnam->get(QNetworkRequest(QUrl(imageUrl)));

  connect(
    reply,
    SIGNAL(finished()),
    this,
    SLOT(loadImage()));
}


void LinRSSParser::parseRSSChannel(
  QXmlStreamReader &newsReader)
{
  imageAlreadySeen = false;
  bool firstItemAlreadySeen = false;

  while (!newsReader.atEnd())
  {
    newsReader.readNext();

    if (newsReader.isStartElement())
    {
      if (newsReader.name() == "title")
      {
//qDebug() << "found title";
        channelTitle = parseRSSText("title", newsReader);
      }
      else if (!imageAlreadySeen && newsReader.name() == "image")
      {
        if (QString::compare(
             newsReader.namespaceUri().toString(), ITUNES_NS, Qt::CaseInsensitive) == 0)
        {
          if (newsReader.attributes().hasAttribute("href"))
          {
            imageUrl = newsReader.attributes().value("href").toString();
          }
        }
        else
        {
          parseRSSImage(newsReader);
        }

        imageAlreadySeen = true;
      }
      else if (newsReader.name() == "item")
      {
        // we only want the first item:
        if (!firstItemAlreadySeen)
        {
          parseRSSItem(newsReader);
          firstItemAlreadySeen = true;
        }
        else
        {
          skipRSSItem(newsReader);
        }
      }
    }

    else if (newsReader.isEndElement())
    {
      if (newsReader.name() == "channel")
      {
        break;
      }
    }
  }
}


void LinRSSParser::parseRSSImage(
  QXmlStreamReader &newsReader)
{
  while (!newsReader.atEnd())
  {
    newsReader.readNext();

    if (newsReader.isStartElement())
    {
      if (newsReader.name() == "url")
      {
        imageUrl = parseRSSText("url", newsReader);
      }
    }
    else if (newsReader.isEndElement())
    {
      if (newsReader.name() == "image")
      {
        break;
      }
    }
  }
}


void LinRSSParser::parseRSSItem(
  QXmlStreamReader &newsReader)
{
  while (!newsReader.atEnd())
  {
    newsReader.readNext();

    if (newsReader.isStartElement())
    {
      if (newsReader.name() == "title")
      {
        nwi->setItemTitle(parseRSSText("title", newsReader));
      }
      else if (newsReader.name() == "guid")
      {
        itemGuid = parseRSSText("guid", newsReader);
      }
      else if (newsReader.name() == "pubDate")
      {
        itemPubDate = parseRSSText("pubDate", newsReader);
      }
      else if (!imageAlreadySeen && newsReader.name() == "image")
      {
        // If the channel didn't have an image, use the item image:
        if (newsReader.namespaceUri() == ITUNES_NS)
        {
          if (newsReader.attributes().hasAttribute("href"))
          {
            imageUrl = newsReader.attributes().value("href").toString();
          }
        }
        else
        {
          parseRSSImage(newsReader);
        }

        imageAlreadySeen = true;
      }
      else if (newsReader.name() == "enclosure")
      {
        if (newsReader.attributes().hasAttribute("url"))
        {
          nwi->setMediaUrl(
            newsReader.attributes().value("url").toString());
        }
      }
    }
    else if (newsReader.isEndElement())
    {
      if (newsReader.name() == "item")
      {
        break;
      }
    }
  }
}


void LinRSSParser::skipRSSItem(
  QXmlStreamReader &newsReader)
{
  while (!newsReader.atEnd())
  {
    newsReader.readNext();

    if (newsReader.isEndElement())
    {
      if (newsReader.name() == "item")
      {
        break;
      }
    }
  }
}


QString LinRSSParser::parseRSSText(
  QString elementName,
  QXmlStreamReader &newsReader)
{
  QString textString;

  while (!newsReader.atEnd())
  {
    newsReader.readNext();

    if (newsReader.isCharacters() && !newsReader.isWhitespace())
    {
      textString.append(newsReader.text().toString());
    }
    else if (newsReader.isEndElement())
    {
      if (newsReader.name() == elementName)
      {
        return textString;
      }
    }
  }

  return textString;
}


void LinRSSParser::loadImage()
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

  nwi->setImage(ba);

  // Clean up the reply:
  reply->deleteLater();
  reply = 0;
}
