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
  MediaType m,
  LanguageType l,
  const QSet<QString> &t,
  QString a,
  QNetworkAccessManager *q)
  : alreadyParsed(false),
    name(n),
    sourceUrl(s),
    frequency(f),
    media(m),
    language(l),
    tags(t),
    activeTextColor(a),
    qnam(q),
    parser(0)
{
//  parser = new LinRSSParser(this, sourceUrl, qnam);
  setText(name);
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


void LinNewsfeedWidgetItem::setItemSummary(
  QString is)
{
  itemSummary = is;
}


void LinNewsfeedWidgetItem::setItemPubDate(
  QString ipd)
{
  itemPubDate = ipd;
}


void LinNewsfeedWidgetItem::setFaviconUrl(
  QString fUrl)
{
  faviconUrl = fUrl;
}


/*
void LinNewsfeedWidgetItem::setImage(
  const QByteArray &ba)
{
  image.loadFromData(ba);

  setIcon(QIcon(image));
}
*/


void LinNewsfeedWidgetItem::setItemTitle(
  QString s)
{
  itemTitle = s;
}


void LinNewsfeedWidgetItem::setMediaUrl(
  QString s)
{
  mediaUrl = s;
}


void LinNewsfeedWidgetItem::resetTitle()
{
  // Update the item's on-screen info:
  if (!itemTitle.isEmpty())
  {
/*
    if (!itemPubDate.isEmpty())
    {
      QString tempString = itemTitle;
      tempString += "\n";
      tempString += itemPubDate;
      setText(tempString);
    }
    else
    {
      setText(itemTitle);
    }
*/
    setText(itemTitle);
  }
}


/*
bool LinNewsfeedWidgetItem::operator<(
  const QListWidgetItem &other) const
{
  const LinNewsfeedWidgetItem *otherNWI =
    dynamic_cast<const LinNewsfeedWidgetItem *>(&other);

  if (!otherNWI)
  {
    // fall back to the parent's implementation:
    return QListWidgetItem::operator<(other);
  }

  return (itemPubDate < otherNWI->getItemPubDate());
}
*/


void LinNewsfeedWidgetItem::addToSettings(
  QSettings &settings)
{
  settings.setValue("name", name);
  settings.setValue("url", sourceUrl);
  settings.setValue("frequency", frequency);
  settings.setValue("media", media);
  settings.setValue("language", language);

  settings.beginWriteArray("tags");

  QSet<QString>::const_iterator index = tags.constBegin();
  QSet<QString>::const_iterator end = tags.constEnd();
  int settingsIndex = 0;

  while (index != end)
  {
    settings.setArrayIndex(settingsIndex);
    settings.setValue("tag", *index);
    ++settingsIndex;
    ++index;
  }

  settings.endArray();
}
