//
// lintordisplayform.cpp
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

#include "lintordisplayform.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMaemo5InformationBox>
#include <QDesktopServices>
#include <QWebFrame>
#include "mainwindow.h"

#include <QDebug>

LinTORDisplayForm::LinTORDisplayForm(
  MainWindow *mw,
  QNetworkAccessManager *q)
  : LinHtmlDisplayForm(mw),
    mainWindow(mw),
    qnam(q),
    reply(0),
    nowPlayingForm(0),
    videoDisplayForm(0)
{
}


LinTORDisplayForm::~LinTORDisplayForm()
{
  if (nowPlayingForm) delete nowPlayingForm;
  if (videoDisplayForm) delete videoDisplayForm;
}


void LinTORDisplayForm::setupTORDisplay(
  QString windowTitle,
  bool openExternalPlayer,
  bool openExternalBrowser)
{
  setWindowTitle(windowTitle);

  useExternalPlayer = openExternalPlayer;
  useExternalBrowser = openExternalBrowser;
}


void LinTORDisplayForm::displayItems(
  const LinTORItemCollection &items)
{
  QString htmlOutput = htmlPrefix;

  if (items.isEmpty())
  {
    // Construct a blank page and get out:
    htmlOutput +=" </body>\n</html>";
    setHtml(htmlOutput);
    return;
  }

  LinTORItemCollection::const_iterator index = items.constBegin();
  LinTORItemCollection::const_iterator end = items.constEnd();

  while (index != end)
  {
    QString link = index.value().getSourceUrl();
    QString title = index.value().getTitle();

    // Construct the html display:
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

    // Next, the enclosure & summary:

    htmlOutput += "<p>";

    QString enclosureUrl = index.value().getEnclosureUrl();
    if (!enclosureUrl.isEmpty())
    {
      if (index.value().getEnclosureType() == "image/jpeg")
      {
        htmlOutput += "<img class=\"main\" src=\"";
        htmlOutput += enclosureUrl;
        htmlOutput += "\"/>\n";
      }
      else
      {
        htmlOutput += "<button onclick=\"linguineManager.displayTORmedia('";
        htmlOutput += index.key();
        htmlOutput += "')\"><img src=\"qrc:/icons/blue-play.svg\" width=\"64px\" height=\"64px\"/></button>\n";
      }
    }

    htmlOutput += index.value().getSummary();

    htmlOutput += "</p>";

    htmlOutput += "<hr width=\"60%\"/>";

    ++index;
  }

  htmlOutput +=" </body>\n</html>";
  setHtml(htmlOutput);

  getFrame()->addToJavaScriptWindowObject("linguineManager", this);
}


void LinTORDisplayForm::displayTORmedia(
  QString id)
{
  if (!mainWindow->hasTORItem(id))
  {
    // report an error here
    return;
  }

  LinTORItem item = mainWindow->getTORItem(id);

  if (useExternalPlayer)
  {
    mainWindow->launchDBusPlayer(item.getEnclosureUrl());
    return;
  }

  QString type = item.getEnclosureType();

  if (type == "audio/mpeg")
  {
    if (!nowPlayingForm)
    {
      nowPlayingForm = new LinNowPlayingForm(this);
    }

    nowPlayingForm->setupTOR(
      item.getFeedName(),
      item.getTitle(),
      item.getSummary(),
      item.getEnclosureUrl());

    nowPlayingForm->show();
  }
  else if (type == "video/mpeg")
  {
    if (!videoDisplayForm)
    {
      videoDisplayForm = new LinVideoDisplayForm(this);
    }

    videoDisplayForm->setProgram(
      item.getFeedName(),
      item.getSummary(),
      QString(),
      item.getEnclosureUrl());

    // Set auto orientation to false:
    setAttribute(static_cast<Qt::WidgetAttribute>(130), false);

    // Set landscape orientation to true:
    setAttribute(static_cast<Qt::WidgetAttribute>(129), true);

    videoDisplayForm->show();
  }
  else
  {
    QString err = "Unknown enclosure type: ";
    err += type;
    QMaemo5InformationBox::information(this, err);
    qWarning() << err;
  }
}
