//
// linatomparser.h
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

#ifndef LINATOMPARSER_H
#define LINATOMPARSER_H

#include <QObject>
#include "linnewsfeedwidgetitem.h"
#include <QString>

class QXmlStreamReader;
class QNetworkReply;
class QNetworkAccessManager;
class LinImageLoader;

class LinAtomParser: public QObject
{
  Q_OBJECT

public:
  LinAtomParser(
    LinNewsfeedWidgetItem *nwi,
    QString sourceUrl,
    QNetworkAccessManager *qnam);

  ~LinAtomParser();

  void startParsing();

signals:
  void itemUpdated();

private slots:
  void parseAtomFeed();

private:
  void parseAtomEntry(
    QXmlStreamReader &newsReader);

  void skipAtomEntry(
    QXmlStreamReader &newsReader);

  QString parseAtomText(
    QString elementName,
    QXmlStreamReader &newsReader);

  LinImageLoader *imageLoader;

  QString title;
  QString imageUrl;

  QString sourceUrl;
  QNetworkAccessManager *qnam;
  QNetworkReply *reply;
  LinNewsfeedWidgetItem *nwi;

//  bool imageAlreadySeen;
};

#endif // LINATOMPARSER_H
