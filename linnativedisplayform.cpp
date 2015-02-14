//
// linnativedisplayform.cpp
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

#include "linnativedisplayform.h"

#include "mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QMaemo5InformationBox>
#include <QDesktopServices>

#include <QDebug>

#define MEDIA_NS "http://search.yahoo.com/mrss/"

LinNativeDisplayForm::LinNativeDisplayForm(
  MainWindow *mw,
  QNetworkAccessManager *q)
  : LinHtmlDisplayForm(mw),
    qnam(q),
    reply(0)
{
}


LinNativeDisplayForm::~LinNativeDisplayForm()
{
}


void LinNativeDisplayForm::displayText(
  QString feedName,
  QString sUrl,
  QString fUrl,
  LinFormatType format,
  bool hideImagesFlag,
  bool openExternalBrowser)
{
  setWindowTitle(feedName);

  sourceUrl = sUrl;
  faviconUrl = fUrl;
  reply = qnam->get(QNetworkRequest(QUrl(sourceUrl)));

  if (format == Atom_Format)
  {
    connect(
      reply,
      SIGNAL(finished()),
      this,
      SLOT(parseAtomFeed()));
  }
  else
  {
    // Default to RSS for now:
    connect(
      reply,
      SIGNAL(finished()),
      this,
      SLOT(parseRSSFeed()));
  }

  hideImages = hideImagesFlag;
  useExternalBrowser = openExternalBrowser;
}


void LinNativeDisplayForm::parseRSSFeed()
{
  QXmlStreamReader textReader(reply);

  while (!textReader.atEnd())
  {
    textReader.readNext();

    if (textReader.isStartElement())
    {
      if (textReader.name() == "channel")
      {
        parseRSSChannel(textReader);
        // For now, only parse one channel:
        break;
      }
    }
  }

  if (textReader.hasError())
  {
    QString err;
    err.append("QXmlStreamReader returned error: ");
    err.append(textReader.errorString());
    err.append("\nFor URL: ");
    err.append(sourceUrl);
    QMaemo5InformationBox::information(0, err, 0);
    qWarning() << err;
    return;
  }

  reply->deleteLater();
}


void LinNativeDisplayForm::parseRSSChannel(
  QXmlStreamReader &textReader)
{
  QString htmlOutput = htmlPrefix;

  while (!textReader.atEnd())
  {
    textReader.readNext();

    if (textReader.isStartElement())
    {
      if (textReader.name() == "item")
      {
        parseRSSItem(htmlOutput, textReader);
      }
    }

    else if (textReader.isEndElement())
    {
      if (textReader.name() == "channel")
      {
        htmlOutput +=" </body>\n</html>";
        setHtml(htmlOutput, sourceUrl);

        break;
      }
    }
  }
}


void LinNativeDisplayForm::parseRSSItem(
  QString &htmlOutput,
  QXmlStreamReader &textReader)
{
  QString title;
  QString link;
  QString imageUrl;
  QString enclosureUrl;
  QString enclosureType;
  QString mediaContentType;
  QString description;
  QString pubDate;
  bool mediaContentAlreadySeen = false;

  while (!textReader.atEnd())
  {
    textReader.readNext();

    if (textReader.isStartElement())
    {
      if (textReader.name() == "title")
      {
        title = parseText("title", textReader);
      }
      else if (textReader.name() == "link")
      {
        link = parseText("link", textReader);
      }
      else if (textReader.name() == "enclosure")
      {
        if (textReader.attributes().hasAttribute("type"))
        {
          enclosureType = textReader.attributes().value("type").toString();
        }

        if (textReader.attributes().hasAttribute("url"))
        {
          if (enclosureType == "image/jpeg")
          {
            // Use the enclosure as our image:
            imageUrl = textReader.attributes().value("url").toString();
          }
          else
          {
            enclosureUrl = textReader.attributes().value("url").toString();
          }
        }
      }
      else if (textReader.name() == "thumbnail")
      {
        // For some reason, BBC throws two thumbnails into each item.
        // The second one is always bigger, so we'll just let it overwrite
        // the first one.
        // Also, the Australian BC puts both "content" and "thumbnail"
        // images into its rss; if content has already been seen, I'll just
        // ignore the thumbnail:
        if ( !mediaContentAlreadySeen
          && QString::compare(
            textReader.namespaceUri().toString(),
            MEDIA_NS,
            Qt::CaseInsensitive) == 0
          && textReader.attributes().hasAttribute("url"))
        {
          imageUrl = textReader.attributes().value("url").toString();
        }
      }
      else if (textReader.name() == "content")
      {
        if (textReader.attributes().hasAttribute("medium"))
        {
          mediaContentType = textReader.attributes().value("medium").toString();
        }

        // If there are multiple content items, choose the first one:
        if ( !mediaContentAlreadySeen
          && QString::compare(
            textReader.namespaceUri().toString(),
            MEDIA_NS,
            Qt::CaseInsensitive) == 0
          && textReader.attributes().hasAttribute("url"))
        {
          if (mediaContentType == "image")
          {
            imageUrl = textReader.attributes().value("url").toString();
          }
          else
          {
            enclosureUrl = textReader.attributes().value("url").toString();
          }
          mediaContentAlreadySeen = true;
        }
      }
      else if (textReader.name() == "description")
      {
        description = parseText("description", textReader);
      }
      else if (textReader.name() == "pubDate")
      {
        pubDate = parseText("pubDate", textReader);
      }
    }

    else if (textReader.isEndElement())
    {
      if (textReader.name() == "item")
      {
        // marshal the entire item into the HTML display:
        if (!title.isEmpty())
        {
          htmlOutput += "<b>";

          if (!link.isEmpty())
          {
            htmlOutput += "<a href=\"";
            htmlOutput += link;
            htmlOutput += "\">";
            htmlOutput += title;
            htmlOutput += "</a>";
          }
          else
          {
            htmlOutput += title;
          }
          htmlOutput += "</b>";
        }

        htmlOutput += "<p>";

        if (!imageUrl.isEmpty() && !hideImages)
        {
          htmlOutput += "<img class=\"main\" src=\"";
          htmlOutput += imageUrl;
          htmlOutput += "\"/>\n";
        }

        if (!enclosureUrl.isEmpty())
        {
          htmlOutput += "<a href=\"";
          htmlOutput += enclosureUrl;
          htmlOutput += "\"><img src=\"qrc:/icons/blue-play.svg\" width=\"64px\" height=\"64px\"/></a>\n";
        }

        if (!description.isEmpty())
        {
          htmlOutput += description;
        }

        htmlOutput += "</p>\n";

        if (!faviconUrl.isEmpty())
        {
          htmlOutput += "<img class=\"footer\" src=\"";
          htmlOutput += faviconUrl;
          htmlOutput += "\"/>";
          if (pubDate.isEmpty())
          {
            htmlOutput += "<br>\n";
          }
          else
          {
            htmlOutput += " &nbsp; ";
          }
        }

        if (!pubDate.isEmpty())
        {
          htmlOutput += "<small>";
          htmlOutput += pubDate;
          htmlOutput += "</small><br>\n";
        }

        htmlOutput += "<hr width=\"60%\"/>";

        break;
      }
    }
  }
}


QString LinNativeDisplayForm::parseText(
  QString elementName,
  QXmlStreamReader &textReader)
{
  QString textString;

  while (!textReader.atEnd())
  {
    textReader.readNext();

    if (textReader.isEndElement())
    {
      if (textReader.name() == elementName)
      {
        return textString;
      }
      else
      {
        textString.append(textReader.text().toString());
      }
    }
    else
    {
      textString.append(textReader.text().toString());
    }
  }

  return textString;
}


void LinNativeDisplayForm::parseAtomFeed()
{
  QXmlStreamReader textReader(reply);
  QString htmlOutput = htmlPrefix;

  while (!textReader.atEnd())
  {
    textReader.readNext();

    if (textReader.isStartElement())
    {
      if (textReader.name() == "entry")
      {
        parseAtomEntry(htmlOutput, textReader);
      }
    }
  }

  if (textReader.hasError())
  {
    QString err;
    err.append("QXmlStreamReader returned error: ");
    err.append(textReader.errorString());
    err.append("\nFor URL: ");
    err.append(sourceUrl);
    QMaemo5InformationBox::information(0, err, 0);
    qWarning() << err;
    return;
  }

  reply->deleteLater();

  htmlOutput += " </body>\n</html>";
  setHtml(htmlOutput, sourceUrl);
}


void LinNativeDisplayForm::parseAtomEntry(
  QString &htmlOutput,
  QXmlStreamReader &textReader)
{
  QString title;
  QString link;
  QString imageUrl;
  QString enclosureUrl;
  QString enclosureType;
  QString mediaContentType;
  QString description;
  QString pubDate;
  bool mediaContentAlreadySeen = false;

  while (!textReader.atEnd())
  {
    textReader.readNext();

    if (textReader.isStartElement())
    {
      if (textReader.name() == "title")
      {
        title = parseText("title", textReader);
      }
      else if (textReader.name() == "link")
      {
        if (textReader.attributes().hasAttribute("href"))
        {
          link = textReader.attributes().value("href").toString();
        }
      }
      else if (textReader.name() == "summary")
      {
        description = parseText("summary", textReader);
      }
      else if (textReader.name() == "published")
      {
        pubDate = parseText("published", textReader);
      }
    }

    else if (textReader.isEndElement())
    {
      if (textReader.name() == "entry")
      {
        // marshal the entire item into the HTML display:
        if (!title.isEmpty())
        {
          htmlOutput += "<b>";

          if (!link.isEmpty())
          {
            htmlOutput += "<a href=\"";
            htmlOutput += link;
            htmlOutput += "\">";
            htmlOutput += title;
            htmlOutput += "</a>";
          }
          else
          {
            htmlOutput += title;
          }
          htmlOutput += "</b>";
        }

        htmlOutput += "<p>";

        if (!imageUrl.isEmpty() && !hideImages)
        {
          htmlOutput += "<img class=\"main\" src=\"";
          htmlOutput += imageUrl;
          htmlOutput += "\"/>\n";
        }

        if (!enclosureUrl.isEmpty())
        {
          htmlOutput += "<a href=\"";
          htmlOutput += enclosureUrl;
          htmlOutput += "\"><img src=\"qrc:/icons/blue-play.svg\" width=\"64px\" height=\"64px\"/></a>\n";
        }

        if (!description.isEmpty())
        {
          htmlOutput += description;
        }

        htmlOutput += "</p>\n";

        if (!faviconUrl.isEmpty())
        {
          htmlOutput += "<img class=\"footer\" src=\"";
          htmlOutput += faviconUrl;
          htmlOutput += "\"/>";
          if (pubDate.isEmpty())
          {
            htmlOutput += "<br>\n";
          }
          else
          {
            htmlOutput += " &nbsp; ";
          }
        }

        if (!pubDate.isEmpty())
        {
          htmlOutput += "<small>";
          htmlOutput += pubDate;
          htmlOutput += "</small><br>\n";
        }

        htmlOutput += "<hr width=\"60%\"/>";

        break;
      }
    }
  }
}
