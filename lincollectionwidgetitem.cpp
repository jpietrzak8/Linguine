//
// lincollectionwidgetitem.cpp
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

#include "lincollectionwidgetitem.h"

#include <QSettings>

LinCollectionWidgetItem::LinCollectionWidgetItem(
  QString title,
  FrequencyType f,
  MediaType m,
  LanguageType l,
  const QSet<QString> &rt)
  : QListWidgetItem(title),
    frequency(f),
    media(m),
    language(l),
    requiredTags(rt)
{
}


void LinCollectionWidgetItem::setFrequency(
  FrequencyType f)
{
  frequency = f;
}


void LinCollectionWidgetItem::setMedia(
  MediaType m)
{
  media = m;
}


void LinCollectionWidgetItem::setLanguage(
  LanguageType l)
{
  language = l;
}


/*
void LinCollectionWidgetItem::insertTag(
  QString tag)
{
  tags.insert(tag);
}


bool LinCollectionWidgetItem::hasTag(
  QString tag)
{
  return tags.contains(tag);
}
*/


void LinCollectionWidgetItem::addToSettings(
  QSettings &settings)
{
  settings.setValue("name", text());
  settings.setValue("frequency", frequency);
  settings.setValue("media", media);
  settings.setValue("language", language);

  QSet<QString>::const_iterator index = requiredTags.constBegin();
  QSet<QString>::const_iterator end = requiredTags.constEnd();
  int settingsIndex = 0;

  settings.beginWriteArray("requiredTags");
  while (index != end)
  {
    settings.setArrayIndex(settingsIndex);
    settings.setValue("requiredTag", *index);
    ++settingsIndex;
    ++index;
  }
  settings.endArray();
}
