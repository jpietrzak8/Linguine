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
class QWebViewSelectionSuppressor;
class QCloseEvent;
class QUrl;
class QWebFrame;

namespace Ui {
class LinHtmlDisplayForm;
}

class LinHtmlDisplayForm : public QWidget
{
  Q_OBJECT
  
public:
  LinHtmlDisplayForm(
    MainWindow *mainWindow);

  ~LinHtmlDisplayForm();

protected:
  void closeEvent(
    QCloseEvent *event);

  void setHtml(
    QString htmlData);

  void setHtml(
    QString htmlData,
    QString sourceUrl);

  QWebFrame *getFrame();

  QString htmlPrefix;

  bool hideImages;
  bool useExternalPlayer;
  bool useExternalBrowser;

private slots:
  void onLinkClicked(
    const QUrl &url);

private:
  QString constructHtmlPrefix();

  Ui::LinHtmlDisplayForm *ui;

  MainWindow *mainWindow;

  QWebViewSelectionSuppressor *suppressor;

  QString blankHtmlPage;
};

#endif // LINHTMLDISPLAYFORM_H
