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
    sortid(s),
    unreadCount(0)
{
  // Construct the title:

  if (type == Custom_TORFolder)
  {
    // Just set the title to the id:
    title = id;
  }
  else if (id.startsWith("user/-/label/"))
  {
    title = id;
    title.remove(0, 13);
  }
  else if (id == "user/-/state/com.google/starred")
  {
    title = "Starred";
  }
  else if (id == "user/-/state/com.google/reading-list")
  {
    title = "All items";
  }
  else
  {
    qWarning() << "Unknown Tag: " << id;
    title = "Unknown Tag";
  }

  setText(0, title);
}


LinTORFolderItem::LinTORFolderItem(
  QString i,
  QString s,
  QString t)
  : id(i),
    sortid(s),
    title(t)
{
  setText(0, title);
}


LinTORFolderItem::LinTORFolderItem(
  const LinTORFolderItem &other)
  : QTreeWidgetItem(other)
{
  id = other.getID();
  sortid = other.getSortid();
  title = other.getTitle();

  setText(0, title);
}


void LinTORFolderItem::setUnreadCount(
  int uc)
{
  unreadCount = uc;

  setData(0, Qt::UserRole + 3, unreadCount);
}


/////////////////////////////////////

LinTORSubscriptionItem::LinTORSubscriptionItem(
  QString id,
  QString title,
  QString sortid,
  QString firstitemusec,
  QString u,
  QString h,
  QString i2,
  QNetworkAccessManager *q)
  : LinTORFolderItem(id, sortid, title),
    url(u),
    htmlUrl(h),
    iconUrl(i2),
    unreadCount(0),
    qnam(q),
    imageLoader(0)
{
  // Chop the last three chars off the usec:
  firstitemmsec = firstitemusec;
  firstitemmsec.chop(3);

  // Set the timestamp value:
  timestamp.setMSecsSinceEpoch(firstitemmsec.toLongLong());
  setData(0, Qt::UserRole + 1, getTimestamp());

  // Initialize the unread count data role:
  setData(0, Qt::UserRole + 3, unreadCount);

  // Need to do this here, due to TOR strangeness
  loadImage();
}


LinTORSubscriptionItem::LinTORSubscriptionItem(
  const LinTORSubscriptionItem &other)
  : LinTORFolderItem(other),
    imageLoader(0)
{
//  id = other.getID();
//  title = other.getTitle();
//  sortid = other.getSortid();
  firstitemmsec = other.getFirstitemmsec();
  url = other.getUrl();
  htmlUrl = other.getHtmlUrl();
  iconUrl = other.getIconUrl();
  unreadCount = other.getUnreadCount();
  qnam = other.getQnam();

  timestamp.setMSecsSinceEpoch(firstitemmsec.toLongLong());
  setData(0, Qt::UserRole + 1, getTimestamp());
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


QString LinTORSubscriptionItem::getTimestamp()
{
  return timestamp.toString(Qt::ISODate);
}
