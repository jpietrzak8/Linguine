//
// lintoritems.cpp
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

#include "lintoritems.h"

#include "linimageloader.h"

#include <QDebug>

LinTORFolderItem::LinTORFolderItem(
  QString i,
  QString s,
  LinTORFolderType type)
  : id(i),
    sortid(s)
{
  // Construct the label:

  if (type == Custom_TORFolder)
  {
    // Just set the label to the id:
    label = id;
  }
  else if (id.startsWith("user/-/label/"))
  {
    label = id;
    label.remove(0, 13);
  }
  else if (id == "user/-/state/com.google/starred")
  {
    label = "Starred";
  }
  else if (id == "user/-/state/com.google/reading-list")
  {
    label = "All items";
  }
  else
  {
    qWarning() << "Unknown Tag: " << id;
    label = "Unknown Tag";
  }

  setText(0, label);
}


/////////////////////////////////////

LinTORSubscriptionItem::LinTORSubscriptionItem(
  QString i,
  QString t,
  QString s,
  QString f,
  QString u,
  QString h,
  QString i2,
  QNetworkAccessManager *q)
  : id(i),
    title(t),
    sortid(s),
    firstitemmsec(f),
    url(u),
    htmlUrl(h),
    iconUrl(i2),
    qnam(q),
    imageLoader(0)
{
  setText(0, title);

  // Need to do this here, due to TOR strangeness
  loadImage();
}


LinTORSubscriptionItem::LinTORSubscriptionItem(
  const LinTORSubscriptionItem &item)
  : QTreeWidgetItem(item),
    imageLoader(0)
{
  id = item.getID();
  title = item.getTitle();
  sortid = item.getSortid();
  firstitemmsec = item.getFirstitemmsec();
  url = item.getUrl();
  htmlUrl = item.getHtmlUrl();
  iconUrl = item.getIconUrl();
  qnam = item.getQnam();
}


LinTORSubscriptionItem::~LinTORSubscriptionItem()
{
  if (imageLoader) delete imageLoader;
}


/*
void LinTORSubscriptionItem::addCategory(
  QString id,
  QString label)
{
  categoryIDs.insert(id);

  // Do we need to store the label for anything?
}
*/


void LinTORSubscriptionItem::loadImage()
{
  if (!imageLoader)
  {
    imageLoader = new LinImageLoader(this, iconUrl, qnam);
  }
}


/*
bool LinTORSubscriptionItem::includedInFolder(
  QString folderID)
{
  // If it is the "All Subscriptions" folder, return true:
  if (folderID == "All Subscriptions")
  {
    return true;
  }

  // If this subscription has no categories, return false:
  if (categoryIDs.isEmpty())
  {
    return false;
  }

  QSet<QString>::const_iterator index = categoryIDs.constBegin();
  QSet<QString>::const_iterator end = categoryIDs.constEnd();

  while (index != end)
  {
    if (folderID == *index)
    {
      return true;
    }
    ++index;
  }

  return false;
}
*/
