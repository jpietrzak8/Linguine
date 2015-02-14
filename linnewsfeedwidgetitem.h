//
// linnewsfeedwidgetitem.h
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

#ifndef LINNEWSFEEDWIDGETITEM_H
#define LINNEWSFEEDWIDGETITEM_H

#include <QListWidgetItem>
#include <QPixmap>
#include <QString>
#include <QSet>
#include "linfilteritems.h"

class QSettings;
class LinRSSParser;
class LinAtomParser;
class QNetworkAccessManager;

typedef QSet<QString> TagCollection;


class LinNewsfeedWidgetItem: public QListWidgetItem
{
public:
  LinNewsfeedWidgetItem(
    QString name,
    QString sourceUrl,
    FrequencyType freq,
    MediaType media,
    LanguageType language,
    LinFormatType format,
    const TagCollection &tags,
    QString activeTextColor,
    QNetworkAccessManager *qnam);

  ~LinNewsfeedWidgetItem();

  void parseFeed();

  void setItemSummary(
    QString itemSummary);

  void setItemPubDate(
    QString itemPubDate);

  void setFaviconUrl(
    QString faviconUrl);

  void setImage(
    const QByteArray &ba);

  void setItemTitle(
    QString itemTitle);

  void setMediaUrl(
    QString mediaUrl);

  QString getItemSummary() const { return itemSummary; }
  QString getItemPubDate() const { return itemPubDate; }
  QString getFaviconUrl() const { return faviconUrl; }
  QPixmap getImage() const { return image; }
  QString getItemTitle() const { return itemTitle; }
  QString getMediaUrl() const { return mediaUrl; }

  void setFrequency(
    FrequencyType f)
    {frequency = f;}

  void setMedia(
    MediaType m)
    {media = m;}

  void setLanguage(
    LanguageType l)
    {language = l;}

  void setAtomFormat()
    {feedFormat = Atom_Format;}

  void insertTag(
    QString tag)
    {tags.insert(tag);}

  FrequencyType getFrequency() const {return frequency;}
  MediaType getMedia() const {return media;}
  LanguageType getLanguage() const {return language;}
  LinFormatType getFormat() const {return feedFormat;}
  QString getSourceUrl() const {return sourceUrl;}
  QString getName() const { return name; }
  QNetworkAccessManager *getQnam() { return qnam; }

/*
  QSet<QString>::const_iterator tagsBegin()
    {return tags.constBegin();}

  QSet<QString>::const_iterator tagsEnd()
    {return tags.constEnd();}
*/

  bool hasTag(
    QString tag)
    { return tags.contains(tag); }

  // For sorting purposes:
  virtual bool operator<(const QListWidgetItem &other) const;

  void addToSettings(
    QSettings &settings);

  // Class Static:
  static LinNewsfeedSortType sortType;

private:
  bool alreadyParsed;
  QString name;
  QString sourceUrl;
  FrequencyType frequency;
  MediaType media;
  LanguageType language;
  LinFormatType feedFormat;
  TagCollection tags;

  QString itemSummary;
  QString itemPubDate;
  QString faviconUrl;
  QPixmap image;
  QString itemTitle;
  QString mediaUrl;
  QString activeTextColor;

  QNetworkAccessManager *qnam;

  LinRSSParser *rssParser;
  LinAtomParser *atomParser;
};

#endif // LINNEWSFEEDWIDGETITEM_H
