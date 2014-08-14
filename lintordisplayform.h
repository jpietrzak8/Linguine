//
// lintordisplayform.h
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

#ifndef LINTORDISPLAYFORM_H
#define LINTORDISPLAYFORM_H

#include "linhtmldisplayform.h"
#include "lintoritems.h"
#include "linnowplayingform.h"
#include "linvideodisplayform.h"
#include <QString>
#include <QList>

class MainWindow;
class QNetworkAccessManager;
class QNetworkReply;
class QXmlStreamReader;
class QWebViewSelectionSuppressor;
class QUrl;

class LinTORDisplayForm : public LinHtmlDisplayForm
{
  Q_OBJECT
  
public:
  LinTORDisplayForm(
    MainWindow *mainWindow,
    QNetworkAccessManager *qnam);

  ~LinTORDisplayForm();

  void setupTORDisplay(
    QString windowTitle,
    bool openExternalPlayer,
    bool openExternalBrowser);

  void displayItems(
    const LinTORItemCollection &items);

  Q_INVOKABLE void displayTORmedia(
    QString id);

private:
  MainWindow *mainWindow;
  QString sourceUrl;
  QString faviconUrl;
  QNetworkAccessManager *qnam;
  QNetworkReply *reply;

  LinNowPlayingForm *nowPlayingForm;
  LinVideoDisplayForm *videoDisplayForm;
};

#endif // LINTORDISPLAYFORM_H
