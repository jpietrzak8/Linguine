//
// linnewsfeedwidgetitem.cpp
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

#include "linnewsfeedwidgetitem.h"

#include <QSettings>
#include "linrssparser.h"

#include <QDebug>


LinNewsfeedWidgetItem::LinNewsfeedWidgetItem(
  QString n,
  QString s,
  FrequencyType f,
  ContentType c,
  MediaType m,
  LanguageType l,
  QNetworkAccessManager *q)
  : alreadyParsed(false),
    name(n),
    sourceUrl(s),
    frequency(f),
    category(c),
    media(m),
    language(l),
    qnam(q),
    parser(0)
{
//  parser = new LinRSSParser(this, sourceUrl, qnam);
}


LinNewsfeedWidgetItem::LinNewsfeedWidgetItem(
  QString n,
  QString s,
  FrequencyType f,
  ContentType c,
  MediaType m,
  LanguageType l,
  QNetworkAccessManager *q,
  QSettings &settings)
  : alreadyParsed(false),
    name(n),
    sourceUrl(s),
    frequency(f),
    category(c),
    media(m),
    language(l),
    qnam(q),
    parser(0)
{
//  parser = new LinRSSParser(this, sourceUrl, qnam);

  settings.setValue("name", name);
  settings.setValue("url", sourceUrl);
  settings.setValue("frequency", frequency);
  settings.setValue("category", category);
  settings.setValue("media", media);
  settings.setValue("language", language);
}


LinNewsfeedWidgetItem::~LinNewsfeedWidgetItem()
{
  if (parser) delete parser;
}


void LinNewsfeedWidgetItem::parseRSS()
{
  if (!alreadyParsed)
  {
    parser = new LinRSSParser(this, sourceUrl, qnam);
    parser->startParsing();
    alreadyParsed = true;
  }
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
