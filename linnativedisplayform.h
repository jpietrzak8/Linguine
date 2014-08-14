//
// linnativedisplayform.h
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

#ifndef LINNATIVEDISPLAYFORM_H
#define LINNATIVEDISPLAYFORM_H

#include "linhtmldisplayform.h"
#include <QWidget>
#include <QString>

class MainWindow;
class QNetworkAccessManager;
class QNetworkReply;
class QXmlStreamReader;
class QWebViewSelectionSuppressor;
class QUrl;

class LinNativeDisplayForm : public LinHtmlDisplayForm
{
  Q_OBJECT
  
public:
  LinNativeDisplayForm(
    MainWindow *mainWindow,
    QNetworkAccessManager *qnam);

  ~LinNativeDisplayForm();

  void displayText(
    QString feedName,
    QString sourceUrl,
    QString faviconUrl,
    bool hideImagesFlag,
    bool openExternalBrowser);

private slots:
  void parseRSSFeed();
  
private:
  void parseRSSChannel(
    QXmlStreamReader &textReader);

  void parseRSSItem(
    QString &htmlOutput,
    QXmlStreamReader &textReader);

  QString parseRSSText(
    QString elementName,
    QXmlStreamReader &textReader);

  QString sourceUrl;
  QString faviconUrl;
  QNetworkAccessManager *qnam;
  QNetworkReply *reply;
};

#endif // LINNATIVEDISPLAYFORM_H
