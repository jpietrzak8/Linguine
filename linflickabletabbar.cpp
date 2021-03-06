//
// linflickabletabbar.cpp
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

#include "linflickabletabbar.h"
#include "lincollectionwidgetitem.h"
#include "linnewsfeedwidgetitem.h"

#include <QDebug>

LinFlickableTabBar::LinFlickableTabBar(
  QWidget *parent)
  : QListWidget(parent)
{
  setFlow(QListWidget::LeftToRight);

//  setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

  setFixedHeight(64);

  setStyleSheet(
    "QListView::item {"
      "background: qlineargradient("
        "spread: pad, x1:0, y1:0, x2:0, y2:1,"
        "stop: 0 rgba(48, 48, 48, 255),"
        "stop: 1 rgba(0, 0, 0, 255));"
      "padding: 0px 12px 0px 12px;"
    "}"

    "QListView::item:selected {"
      "background: qlineargradient("
        "spread: pad, x1:0, y1:0, x2:0, y2:1,"
        "stop: 0 rgba(96, 96, 96, 255),"
        "stop: 0.5 rgba(48, 48, 48, 255),"
        "stop: 1 rgba(0, 0, 0, 255));"
    "}");
}


bool LinFlickableTabBar::matchesCurrentCollection(
  LinNewsfeedWidgetItem *nwi)
{
  LinCollectionWidgetItem *cwi =
    dynamic_cast<LinCollectionWidgetItem *>(currentItem());

  FrequencyType itemFreq = nwi->getFrequency();
  FrequencyType catFreq = cwi->getFrequency();

  if ( (itemFreq != Any_Rate)
    && (catFreq != Any_Rate)
    && (itemFreq != catFreq))
  {
    return false;
  }

  MediaType itemMedia = nwi->getMedia();
  MediaType catMedia = cwi->getMedia();

  if ( (itemMedia != Any_Media)
    && (catMedia != Any_Media)
    && (itemMedia != catMedia))
  {
    return false;
  }

  LanguageType itemLang = nwi->getLanguage();
  LanguageType catLang = cwi->getLanguage();

  if ( (itemLang != Any_Language)
    && (catLang != Any_Language)
    && (itemLang != catLang))
  {
    return false;
  }

  QSet<QString>::const_iterator index = cwi->requiredTagsBegin();
  QSet<QString>::const_iterator end = cwi->requiredTagsEnd();

  while (index != end)
  {
    if (!nwi->hasTag(*index))
    {
      return false;
    }

    ++index;
  }

  return true;
}
