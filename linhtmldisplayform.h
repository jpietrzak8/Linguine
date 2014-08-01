//
// linhtmldisplayform.h
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

#ifndef LINHTMLDISPLAYFORM_H
#define LINHTMLDISPLAYFORM_H

#include <QWidget>
#include <QString>

class MainWindow;
class QNetworkAccessManager;
class QNetworkReply;
class QXmlStreamReader;
class QWebViewSelectionSuppressor;
class QCloseEvent;
class QUrl;

namespace Ui {
class LinHtmlDisplayForm;
}

class LinHtmlDisplayForm : public QWidget
{
  Q_OBJECT
  
public:
  LinHtmlDisplayForm(
    MainWindow *mainWindow,
    QNetworkAccessManager *qnam);

  ~LinHtmlDisplayForm();

  void displayText(
    QString textUrl);

protected:
  void closeEvent(
    QCloseEvent *event);

private slots:
  void onLinkClicked(
    const QUrl &url);

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

  Ui::LinHtmlDisplayForm *ui;

  MainWindow *mainWindow;

  QString sourceUrl;
  QNetworkAccessManager *qnam;
  QNetworkReply *reply;
  QWebViewSelectionSuppressor *suppressor;

  QString blankHtmlPage;

  bool useExternalBrowser;
};

#endif // LINHTMLDISPLAYFORM_H
