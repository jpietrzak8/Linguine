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
#include "linfilteritems.h"
#include "linmaemo5theme.h"
#include "lintoritems.h"
#include "lintormanager.h"
#include "lindbus.h"

class LinFlickableTabBar;
class LinNowPlayingForm;
class LinVideoDisplayForm;
//class LinHtmlDisplayForm;
class LinNativeDisplayForm;
class LinTORDisplayForm;
class QListWidgetItem;
class QTreeWidgetItem;
class LinNewsfeedWidgetItem;
class QXmlStreamReader;
class LinPreferencesForm;
class LinDocumentationForm;
class LinAboutForm;
class QSettings;
class LinAuthenticationDialog;

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

  QString getAccentColor()
    { return themeSettings.getAccentColor(); }

  QString getSystemFontFamily()
    { return themeSettings.getSystemFontFamily(); }

  bool hasTORItem(QString id)
    { return torManager->hasTORItem(id); }

  LinTORItem getTORItem(QString id)
    { return torManager->getTORItem(id); }

  void launchDBusPlayer(QString mediaUrl)
    { dbus->launchMedia(mediaUrl); }

private slots:
  void on_sourcesListWidget_itemActivated(QListWidgetItem *item);
  void on_nativeMediaList_itemActivated(QListWidgetItem *item);
  void on_torTreeWidget_itemActivated(QTreeWidgetItem *item, int column);

  void on_actionSelect_New_Source_triggered();
  void on_actionLoad_Newsfeeds_File_triggered();
  void on_actionReset_Newsfeeds_triggered();
  void on_actionPreferences_triggered();
  void on_actionDocumentation_triggered();
  void on_actionAbout_triggered();

  void refilter(QListWidgetItem *item);

  void displayTORItems();

  void retryTORLogin();

private:
  void retrieveNewsfeeds(
    QSettings &settings);

  void parseXML(
    QXmlStreamReader &reader);

  void parseLinguineElement(
    QXmlStreamReader &reader);

  void parseNewsfeedElement(
    QString &name,
    QSet<QString> &tags,
    QXmlStreamReader &reader);

  void parseCollectionElement(
    QString &name,
    QSet<QString> &tags,
    QXmlStreamReader &reader);

  QString parseText(
    QXmlStreamReader &reader,
    QString elementName);

  void filterItem(
    LinNewsfeedWidgetItem *nwi);

  FrequencyType parseFrequency(
    QString freqStr);

  MediaType parseMedia(
    QString mediaStr);

  void setupFeedSources();

  void setupNativeUI();

  void setupTORUI();

  Ui::MainWindow *ui;

  LinNowPlayingForm *nowPlayingForm;
  LinVideoDisplayForm *videoDisplayForm;
//  LinHtmlDisplayForm *htmlDisplayForm;
  LinNativeDisplayForm *nativeDisplayForm;
  LinTORDisplayForm *torDisplayForm;
  LinAuthenticationDialog *torAuthenticationDialog;
  LinPreferencesForm *preferencesForm;
  LinDocumentationForm *documentationForm;
  LinAboutForm *aboutForm;
  LinDBus *dbus;

  bool nativeAlreadySetup;
  LinFlickableTabBar *nativeFlickableTabBar;

  bool torAlreadySetup;
  LinFlickableTabBar *torFlickableTabBar;
  LinTORManager *torManager;

  QNetworkAccessManager qnam;

  LinMaemo5Theme themeSettings;
};

#endif // MAINWINDOW_H
