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
#include "linimageloader.h"

#include <QXmlStreamReader>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QMaemo5InformationBox>

#include <QDebug>

#define ITUNES_NS "http://www.itunes.com/dtds/podcast-1.0.dtd"
#define RDF_NS "http://www.w3.org/1999/02/22-rdf-syntax-ns#"

LinRSSParser::LinRSSParser(
  LinNewsfeedWidgetItem *item,
  QString s,
  QNetworkAccessManager *manager)
  : nwi(item),
    sourceUrl(s),
    qnam(manager),
    reply(0),
    imageLoader(0),
    imageAlreadySeen(false)
{
}


LinRSSParser::~LinRSSParser()
{
  if (imageLoader) delete imageLoader;
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
  QXmlStreamReader newsReader(reply);

  while (!newsReader.atEnd())
  {
    newsReader.readNext();

    if (newsReader.isStartElement())
    {
      if (newsReader.name() == "channel")
      {
        parseRSSChannel(newsReader);
      }
    }
  }

  // No longer need the current reply:
  reply->deleteLater();

  if (newsReader.hasError())
  {
    QString err;
    err.append("QXmlStreamReader returned error: ");
    err.append(newsReader.errorString());
    err.append("\nFor URL: ");
    err.append(sourceUrl);
    QMaemo5InformationBox::information(0, err, 0);
    qWarning() << err;
    return;
  }

  nwi->setFaviconUrl(imageUrl);

  // At this point, assume item has been updated:
  emit itemUpdated();

  // Retrieve the image:
  if (imageLoader) delete imageLoader; // work on this
  imageLoader = new LinImageLoader(nwi, imageUrl, qnam);

  // Make sure that a signal is sent if the image is updated:
  connect(
    imageLoader,
    SIGNAL(imageUpdated()),
    this,
    SIGNAL(itemUpdated()));
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
            imageAlreadySeen = true;
          }
        }
        else if (QString::compare(
             newsReader.namespaceUri().toString(), RDF_NS, Qt::CaseInsensitive) == 0)
        {
          if (newsReader.attributes().hasAttribute("resource"))
          {
            imageUrl = newsReader.attributes().value("resource").toString();
            imageAlreadySeen = true;
          }
        }
        else
        {
          parseRSSImage(newsReader);
        }
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
        imageAlreadySeen = true;
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
//        itemPubDate = parseRSSText("pubDate", newsReader);
        nwi->setItemPubDate(parseRSSText("pubDate", newsReader));
      }
      else if (!imageAlreadySeen && newsReader.name() == "image")
      {
        // If the channel didn't have an image, use the item image:
        if (newsReader.namespaceUri() == ITUNES_NS)
        {
          if (newsReader.attributes().hasAttribute("href"))
          {
            imageUrl = newsReader.attributes().value("href").toString();
            imageAlreadySeen = true;
          }
        }
        else
        {
          parseRSSImage(newsReader);
        }
      }
      else if (newsReader.name() == "enclosure")
      {
        if (newsReader.attributes().hasAttribute("url"))
        {
          nwi->setMediaUrl(
            newsReader.attributes().value("url").toString());
        }
      }
      else if ( (newsReader.name() == "summary")
        && (newsReader.namespaceUri() == ITUNES_NS))
      {
//        itemSummary = parseRSSText("summary", newsReader);
        nwi->setItemSummary(parseRSSText("summary", newsReader));
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
        break;
      }
    }
  }

  return textString;
}
