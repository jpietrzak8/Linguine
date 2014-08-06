//
// linhtmldisplayform.cpp
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

#include "linhtmldisplayform.h"
#include "ui_linhtmldisplayform.h"

#include "mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QMaemo5InformationBox>
#include <QDesktopServices>

#include <QDebug>

#include "qwebviewselectionsuppressor.h"

#define MEDIA_NS "http://search.yahoo.com/mrss/"

LinHtmlDisplayForm::LinHtmlDisplayForm(
  MainWindow *mw,
  QNetworkAccessManager *q)
  : QWidget(mw),
    ui(new Ui::LinHtmlDisplayForm),
    mainWindow(mw),
    qnam(q),
    reply(0),
    suppressor(0),
    useExternalBrowser(true)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_Maemo5StackedWindow);
  setWindowFlags(windowFlags() | Qt::Window);

  suppressor = new QWebViewSelectionSuppressor(ui->htmlDisplay);

  blankHtmlPage = "<html><head><style type=\"text/css\">body{background-color: ";
  blankHtmlPage += mainWindow->getBackgroundColor();
  blankHtmlPage += ";}</style></head><body></body></html>";

  // Initialize the display:
  ui->htmlDisplay->setHtml(blankHtmlPage);

  // Manage links:
  ui->htmlDisplay->page()->setLinkDelegationPolicy(
    QWebPage::DelegateExternalLinks);

  connect(
    ui->htmlDisplay->page(),
    SIGNAL(linkClicked(const QUrl &)),
    this,
    SLOT(onLinkClicked(const QUrl &)));
}


LinHtmlDisplayForm::~LinHtmlDisplayForm()
{
  delete ui;
}


void LinHtmlDisplayForm::displayText(
  QString feedName,
  QString sUrl,
  QString fUrl,
  bool openExternalBrowser)
{
  setWindowTitle(feedName);

  sourceUrl = sUrl;
  faviconUrl = fUrl;
  reply = qnam->get(QNetworkRequest(QUrl(sourceUrl)));

  connect(
    reply,
    SIGNAL(finished()),
    this,
    SLOT(parseRSSFeed()));

  useExternalBrowser = openExternalBrowser;
}


void LinHtmlDisplayForm::closeEvent(
  QCloseEvent *event)
{
  QWidget::closeEvent(event);

  // The HTML browser can be very expensive, so don't let it continue running
  // in the background:
  ui->htmlDisplay->stop();

  // Reset the page:
  ui->htmlDisplay->setHtml(blankHtmlPage);
}


void LinHtmlDisplayForm::onLinkClicked(
  const QUrl &url)
{
  if (useExternalBrowser)
  {
    QDesktopServices::openUrl(url);
  }
  else
  {
    ui->htmlDisplay->load(url);
  }
}


void LinHtmlDisplayForm::parseRSSFeed()
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
    qDebug() << err;
    return;
  }

  reply->deleteLater();
}


void LinHtmlDisplayForm::parseRSSChannel(
  QXmlStreamReader &textReader)
{
  QString htmlOutput;

  // Create the head portion of the HTML:
  htmlOutput += "<html>\n <head>\n  <title>Linguine Output</title>\n";
  // Place some CSS in the head of the html:
  htmlOutput += "  <style type=\"text/css\">\n";
  htmlOutput += "   hr {color: " + mainWindow->getAccentColor() + ";}\n";
  htmlOutput += "   img.main {max-width: 100%; height: auto;}\n";
  htmlOutput += "   img.footer {height: 20px; width: auto;}\n";
//  htmlOutput += "   body {-webkit-user-select: none;}\n";
  htmlOutput += "   body {\n";
  htmlOutput += "    color: " + mainWindow->getDefaultTextColor() + ";\n";
  htmlOutput += "    background-color: " + mainWindow->getBackgroundColor() + ";\n";
  htmlOutput += "    font-family: \"" + mainWindow->getSystemFontFamily() + "\", sans-serif;\n";
  htmlOutput += "    font-size: x-large;\n";
  htmlOutput += "   }\n";
  htmlOutput += "   a:link {\n";
  htmlOutput += "    color: " + mainWindow->getActiveTextColor() + ";\n";
  htmlOutput += "   }\n";
  htmlOutput += "   a:visited {\n";
  htmlOutput += "    color: " + mainWindow->getSecondaryTextColor() + ";\n";
  htmlOutput += "   }\n";
  htmlOutput += "  </style>\n";
  // Finish the head portion, start the body:
  htmlOutput += " </head>\n <body>\n";

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
        ui->htmlDisplay->setHtml(htmlOutput, sourceUrl);

        break;
      }
    }
  }
}


void LinHtmlDisplayForm::parseRSSItem(
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
        title = parseRSSText("title", textReader);
      }
      else if (textReader.name() == "link")
      {
        link = parseRSSText("link", textReader);
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
        description = parseRSSText("description", textReader);
      }
      else if (textReader.name() == "pubDate")
      {
        pubDate = parseRSSText("pubDate", textReader);
      }
    }

    else if (textReader.isEndElement())
    {
      if (textReader.name() == "item")
      {
        // marshal the entire item into the HTML display:
        if (!title.isEmpty())
        {
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
        htmlOutput += "<br>\n";

        if (!imageUrl.isEmpty())
        {
          htmlOutput += "<img class=\"main\" src=\"";
          htmlOutput += imageUrl;
          htmlOutput += "\"/><br>\n";
        }

        if (!enclosureUrl.isEmpty())
        {
          htmlOutput += "<a href=\"";
          htmlOutput += enclosureUrl;
          htmlOutput += "\"><img src=\"qrc:/icons/podcast_icon&48.png\"></a>\n";
        }

        if (!description.isEmpty())
        {
          htmlOutput += "<p>";
          htmlOutput += description;
          htmlOutput += "</p>\n";
        }

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


QString LinHtmlDisplayForm::parseRSSText(
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

