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
#include "linfilteritems.h"

class QSettings;
class LinRSSParser;
class QNetworkAccessManager;


class LinNewsfeedWidgetItem: public QListWidgetItem
{
public:
  LinNewsfeedWidgetItem(
    QString name,
    QString sourceUrl,
    FrequencyType freq,
    ContentType category,
    MediaType media,
    LanguageType language,
    QString activeTextColor,
    QNetworkAccessManager *qnam);

  LinNewsfeedWidgetItem(
    QString name,
    QString sourceUrl,
    FrequencyType freq,
    ContentType category,
    MediaType media,
    LanguageType language,
    QString activeTextColor,
    QNetworkAccessManager *qnam,
    QSettings &settings);

  ~LinNewsfeedWidgetItem();

  void parseRSS();

  void setImage(
    const QByteArray &ba);

  void setItemTitle(
    QString itemTitle);

  void setMediaUrl(
    QString mediaUrl);

  QPixmap getImage();
  QString getItemTitle();
  QString getMediaUrl();

  void setFrequency(
    FrequencyType f)
    {frequency = f;}

  void setMedia(
    MediaType m)
    {media = m;}

  void setContent(
    ContentType c)
    {category = c;}

  void setLanguage(
    LanguageType l)
    {language = l;}

  FrequencyType getFrequency() {return frequency;}
  MediaType getMedia() {return media;}
  ContentType getContent() {return category;}
  LanguageType getLanguage() {return language;}
  QString getSourceUrl() {return sourceUrl;}

private:
  bool alreadyParsed;
  QString name;
  QString sourceUrl;
  FrequencyType frequency;
  ContentType category;
  MediaType media;
  LanguageType language;

  QPixmap image;
  QString itemTitle;
  QString mediaUrl;
  QString activeTextColor;
  QNetworkAccessManager *qnam;

  LinRSSParser *parser;
};

#endif // LINNEWSFEEDWIDGETITEM_H
