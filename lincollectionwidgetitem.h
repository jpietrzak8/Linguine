//
// lincollectionwidgetitem.h
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

#ifndef LINCOLLECTIONWIDGETITEM_H
#define LINCOLLECTIONWIDGETITEM_H

#include <QListWidgetItem>
#include "linfilteritems.h"
#include <QSet>
#include <QString>

class QSettings;

class LinCollectionWidgetItem: public QListWidgetItem
{
public:
  LinCollectionWidgetItem(
    QString title,
    FrequencyType frequency,
    MediaType media,
    LanguageType language,
    const QSet<QString> &requiredTags);

  void setFrequency(
    FrequencyType frequency);

  void setMedia(
    MediaType media);

  void setLanguage(
    LanguageType language);

/*
  void insertTag(
    QString tag);
*/

  FrequencyType getFrequency() { return frequency; }
  MediaType getMedia() { return media; }
  LanguageType getLanguage() { return language; }

  QSet<QString>::const_iterator requiredTagsBegin()
    { return requiredTags.constBegin(); }

  QSet<QString>::const_iterator requiredTagsEnd()
    { return requiredTags.constEnd(); }

  void addToSettings(
    QSettings &settings);

private:
  FrequencyType frequency;
  MediaType media;
  LanguageType language;
  QSet<QString> requiredTags;
};

#endif // LINCOLLECTIONWIDGETITEM_H
