//
// mainwindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QNetworkAccessManager>
#include "linmaemo5theme.h"

class LinFlickableTabBar;
class LinNowPlayingForm;
class LinVideoDisplayForm;
class LinHtmlDisplayForm;
class QListWidgetItem;
class LinNewsfeedWidgetItem;
class QXmlStreamReader;
class LinDocumentationForm;
class LinAboutForm;
class QSettings;
//class LinFilterDialog;

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  enum ScreenOrientation {
      ScreenOrientationLockPortrait,
      ScreenOrientationLockLandscape,
      ScreenOrientationAuto
  };

  explicit MainWindow(
    QWidget *parent = 0);

  virtual ~MainWindow();

  // Note that this will only have an effect on Symbian and Fremantle.
  void setOrientation(ScreenOrientation orientation);

  void showExpanded();

  QString getBackgroundColor()
    { return themeSettings.getBackgroundColor(); }

  QString getDefaultTextColor()
    { return themeSettings.getDefaultTextColor(); }

  QString getActiveTextColor()
    { return themeSettings.getActiveTextColor(); }

  QString getSecondaryTextColor()
    { return themeSettings.getSecondaryTextColor(); }

  QString getSystemFontFamily()
    { return themeSettings.getSystemFontFamily(); }

private slots:
  void on_mediaListWidget_itemActivated(QListWidgetItem *item);

  void on_actionManage_Categories_triggered();
  void on_actionLoad_Newsfeeds_File_triggered();
  void on_actionReset_Newsfeeds_triggered();
  void on_actionDocumentation_triggered();
  void on_actionAbout_triggered();

/*
  void on_frequencyComboBox_currentIndexChanged(int index);
  void on_categoryComboBox_currentIndexChanged(int index);
*/

//  void on_filterButton_clicked();

//  void refilter();
  void refilter(QListWidgetItem *item);

private:
  void retrieveNewsfeeds(
    QSettings &settings);

  void parseXML(
    QXmlStreamReader &reader);

  void parseLinguineElement(
    QXmlStreamReader &reader);

  QString parseNameElement(
    QXmlStreamReader &reader);

  QString parseText(
    QXmlStreamReader &reader,
    QString elementName);

  void filterItem(
    LinNewsfeedWidgetItem *nwi);

  Ui::MainWindow *ui;

  LinFlickableTabBar *flickableTabBar;
  LinNowPlayingForm *nowPlayingForm;
  LinVideoDisplayForm *videoDisplayForm;
  LinHtmlDisplayForm *htmlDisplayForm;
  LinDocumentationForm *documentationForm;
  LinAboutForm *aboutForm;
//  LinFilterDialog *filterDialog;

  QNetworkAccessManager qnam;

  LinMaemo5Theme themeSettings;
};

#endif // MAINWINDOW_H
