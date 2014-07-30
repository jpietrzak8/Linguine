//
// lincategorywidgetitem.h
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

#ifndef LINCATEGORYWIDGETITEM_H
#define LINCATEGORYWIDGETITEM_H

#include <QListWidgetItem>
#include "linfilteritems.h"

class LinCategoryWidgetItem: public QListWidgetItem
{
public:
  LinCategoryWidgetItem(
    QString title);

  void setFrequency(
    FrequencyType frequency);

  void setMedia(
    MediaType media);

  void setContent(
    ContentType content);

  void setLanguage(
    LanguageType language);

  void setTag(
    QString tag);

  FrequencyType getFrequency() { return frequency; }
  MediaType getMedia() { return media; }
  ContentType getContent() { return content; }
  LanguageType getLanguage() { return language; }
  QString getTag() { return tag; }

private:
  FrequencyType frequency;
  MediaType media;
  ContentType content;
  LanguageType language;
  QString tag;
};

#endif // LINCATEGORYWIDGETITEM_H
