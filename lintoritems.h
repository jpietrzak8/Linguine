//
// lintoritems.h
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

#ifndef LINTORITEMS_H
#define LINTORITEMS_H

#include <QTreeWidgetItem>
#include <QString>
#include <QSet>
#include <QMap>

enum LinTORFolderType
{
  Standard_TORFolder,
  Custom_TORFolder
};


class LinTORFolderItem: public QTreeWidgetItem
{
public:
  LinTORFolderItem(
    QString id,
    QString sortid,
    LinTORFolderType type);

  QString getID()
    { return id; }

  QString getLabel()
    { return label; }

private:
  QString id;
  QString sortid;
  QString label;
};

/////////////////////////////////////

class QNetworkAccessManager;
class LinImageLoader;

class LinTORSubscriptionItem: public QTreeWidgetItem
{
public:
  LinTORSubscriptionItem(
    QString id,
    QString title,
    QString sortid,
    QString firstitemmsec,
    QString url,
    QString htmlUrl,
    QString iconUrl,
    QNetworkAccessManager *qnam);

  LinTORSubscriptionItem(
    const LinTORSubscriptionItem &item);

  ~LinTORSubscriptionItem();

/*
  void addCategory(
    QString id,
    QString label);
*/

  void loadImage();

/*
  bool includedInFolder(
    QString folderID);
*/

  QString getID() const { return id; }
  QString getTitle() const { return title; }
  QString getSortid() const { return sortid; }
  QString getFirstitemmsec() const { return firstitemmsec; }
  QString getUrl() const { return url; }
  QString getHtmlUrl() const { return htmlUrl; }
  QString getIconUrl() const { return iconUrl; }
  QNetworkAccessManager *getQnam() const { return qnam; }

private:
  QString id;
  QString title;
  QString sortid;
  QString firstitemmsec;
  QString url;
  QString htmlUrl;
  QString iconUrl;
//  QSet<QString> categoryIDs;  // Do I need the labels?
  QNetworkAccessManager *qnam;
  LinImageLoader *imageLoader;
};

//////////////////////////////////

class LinTORItem
{
public:
  void setID(QString id)
    { torID = id; }

  void setFeedName(QString fn)
    { feedName = fn; }

  void setTitle(QString t)
    { title = t; }

  void setSourceUrl(QString su)
    { sourceUrl = su; }

  void setSummary(QString s)
    { summary = s; }

  void setEnclosureUrl(QString eu)
    { enclosureUrl = eu; }

  void setEnclosureType(QString et)
    { enclosureType = et; }

  QString getID() const { return torID; }
  QString getFeedName() const { return feedName; }
  QString getTitle() const { return title; }
  QString getSourceUrl() const { return sourceUrl; }
  QString getSummary() const { return summary; }
  QString getEnclosureUrl() const { return enclosureUrl; }
  QString getEnclosureType() const { return enclosureType; }

private:
  QString torID;
  QString feedName;
  QString title;
  QString sourceUrl;
  QString summary;
  QString enclosureUrl;
  QString enclosureType;
};

typedef QMap<QString, LinTORItem> LinTORItemCollection;

#endif // LINTORITEMS_H

