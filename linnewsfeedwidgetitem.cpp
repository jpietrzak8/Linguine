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
#include "linatomparser.h"

#include <QDebug>


// Initialize the static value:
LinNewsfeedSortType LinNewsfeedWidgetItem::sortType = Name_Sort;

LinNewsfeedWidgetItem::LinNewsfeedWidgetItem(
  QString n,
  QString s,
  FrequencyType f,
  MediaType m,
  LanguageType l,
  LinFormatType format,
  const TagCollection &t,
  QString a,
  QNetworkAccessManager *q)
  : alreadyParsed(false),
    name(n),
    sourceUrl(s),
    frequency(f),
    media(m),
    language(l),
    feedFormat(format),
    tags(t),
    activeTextColor(a),
    qnam(q),
    rssParser(0),
    atomParser(0)
{
  setText(name);

  // Set data for use by delegates:
  setData(Qt::UserRole, name);
}


LinNewsfeedWidgetItem::~LinNewsfeedWidgetItem()
{
  if (atomParser) delete atomParser;
  if (rssParser) delete rssParser;
}


void LinNewsfeedWidgetItem::parseFeed()
{
  if (!alreadyParsed)
  {
    if (feedFormat == RSS_Format)
    {
      rssParser = new LinRSSParser(this, sourceUrl, qnam);
      rssParser->startParsing();
    }
    else
    {
      atomParser = new LinAtomParser(this, sourceUrl, qnam);
      atomParser->startParsing();
    }
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

  setData(Qt::UserRole + 1, itemPubDate);
}


void LinNewsfeedWidgetItem::setFaviconUrl(
  QString fUrl)
{
  faviconUrl = fUrl;
}


void LinNewsfeedWidgetItem::setImage(
  const QByteArray &ba)
{
  image.loadFromData(ba);

  if (!image.isNull())
  {
    setData(
      Qt::UserRole + 2,
      image.scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  }
  else
  {
    setData(
      Qt::UserRole + 2,
      image);
  }

  setIcon(QIcon(image));
}


void LinNewsfeedWidgetItem::setItemTitle(
  QString s)
{
  itemTitle = s;

  if (!itemTitle.isEmpty())
  {
    setText(itemTitle);
  }
}


void LinNewsfeedWidgetItem::setMediaUrl(
  QString s)
{
  mediaUrl = s;
}


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

  if (sortType == Timestamp_Sort)
  {
    // Need a better representation of time here!
    return (itemPubDate < otherNWI->getItemPubDate());
  }
  else
  {
    // Default sorting method:
    return (name < otherNWI->getName());
  }
}


void LinNewsfeedWidgetItem::addToSettings(
  QSettings &settings)
{
  settings.setValue("name", name);
  settings.setValue("url", sourceUrl);
  settings.setValue("frequency", frequency);
  settings.setValue("media", media);
  settings.setValue("language", language);

  settings.beginWriteArray("tags");

  TagCollection::const_iterator index = tags.constBegin();
  TagCollection::const_iterator end = tags.constEnd();
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
