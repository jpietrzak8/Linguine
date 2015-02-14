//
// linatomparser.cpp
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

#include "linatomparser.h"

#include "linimageloader.h"

#include <QXmlStreamReader>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QMaemo5InformationBox>

#include <QDebug>


LinAtomParser::LinAtomParser(
  LinNewsfeedWidgetItem *item,
  QString s,
  QNetworkAccessManager *manager)
  : imageLoader(0),
    sourceUrl(s),
    qnam(manager),
    reply(0),
    nwi(item)
{
}


LinAtomParser::~LinAtomParser()
{
  if (imageLoader) delete imageLoader;
}


void LinAtomParser::startParsing()
{
  reply = qnam->get(QNetworkRequest(QUrl(sourceUrl)));

  connect(
    reply,
    SIGNAL(finished()),
    this,
    SLOT(parseAtomFeed()));
}


void LinAtomParser::parseAtomFeed()
{
  QXmlStreamReader newsReader(reply);
  bool firstEntryAlreadySeen = false;

  while (!newsReader.atEnd())
  {
    newsReader.readNext();

    if (newsReader.isStartElement())
    {
      if (newsReader.name() == "title")
      {
        title = parseAtomText("title", newsReader);
      }
      else if (newsReader.name() == "logo")
      {
        imageUrl = parseAtomText("logo", newsReader);
      }
      else if (newsReader.name() == "entry")
      {
        if (!firstEntryAlreadySeen)
        {
          parseAtomEntry(newsReader);
          firstEntryAlreadySeen = true;
        }
        else
        {
          skipAtomEntry(newsReader);
        }
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

  emit itemUpdated();

  // Retrieve the image:
  if (imageLoader) delete imageLoader; // work on this
  imageLoader = new LinImageLoader(nwi, imageUrl, qnam);

  connect(
    imageLoader,
    SIGNAL(imageUpdated()),
    this,
    SIGNAL(itemUpdated()));
}


void LinAtomParser::parseAtomEntry(
  QXmlStreamReader &newsReader)
{
  while (!newsReader.atEnd())
  {
    newsReader.readNext();

    if (newsReader.isStartElement())
    {
      if (newsReader.name() == "title")
      {
        nwi->setItemTitle(parseAtomText("title", newsReader));
      }
      else if (newsReader.name() == "published")
      {
        nwi->setItemPubDate(parseAtomText("published", newsReader));
      }
      else if (newsReader.name() == "summary")
      {
        nwi->setItemSummary(parseAtomText("summary", newsReader));
      }
      else if (newsReader.name() == "link")
      {
        if (newsReader.attributes().hasAttribute("href"))
        {
          nwi->setMediaUrl(
            newsReader.attributes().value("href").toString());
        }
      }
    }
    else if (newsReader.isEndElement())
    {
      if (newsReader.name() == "entry")
      {
        break;
      }
    }
  }
}


void LinAtomParser::skipAtomEntry(
  QXmlStreamReader &newsReader)
{
  while (!newsReader.atEnd())
  {
    newsReader.readNext();

    if (newsReader.isEndElement())
    {
      if (newsReader.name() == "entry")
      {
        break;
      }
    }
  }
}


QString LinAtomParser::parseAtomText(
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
